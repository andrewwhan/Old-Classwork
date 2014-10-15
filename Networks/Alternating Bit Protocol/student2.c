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

//Hold previously sent and received seq/ack numbers
int aSent = 0;
int aRec = 0;
int bSent = 0;
int bRec = 0;

//Last sent packet for resending purposes
struct pkt aPacket;

//Cyclical array queue
struct msg buffer[MAX_BUFFER];
int bufferFront = 0;
int bufferCount = 0;

void A_output(struct msg message) {
	//If last message was acknowledged and nothing in the buffer send packet
	if(aSent == aRec && bufferCount == 0){
		//Construct packet
		aSent = !aSent;
		struct pkt packet = {aSent, aRec};
		memcpy(packet.payload, message.data, MESSAGE_LENGTH);
		packet.checksum = checksum(packet.seqnum, packet.acknum, packet.payload);

		//Store packet for resending
		aPacket = packet;

		startTimer(AEntity, TIMEOUT);
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
	//If next packet is acked and the ack is not corrupt
	if(aRec == !packet.acknum && 
		checksum(packet.seqnum, packet.acknum, packet.payload) == packet.checksum){
		aRec = packet.acknum;
		stopTimer(AEntity);

		//Check buffer for waiting messages and send the next one
		if(bufferCount != 0){
			//Construct packet
			aSent = !aSent;
			struct pkt packet = {aSent, aRec};
			memcpy(packet.payload, buffer[bufferFront].data, MESSAGE_LENGTH);
			packet.checksum = checksum(packet.seqnum, packet.acknum, packet.payload);

			//Remove from buffer
			bufferFront = (bufferFront + 1) % MAX_BUFFER;
			bufferCount--;

			//Store for resending
			aPacket = packet;
			startTimer(AEntity, TIMEOUT);
			tolayer3(AEntity, packet);
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
	//Resend last packet and start the timer
	startTimer(AEntity, TIMEOUT);
	tolayer3(AEntity, aPacket);
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
	//If the packet is new and is not corrupt:
	if(packet.seqnum != bRec &&
		checksum(packet.seqnum, packet.acknum, packet.payload) == packet.checksum &&
		packet.seqnum < 2 &&
		packet.acknum < 2){
		//Send an ack
		bRec = packet.seqnum;
		struct pkt ack = {bSent, bRec};
		ack.checksum = checksum(ack.seqnum, ack.acknum, ack.payload);
		tolayer3(BEntity, ack);

		//Extract data and send to application layer
		struct msg message;
		memcpy(message.data, packet.payload, MESSAGE_LENGTH);
		tolayer5(BEntity, message);
	}
	//Resend previous acknowledgement
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