/* Life.h -- Andrew Han */

#include <stdio.h>
#include <stdlib.h>

extern int x, y;

//Functions from Board.c
void initialize(char **board);
void shiftboard(char **board, int rows, int columns, int shifti, int shiftj);
void printboard(char **board);
void ageboards(char **board1, char **board2, char **board3);
int compareboards(char **board1, char **board2);
int isbarren(char **board);

//Functions from Game.c
void playgen(char **fromboard, char **toboard);
