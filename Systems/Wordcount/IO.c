/* IO.c -- Andrew Han */

#include "IO.h"

char* readword(FILE* input){
	char* word = calloc(32, sizeof(char));
	char c;
	int i=1;
	while(i<32){
		c = getc(input);
		if(isalpha(c)||c=='-'||c=='\''){
			char temp[2];
			temp[0] = tolower(c);
			temp[1] = '\0';
			word = strcat(word, temp);
			i++;
		}
		else break;
	}
	int isword=0;
	for(i=0; i<31; i++){
		if(isalpha(word[i])) isword=1;
	}
	if(isword) return word;
	else return NULL;
}

void parsefile(FILE* input, Node** tree){
	while(!feof(input)){
		char* word = readword(input);
		if(word != NULL) addword(tree, word);
		free(word);
	}
}

void traverseandprint(Node* tree, FILE* output){
	int count=0;
	traverse(tree, &count, output);
	fprintf(output, "----------\n");
	fprintf(output, "%6d\tTotal number of different words", count);
}
