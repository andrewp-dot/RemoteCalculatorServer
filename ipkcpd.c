#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

/* Custom modules */
#include "calculator.h"

/* Exit status */
#define SUCCESS 0
#define EXIT_FAILURE 1 
#define ERROR_EXIT_ARGS 2

#define NEXT_ARG arg + 1
#define IP_PART_LIMIT 255

/* FUNCTION DECLARTIONS */
bool verify_ipv4(char * ip);

int main(int argc, char ** argv)
{
    printf("[IPK2]: Prepare to die...\n");
    
    /* load args */
    
    for(int arg = 1; arg < argc; arg++)
    {
        switch (argv[arg][0])
        {
        case '-':
            printf("Short option\n");

            if(argv[NEXT_ARG] == NULL) 
            {
                fprintf(stderr,"No option value passed.\n");
                return ERROR_EXIT_ARGS;
            }
            break;
        
        default:
            printf("arg %d: %s\n",arg,argv[arg]);
            break;
        }
        
    }
    calculator();
    return SUCCESS;
}

/* FUNCTION DEFINITIONS */
bool verify_ipv4(char * ip)
{
    int ip_length = strlen(ip);
    if(ip_length == 0) return false;
    if(!(isdigit(ip[0]) && isdigit(ip[ip_length - 1]))) return false;

    for(int ip_parts_count = 0; ip != NULL; ip_parts_count++)
    {
        char num_part[3];
        for(int idx = 0; isdigit(*ip); idx++)
        {
            if(idx == 3) return false;
            num_part[idx] = *ip;
            ip += 1;
        }
        if(ip_parts_count > 4) return false;
        if(atoi(num_part) > IP_PART_LIMIT || atoi(num_part) < 0) return false;

    }
    return true;
}