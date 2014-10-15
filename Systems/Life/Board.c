/* Board.c -- Andrew Han */

#include "Life.h"

void initialize(char **board){
	int i, j;
	for(i=0; i<x; i++){
		for(j=0; j<y; j++){
			board[i][j] = 'o';
		}
	}
	return;
} //void initialize()

void shiftboard(char **board, int rows, int columns, int shiftrows, int shiftcolumns){
	int i, j;
	for(i=rows; i>=0; i--){
		for(j=columns; j>=0; j--){
			board[i+shiftrows][j+shiftcolumns] = board[i][j];
			(shiftrows || shiftcolumns) ? board[i][j] = 'o' : 0;
		}
	}
} //void shiftboard()

void printboard(char **board){
	int i, j;
	for(i=0; i<x; i++){
		for(j=0; j<y; j++){
			printf("%c",board[i][j]);
		}
		printf("\n");
	}
} //void printboard()

void ageboards(char **board1, char **board2, char **board3){
	int i, j;
	for(i=0; i<x; i++){
		for(j=0; j<y; j++){
			board3[i][j] = board2[i][j];
			board2[i][j] = board1[i][j];
		}
	}
} //void ageboards()

int compareboards(char **board1, char **board2){
	int issame = 1, i, j;
	for(i=0; i<x; i++){
		for(j=0; j<y; j++){
			(board1[i][j] == board2[i][j]) ? 0 : (issame = 0);
		}
	}
	return issame;
} //int compareboards()

int isbarren(char **board){
	int alldead = 1, i, j;
	for(i=0; i<x; i++){
		for(j=0; j<y; j++){
			(board[i][j] == 'o') ? 0 : (alldead = 0);
		}
	}
	return alldead;
} //int isbarren()
