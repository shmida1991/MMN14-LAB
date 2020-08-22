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

void handlecmd(char *command, Node *labels) 
{
    char *p;
    p = strtok(command, " \t");
    while(p != NULL)
    {
        SupportedCommand *command;
        if ((command = getcmd(p)) != NULL)
        {
            int numofopr = (*command).numofoper;
            if (numofopr == 0)
            {
                printf("Got not opr");
            }
            else if (numofopr == 1)
            {
                char * opr1 = strtok(NULL, " \t");
                p = opr1;
                if (islegalopr(opr1, true, labels))
                {
                    printf("Got 1 opr for %s\n", (*command).cmd);
                }
               
            }
            else if (numofopr == 2)
            {
                char * opr1 = strtok(NULL, " \t");
                char * opr2 = strtok(NULL, " \t");
                printf("opreands %s %s\n", opr1, opr2);
                if (islegalopr(opr1, false, labels) && islegalopr(opr2, true, labels))
                { 
                    printf("Got 2 opr for %s\n", (*command).cmd);
                }
                p = opr2;
            }
            p = strtok(NULL, " \t");
        }
        else break;
       // check for errors
    }
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
    Node *data = NULL;
    Node *instructions = NULL;
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
                if (strcmp(storecmd, "data") == 0 || strcmp(storecmd, "string") == 0)
                {
                    handlelbl(label, &labels, (unsigned int)DC, "data");
                    DC++;
                    // handle data or string wit
                }
                else if (strcmp(storecmd, "extern") == 0)
                {
                    // add to symbols with label;
                }
                else if (strcmp(storecmd, "entry") == 0)
                {
                    
                } 
                else
                {
                    handlelbl(label, &labels, (unsigned int)IC, "opcode");
                    IC++;
                    handlecmd(storecmd, labels);
                }
            }
            
        }
        else if (line[0] == '.') 
        {
            char *storecmd = (char*)(malloc(strlen(line)));
            strcpy(storecmd, line);
            storecmd++;
            if (strcmp(storecmd, "data") == 0 || strcmp(storecmd, "string"))
            {
                // handle data or string wit
            }
            else if (strcmp(storecmd, "extern") == 0)
            {
                // add to symbols with label;
            }
            else if (strcmp(storecmd, "entry") == 0)
            {
                
            } 
            else
            {
                handlecmd(storecmd, labels);
            }
        }
    }
    
    printList(labels);
    fclose(fp); 
    return(0); 
}




