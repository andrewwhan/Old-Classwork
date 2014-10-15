/* Main.c -- Andrew Han */

#include "IO.h"

int main(int argc, char *argv[]){
	int i;
	FILE *input;
	FILE *output;
	Node *tree = NULL;
	for(i=2; i<=(argc-1); i++){
		input = fopen(argv[i], "r");
		parsefile(input, &tree);
		fclose(input);
	}
	output = fopen(argv[1], "w");
	traverseandprint(tree, output);
	fclose(output);
	freetree(tree);
	return 0;
}
