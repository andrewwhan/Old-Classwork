life, Andrew Han (Eclipse Version)
life is a program that, that takes arguments and plays John Conway's Game of Life. It can be compiled in Eclipse by building the project. It can be cleaned in Eclipse using the "Clean Project" option.
The program takes the following arguments:
x y gens input print pause
x and y are integers representing the number of rows and the number of columns to use for the board, gens is an integer representing the number of generations you wish to play the game, input is the name of a text file with the desired initial configuration (use o for unoccupied spaces and x for occupied spaces). print should be replaced with either 'y' or 'n' and is optional. If 'y', the program will print the board of every intermediate generation. print defaults to 'n'. pause is another optional argument that should be either 'y' or 'n'. If 'y' the program will wait until the user presses the enter key before processing the next generation. pause also defaults to 'n'.
The files test(1-4).txt are sample input files that demonstrate different functionalities of the program, while test(1-4)out.txt contain the output from the respective test cases. 

test1 was run with arguments
10 10 100 test1.txt n n
and demonstrates how the program terminates when a board is the same as the previous board. (This board ends up as the box pattern)

test2 was run with
10 10 100 test2.txt n n
and demonstrates how the program terminates when the board oscillates.

test3 was run with
10 10 5 test3.txt y n
and demonstrates that the program accurately produces the behavior of the "glider" pattern, as well as how the program terminates upon reaching the desired number of generations.

test4 was run with
10 10 100 test4.txt n n
and demonstrates how the program terminates when the board is empty.

My program is broken into 4 files:
Life.h is my header file, which contains my extern variables x and y, defined in Life.c, which are universal as my board size throughout the program. Life.h also contains declarations for functions that I need to use in files other than the file they were declared in.

Life.c is my main file. It contains the main() function, reads the input, and makes the calls to functions in other files to accomplish the task.

Game.c contains the function playgen() that plays a generation from one board to another. It also contains the function neighborcount() that playgen() uses.

Board.c Contains the functions that analyze the boards and operate using the boards.

The program works by first dynamically allocating memory for the 3 boards it will use to keep track of the game. It allocates memory for 3 arrays of pointers to char, then uses a loop to allocate memory for each of these pointers to char. The loop invariant is that since there should be x rows, it allocates memory for each row k from 0 to x-1. This is x rows total. I then initialize each board by filling them with 'o' with a call to initialize() in Board.c. My program then populates board1 using the input file through the use of nested while loops. The loop invariant of the inner while loop is that it continues to read from the file until it reaches a newline character or the end of file. Each character it reads, it increments the column to put the character in the correct place then stores it in the board. The loop invariant of the outer loop is that it will continue until the end of file has been read. Through every iteration, it will read a line from the file, then move to the next row, and reset the column count. I also include conditionals here to stop the program in the input file is larger than the defined board size. My program then makes a call to a function in Board.c called shiftboard(). With these arguments, the function will shift board1 so that the contents of the input file are approximately centered. shiftboard() uses a set of nested loops that starts with the elements in the lower right of the input configuration, so that as I shift items, I don't overwrite parts of my initial configuration. The loop invariant is that I start at rows, columns, which is the bottom right corner of my input. I decrement columns in my inner loop, and decrement rows in my outer loop until reaching 0,0. This allows me to loop over each element in my input file configuration. Next, I have a loop that plays the game for the desired number of generations. The loop invariant is that k goes from 1 to gens, the input number of generations. This is gens iterations, and each iteration plays 1 generation. However, it also tests for the alternate termination conditions, and provides a return statement if a termination condition is reached. Therefore, the program will break the loop correctly under the termination conditions. I play each generation by first shifting the contents from board2 to board3 and then board1 to board2 through a call to ageboards() in Board.c. I then make a call to playgen() in Game.c to play from the generation in board2 to create the new generation in board1. playgen(), initialize(), printboard(), ageboards(), compareboards(), and isbarren() all share a similar loop structure, using the conditions
	for(i=0; i<x; i++){
		for(j=0; j<y; j++){
The invariant for these loops is that for board size x*y, there are rows number from 0 to x-1 and columns from 0 to y-1. Looping from i=0 to i<x and j=0 to j<y give me the values i = 0 to x-1 and j = 0 to y-1. I can therefore use these nested loops to operate over each individual element of the board.
Upon termination, I print the final board state, the termination condition, and free the memory used for the boards. To free memory, I use a loop nearly identical to the one I used to allocate the memory.