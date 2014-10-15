/* Game.c -- Andrew Han */

#include "Life.h"

int neighborcount(char **board, int i, int j){
	int neighbors = 0;
	if(i-1 >= 0){
		(board[i-1][j] == 'x') ? neighbors +=1 : 0;
		if(j-1 >= 0){
			(board[i-1][j-1] == 'x') ? neighbors +=1 : 0;
		}
		if(j+1 < y){
			(board[i-1][j+1] == 'x') ? neighbors +=1 : 0;
		}
	}
	if(j-1 >= 0){
		(board[i][j-1] == 'x') ? neighbors +=1 : 0;
		if(i+1 < x){
			(board[i+1][j-1] == 'x') ? neighbors +=1 : 0;
		}
	}
	if(i+1 < x){
		(board[i+1][j] == 'x') ? neighbors +=1 : 0;
		if(j+1 < y){
			(board[i+1][j+1] == 'x') ? neighbors +=1 : 0;
		}
	}
	if(j+1 < y){
		(board[i][j+1] == 'x') ? neighbors +=1 : 0;
	}
	return neighbors;
} //int neighborcount()

void playgen(char **fromboard, char **toboard){
	int i, j, n;
	for(i=0; i<x; i++){
		for(j=0; j<y; j++){
			n = neighborcount(fromboard, i, j);
			toboard[i][j] = 'o';
			if(fromboard[i][j] == 'x'){
				((n == 2) || (n == 3)) ? (toboard[i][j] = 'x') : (toboard[i][j] = 'o');
			}
			else{
				(n == 3) ? (toboard[i][j] = 'x') : (toboard[i][j] = 'o');
			}
		}
	}
} //void playgen()

