/* Tree.h -- Andrew Han */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct node Node;

Node* addword(Node** tree, char* word);
void traverse(Node* tree, int* count, FILE* output);
void freetree(Node* tree);
