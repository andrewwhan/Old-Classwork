#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "project2.h"
 
/* ***************************************************************************
 ALTERNATING BIT AND GO-BACK-N NETWORK EMULATOR: VERSION 1.1  J.F.Kurose

   This code should be used for Project 2, unidirectional or bidirectional
   data transfer protocols from A to B and B to A.
   Network properties:
   - one way network delay averages five time units (longer if there
     are other messages in the channel for GBN), but can be larger
   - packets can be corrupted (either the header or the data portion)
     or lost, according to user-defined probabilities
   - packets may be delivered out of order.

   Compile as gcc -g project2.c student2.c -o p2
**********************************************************************/



/********* STUDENTS WRITE THE NEXT SEVEN ROUTINES *********/
/* 
 * The routines you will write are detailed below. As noted above, 
 * such procedures in real-life would be part of the operating system, 
 * and would be called by other procedures in the operating system.  
 * All these routines are in layer 4.
 */

/* 
 * A_output(message), where message is a structure of type msg, containing 
 * data to be sent to the B-side. This routine will be called whenever the 
 * upper layer at the sending side (A) has a message to send. It is the job 
 * of your protocol to insure that the data in such a message is delivered 
 * in-order, and correctly, to the receiving side upper layer.
 */
#define TIMEOUT 100
#define MAX_BUFFER 50
#define WINDOW 8

//Hold previously sent and received seq/ack numbers
int aSent = 0;
int aRec = 1;
int bSent = 0;
int bRec = 1;

//Cyclical array queues
struct pkt inWindow[WINDOW];
int windowFront = 0;
int windowCount = 0;

struct msg buffer[MAX_BUFFER];
int bufferFront = 0;
int bufferCount = 0;

void A_output(struct msg message) {
	//If there's space in the window and nothing in the buffer send packet
	if((aSent + 1 < aRec + WINDOW) && bufferCount == 0){
		//Construct packet
		aSent++;
		struct pkt packet = {aSent, aRec};
		memcpy(packet.payload, message.data, MESSAGE_LENGTH);
		packet.checksum = checksum(packet.seqnum, packet.acknum, packet.payload);
		//Add the packet to the window
		inWindow[(windowFront + windowCount) % WINDOW] = packet;
		windowCount++;

		//Start timer if this is the only packet in the window
		if(windowCount == 1){
			startTimer(AEntity, TIMEOUT);
		}
		tolayer3(AEntity, packet);
	}
	//Otherwise, buffer the new message
	else{
		buffer[(bufferFront + bufferCount) % MAX_BUFFER] = message;
		bufferCount++;
		//Give up if buffer is overloaded
		if(bufferCount > MAX_BUFFER){
			printf("ERROR: Message buffer overloaded \n");
			exit(0);
		}
	}
}

/*
 * Just like A_output, but residing on the B side.  USED only when the 
 * implementation is bi-directional.
 */
void B_output(struct msg message)  {

}

/* 
 * A_input(packet), where packet is a structure of type pkt. This routine 
 * will be called whenever a packet sent from the B-side (i.e., as a result 
 * of a tolayer3() being done by a B-side procedure) arrives at the A-side. 
 * packet is the (possibly corrupted) packet sent from the B-side.
 */
void A_input(struct pkt packet) {
	//If a newer packet is acked and the ack is not corrupt
	if(aRec < packet.acknum && 
		packet.acknum <= aRec + WINDOW &&
		checksum(packet.seqnum, packet.acknum, packet.payload) == packet.checksum){
		//Clear the acked packets from the window
		int numAcked = packet.acknum - aRec;
		windowFront += numAcked;
		windowCount -= numAcked;

		//Record the last acked packet and stop the timer
		aRec = packet.acknum;
		stopTimer(AEntity);

		//Check buffer for waiting messages and send until buffer's empty or window's full
		while(bufferCount != 0 && windowCount != WINDOW){
			//Construct packet
			aSent++;
			struct pkt packet = {aSent, aRec};
			memcpy(packet.payload, buffer[bufferFront].data, MESSAGE_LENGTH);
			packet.checksum = checksum(packet.seqnum, packet.acknum, packet.payload);

			//Add to window
			inWindow[(windowFront + windowCount) % WINDOW] = packet;
			windowCount++;

			//Remove from buffer
			bufferFront = (bufferFront + 1) % MAX_BUFFER;
			bufferCount--;
			
			//Send
			tolayer3(AEntity, packet);
		}
		//Restart timer if packets in window
		if(windowCount > 0){
			startTimer(AEntity, TIMEOUT);
		}
	}
}

/*
 * A_timerinterrupt()  This routine will be called when A's timer expires 
 * (thus generating a timer interrupt). You'll probably want to use this 
 * routine to control the retransmission of packets. See starttimer() 
 * and stoptimer() in the writeup for how the timer is started and stopped.
 */
void A_timerinterrupt() {
	//Resend all packets in the window and start the timer
	int i;
	for(i=0; i<windowCount; i++){
		tolayer3(AEntity, inWindow[(windowFront + i) % WINDOW]);
	}
	startTimer(AEntity, TIMEOUT);
}  

/* The following routine will be called once (only) before any other    */
/* entity A routines are called. You can use it to do any initialization */
void A_init() {
}


/* 
 * Note that with simplex transfer from A-to-B, there is no routine  B_output() 
 */

/*
 * B_input(packet),where packet is a structure of type pkt. This routine 
 * will be called whenever a packet sent from the A-side (i.e., as a result 
 * of a tolayer3() being done by a A-side procedure) arrives at the B-side. 
 * packet is the (possibly corrupted) packet sent from the A-side.
 */
void B_input(struct pkt packet) {
	//If the packet has the expected sequence number and is not corrupt:
	if(packet.seqnum == bRec &&
		checksum(packet.seqnum, packet.acknum, packet.payload) == packet.checksum){
		//Send an ack with the next expected sequence number
		bRec++;
		struct pkt ack = {bSent, bRec};
		ack.checksum = checksum(ack.seqnum, ack.acknum, ack.payload);
		tolayer3(BEntity, ack);

		//Extract data and send to application layer
		struct msg message;
		memcpy(message.data, packet.payload, MESSAGE_LENGTH);
		tolayer5(BEntity, message);
	}
	//Resend acknowledgement for next expected sequence number
	else{
		struct pkt ack = {bSent, bRec, 0};
		ack.checksum = checksum(ack.seqnum, ack.acknum, ack.payload);
		tolayer3(BEntity, ack);
	}
}

/*
 * B_timerinterrupt()  This routine will be called when B's timer expires 
 * (thus generating a timer interrupt). You'll probably want to use this 
 * routine to control the retransmission of packets. See starttimer() 
 * and stoptimer() in the writeup for how the timer is started and stopped.
 */
void  B_timerinterrupt() {
}

/* 
 * The following routine will be called once (only) before any other   
 * entity B routines are called. You can use it to do any initialization 
 */
void B_init() {
}

//8-bit BSD checksum
int checksum(int seqnum, int acknum, char payload[20]) {
	int checksum = seqnum & 0xFF;
	checksum = (checksum >> 1) + ((checksum & 0x1) << 7);
	checksum = (checksum + acknum) & 0xFF;

	int i;
	for(i = 0; i < 20; i++){
		checksum = (checksum >> 1) + ((checksum & 0x1) << 7);
		checksum = (checksum + payload[i]) & 0xFF;
	}
	return checksum;
}