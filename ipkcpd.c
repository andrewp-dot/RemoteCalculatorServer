#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

/* Custom modules */
#include "./modules/headers/calculator.h"

/* Exit status */
#define SUCCESS 0
#define EXIT_FAILURE 1 
#define ERROR_EXIT_ARGS 2

#define IP_PART_LIMIT 255
#define MAX_PORT 65535

/* FUNCTION DECLARTIONS */
bool verify_ipv4(char * ip);
bool verify_port(char * port);

int main(int argc, char ** argv)
{
    printf("[IPK2]: Prepare to die...\n");
    
    /* load args */
    
    for(int arg = 1; arg < argc; arg++)
    {
        if(argv[arg][0] == '-')
        {
            if(argv[arg + 1] == NULL) 
            {
                fprintf(stderr,"No option value passed.\n");
                return ERROR_EXIT_ARGS;
            }

            switch (argv[arg][1])
            {
                case 'h':
                    if(!verify_ipv4(argv[++arg])) return ERROR_EXIT_ARGS;
                    break;
                case 'p':
                    if(!verify_port(argv[++arg])) return ERROR_EXIT_ARGS;
                    break;
                case 'm':
                    break;
                default:
                    fprintf(stderr,"Unkown flag: %s\n",argv[arg]);
                    break;
            }

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
    
    int ip_parts_count = 0;
    while (ip != NULL)
    {
        
        char num_part[3];
        for(int idx = 0; isdigit(*ip); idx++)
        {
            if(idx == 3) return false;
            num_part[idx] = *ip;
            ip += 1;
        }
        if(*ip != '.') return false;
        ip += 1;
        ip_parts_count += 1;
        if(ip_parts_count > 4 || atoi(num_part) > IP_PART_LIMIT || atoi(num_part) < 0) return false;
    }
    return true;
}

bool verify_port(char * port)
{
    if (port == NULL) return false;
    while (port != NULL)
    {
        if(!isdigit(*port)) return false;
    }
    if(atoi(port) > MAX_PORT || atoi(port) < 0) return false;
    return true;
}
