#include <stdlib.h>


// Labels linked list
typedef struct node Node;

void push(Node** head_ref, char *new_data, unsigned int address, char *type);
void printList(Node *node);
int length(Node *head);
bool has(Node *head, char *val);
unsigned int getaddr(Node *head, char *val);

// words linked list
typedef struct word Word;
typedef struct wordnode WordNode;

void pushword(WordNode** head_ref, unsigned int new_data);
void printwordlist(WordNode *node);
void printword(WordNode *node);
int wordlistlength(WordNode *head);