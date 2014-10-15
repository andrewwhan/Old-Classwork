/* Tree.c -- Andrew Han */

#include "Tree.h"

struct node{
	char* word;
	int occurrence;
	Node* left;
	Node* right;
};

Node* addword(Node** tree, char* newword){
	if(*tree == NULL){
		*tree = malloc(sizeof(Node));
		char *c = malloc(strlen(newword)+1);
		(*tree)->word = strcpy(c, newword);
		(*tree)->occurrence = 1;
		(*tree)->left = (*tree)->right = NULL;
		return *tree;
	}

	int c = strcmp(newword, (*tree)->word);
	if(c == 0) (*tree)->occurrence++;
	else if(c < 0) (*tree)->left = addword(&(*tree)->left, newword);
	else (*tree)->right = addword(&(*tree)->right, newword);
	return *tree;
}

void traverse(Node* tree, int* count, FILE* output){
	if(tree != NULL){
		traverse(tree->left, count, output);
		(*count)++;
		fprintf(output, "%6d\t%s\n", tree->occurrence, tree->word);
		traverse(tree->right, count, output);
	}
}

void freetree(Node* tree){
	if(tree != NULL){
		freetree(tree->left);
		freetree(tree->right);
		free(tree->word);
		free(tree);
	}
}
