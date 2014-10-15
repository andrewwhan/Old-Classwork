/* Life.c -- Andrew Han */

#include "Life.h"
int x, y;

void freememory(char **board1, char **board2, char **board3){
	int k;
	for(k=0; k<x; k++){
		free(board1[k]);
		free(board2[k]);
		free(board3[k]);
	}
	free(board1);
	free(board2);
	free(board3);
} //void freememory()

int main(int argc, char *argv[]){
	FILE *input;
	int i=-1, j=-1, k, gens;
	char print, pause;
	x = atoi(argv[1]);
	y = atoi(argv[2]);
	gens = atoi(argv[3]);
	input = fopen(argv[4], "r");
	if(argc > 5){
		print = *argv[5];
		if(argc > 6) pause = *argv[6];
	}
	char **board1 = calloc(x, sizeof(char*));
	char **board2 = calloc(x, sizeof(char*));
	char **board3 = calloc(x, sizeof(char*));
	for(k=0; k<x; k++){
		board1[k] = calloc(y,sizeof(char));
		board2[k] = calloc(y,sizeof(char));
		board3[k] = calloc(y,sizeof(char));
	}
	initialize(board1);
	initialize(board2);
	initialize(board3);
	char c;
	while(c != EOF){
		j=-1;
		i++;
		if(i==x){
			printf("Input is larger than board size. Please adjust board size and try again.\n");
			freememory(board1, board2, board3);
			return 0;
		}
		while(((c=getc(input)) != '\n') && (c != EOF)){
			if(c != '\r'){
				j++;
				if(j==y){
					printf("Input is larger than board size. Please adjust board size and try again.\n");
					freememory(board1, board2, board3);
					return 0;
				}
				board1[i][j] = c;
			}
		}
	}
	shiftboard(board1, i, j, ((x-1)-i)/2, ((y-1)-j)/2);
	if(pause == 'y') printf("Pause is enabled; please press enter to advance through generations.\n");
	for(k = 1; k <= gens; k++){
		if(print == 'y'){
			printboard(board1);
			printf("\n");
		}
		if(pause == 'y'){
			getchar();
		}
		ageboards(board1, board2, board3);
		playgen(board2, board1);
		if(isbarren(board1)){
			printf("Final board:\n");
			printboard(board1);
			printf("\n");
			printf("Terminated on generation %d due to no living organisms.\n", k);
			freememory(board1, board2, board3);
			return 0;
		}
		if(compareboards(board1, board2)){
			printf("Final board:\n");
			printboard(board1);
			printf("\n");
			printf("Terminated on generation %d since current board is same as previous.\n", k);
			freememory(board1, board2, board3);
			return 0;
		}
		if(compareboards(board1, board3)){
			printf("Final board:\n");
			printboard(board1);
			printf("\n");
			printf("Terminated on generation %d due to oscillation. (This board is the same as two boards ago.)\n", k);
			freememory(board1, board2, board3);
			return 0;
		}
	}
	printf("Final board:\n");
	printboard(board1);
	printf("\n");
	printf("Predefined number of generations reached.\n");
	freememory(board1, board2, board3);
	return 0;
} //int main()
