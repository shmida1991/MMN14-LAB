#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "linkedlist.h"
#define NUMBER_OF_SUPPORTED_COMMANDS 16

typedef struct suppoertedcmd {
    char *cmd;
    unsigned int funct;
    unsigned int opcode;
    unsigned int numofoper;
} SupportedCommand;

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

const SupportedCommand commands[NUMBER_OF_SUPPORTED_COMMANDS] = {
    {"move", 0, 0, 2},
    {"cmp", 0, 1, 2},
    {"add", 1, 2, 2},
    {"sub", 2, 2, 2},
    {"lea", 0, 4, 2},
    {"clr", 1, 5, 1},
    {"not", 2, 5, 1},
    {"inc", 3, 5, 1},
    {"dec", 4, 5, 1},
    {"jmp", 1, 9, 1},
    {"bne", 2, 9, 1},
    {"jsr", 3, 9, 1},
    {"red", 0 ,12, 1},
    {"prn", 0, 13, 1},
    {"rts", 0, 14, 0},
    {"stop", 0, 15, 0}
};

void handleinstr(char *instruction, Node *instructions)
{
    printf("Handling instruction %s\n", instruction);
}

SupportedCommand* getcmd(char *command) 
{
    for (int i = 0; i < NUMBER_OF_SUPPORTED_COMMANDS; i++)
    {
        SupportedCommand current_command = commands[i];
        SupportedCommand *cmdptr = &current_command;
        if (strcmp(current_command.cmd, command) == 0)
            return cmdptr;
    }

    return NULL;
}

bool isregister(char *operand)
{
    printf("operand %s %c %d %d %d\n", operand, operand[0], operand[1] - '0',(int)strlen(operand), operand[0] == 'r');
    return (strlen(operand) == 2 && operand[0] == 'r' && (((int)(operand[1] - '0')) >= 1 && ((int)(operand[1] - '0')) <= 6));
}

bool islegalopr(char *operand, bool islast, Node *labels)
{
    printList(labels);
    int len = strlen(operand);
    if (!islast && operand[len - 1] != ',')
    {
        printf("illigal operand %s\n", operand);
        return false;
    }
    else if (islast && operand[len - 1] == ',')
    {
        printf("illigal operand %s\n", operand);
        return false;
    }
    if (!islast && operand[len - 1] == ',')
    {
        operand[len - 1] = 0;
    }

    if (isregister(operand)) {
        
        printf("trimmed %s\n", operand);
        printf("found register %s\n", operand);
        return true;
    }

    return false;
}

void handlecmd(char *commandstr, char *operands, Node *labels) 
{
   printf("got command %s with %s", commandstr, operands);
}

void handlelbl(char *label, Node **labels, unsigned int address, char* type)
{
    printf("handling label %s %s %d\n", label, type, address);
    push(labels, label, address, type);
}

void handlestorecmd(char *datatype)
{
    
}

int main(int argc, char const *argv[])
{
    int IC = 100, DC = 0;
    Node *labels = NULL;
    WordNode **data = NULL;
    WordNode *instructions = NULL;
    char line[80];
    int t;

        // open the file 
    FILE *fp = fopen("input2.as","r"); 
  
    // Return if could not open file 
    if (fp == NULL) {
      perror("error opening file");
      return(-1);
    }

    while(fgets(line, 80, fp))
    {
        
        bool haslabel = false;
        if (line[0] == '\n') continue;
        else if (line[0] == ';')
        {
            printf("comment found \n");
            continue;
        }
        else if ((t = (int)(strcspn(line, ":"))) != strlen(line)) 
        {
            printf("with label\n");
            haslabel = true;
            char *label = (char*)(malloc(t));
            char* restcommand =(char*)malloc(strlen(line) - t);
            strncpy(label, line, (size_t)t);
            strncpy(restcommand, line + t + 1, (size_t)(strlen(line) - t));
            restcommand = strtok(restcommand, " \t");
            if (restcommand[0] == '.') 
            {
                char * operand = strtok(NULL, " \t\n");
                if (has(labels, label))
                {
                    // error;
                }
                char *storecmd = (char*)(malloc(strlen(restcommand)));
                strcpy(storecmd, restcommand);
                storecmd++;
                if (strcmp(storecmd, "data") == 0)
                {
                    handlelbl(label, &labels, (unsigned int)DC, "data");
                    while(operand != NULL) // check if operand legal
                    {
                        unsigned int val = operand[0] == '-' ? (int)(0 - (unsigned int)(atoi(operand++))) : atoi(operand);
                        pushword(data, val);
                        DC++;
                        operand = strtok(operand, " \t\n");
                    }
                }
                else if (strcmp(storecmd, "string") == 0) 
                {
                    if (operand[0] == '"' && operand[strlen(operand) - 1] == '"')
                    {
                        for(int i = 0;  i < strlen(operand++) - 1; i++)
                        {
                            
                            printf("%i\n",(unsigned int)(operand[i]));
                            pushword(data, (unsigned int)(operand[i]));
                            DC++;
                        }
                        DC++;
                    }
                }
                else if (strcmp(storecmd, "extern") == 0)
                {
                    handlelbl(operand, &labels, 0, "external");
                }
                else if (strcmp(storecmd, "entry") == 0)
                {
                    // write to entries file
                } 
                else
                {
                    handlelbl(label, &labels, (unsigned int)IC, "opcode");
                    IC++;
                    handlecmd(storecmd, operand, labels);
                }
            }
            
        }
        else if (line[0] == '.') 
        {
            printf("without label\n");
            char * operand = strtok(NULL, " \t\n");
            char *storecmd = (char*)(malloc(strlen(line)));
            strcpy(storecmd, line);
            storecmd++;
            if (strcmp(storecmd, "data") == 0)
            {
                while(operand != NULL) // check if operand legal
                {
                    pushword(data, operand[0] == '-' ? (unsigned int)(0 - atoi(operand++)) : (unsigned int)(atoi(operand)));
                    DC++;
                    operand = strtok(operand, " \t\n");
                }
            }
            else if (strcmp(storecmd, "string") == 0) 
            {
                if (operand[0] == '"' && operand[strlen(operand) - 1] == '"')
                {
                    for(int i = 0;  i < strlen(operand); i++)
                    {
                        pushword(data, (unsigned int)(operand[i]));
                        DC++;
                    }
                    DC++;
                }
            }
            else if (strcmp(storecmd, "extern") == 0)
            {
                handlelbl(operand, &labels, 0, "external");
            }
            else if (strcmp(storecmd, "entry") == 0)
            {
                // write to entries file
            } 
            else
            {
                IC++;
                handlecmd(storecmd, operand, labels);
            }
        }
    }
    
    printList(labels);
    fclose(fp); 
    return(0); 
}




