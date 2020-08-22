#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "linkedlist.h"

typedef struct word {
    unsigned int E :1;
    unsigned int R :1;
    unsigned int A :1;
    unsigned int funct :5;
    unsigned int dst_reg :3;
    unsigned int dst :2;
    unsigned int src_reg :3;
    unsigned int src :2;
    unsigned int optcode :6;
} Word;

typedef struct node 
{ 
    char *data; 
    unsigned int address;
    char *type;
    Node *next; 
} Node;

Node* getlastnode(Node** head_ref)
{printf("1\n");
    Node* current = (*head_ref);
    printf("1 %d\n", current->next);
    while (current->next != NULL)
    {
        current = current->next;
    }
    return current;
}

void push(Node** head_ref, char *new_data, unsigned int address, char *type) 
{
    printf("1\n");
    Node* new_node = (Node*)malloc(sizeof(Node)); 
    printf("2\n");
    Node* last_node = getlastnode(head_ref);
    printf("3\n");
    new_node->data  = malloc(sizeof(char*));
    new_node->type  = malloc(sizeof(char*));
    printf("4\n");
    new_node->next = NULL; 
    new_node->address = address;
  printf("5\n");
    strcpy(new_node->data, new_data);
    strcpy(new_node->type, type);
    printf("6\n");
    last_node->next = new_node;
}

void printList(Node *node) 
{ 
    while (node != NULL) 
    { 
        printf("%s %d %s\n", node->data, node->address, node->type);
        node = node->next; 
    } 
} 

int length(Node *head) 
{
   int length = 0;
   Node *current;
	
   for(current = head; current != NULL; current = current->next) {
      length++; 
   }
	
   return length;
}

bool has(Node *head, char *val) 
{
    Node *current = head;
    while(current != NULL)
    {
        if (strcmp(current->data, val) == 0) return true;
        current = current->next;
    }

    return false;
}

unsigned int getaddr(Node *head, char *val)
{
    Node *current = head;

    while(current != NULL)
    {
        if (strcmp(current->data, val) == 0) return current->address;
        current = current->next;
    }

    return 0;
}


typedef struct wordnode 
{ 
    Word *word;
    WordNode *next; 
} WordNode;

WordNode* getlastwordnode(WordNode** head_ref)
{
    WordNode* current = (*head_ref);
    while (current->next != NULL)
    {
        current = current->next;
    }
    return current;
}

void pushword(WordNode** head_ref, Word new_data) 
{
    WordNode* new_node = (WordNode*)malloc(sizeof(WordNode)); 
    WordNode *last_node = getlastwordnode(head_ref);

    new_node->word = malloc(sizeof(Word)); 
    new_node->word = &new_data;
    new_node->next = NULL;
    last_node->next = new_node;
} 

void printword (WordNode *node)
{
    printf("Priting word\n");
}
void printwordlist(WordNode *node) 
{ 
    while (node != NULL) 
    { 
        printword(node);
        node = node->next; 
    } 
} 

