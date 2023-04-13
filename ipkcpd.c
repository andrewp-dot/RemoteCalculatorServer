#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

/* Custom modules */
#include "./modules/headers/calculator.h"

#define USAGE "./ipcpd -h <host> -p <port> -m <mode>\n"

/* Exit status */
#define SUCCESS 0
#define EXIT_FAILURE 1 
#define ERROR_EXIT_ARGS 2

/* port macros*/
#define PORT_UNDEFINED -1
#define MAX_PORT 65535

/* ipv4 macros */
#define IPV4_LENGTH 16 //255.255.255.255\0
#define IP_PART_LIMIT 255

typedef enum connection_mode{
    tcp,
    udp,
    undef
}connection_mode_t;

int port = PORT_UNDEFINED;
connection_mode_t mode = undef;

/* FUNCTION DECLARTIONS */
bool verify_ipv4(char * ip);
bool verify_port(char * port);
connection_mode_t get_mode(char * mode);

int main(int argc, char ** argv)
{   
    char ip_address[IPV4_LENGTH] = {0};

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
                    strcpy(ip_address,argv[arg]);
                    break;
                case 'p':
                    if(!verify_port(argv[++arg])) return ERROR_EXIT_ARGS;
                    port = atoi(argv[arg]);
                    break;
                case 'm':
                    mode = get_mode(argv[++arg]);
                    break;
                default:
                    fprintf(stderr,"Unkown flag: %s\n",argv[arg]);
                    break;
            }

        }
    }
    if(port == PORT_UNDEFINED || strlen(ip_address) == 0 || mode == undef)
    {
        fprintf(stderr, "Undefined required parameters.\n%s",USAGE);
        return ERROR_EXIT_ARGS;
    }
    return SUCCESS;
}

/* FUNCTION DEFINITIONS */
bool verify_ipv4(char * ip)
{
    int ip_length = strlen(ip);
    if(ip_length == 0) return false;
    if(!(isdigit(ip[0]) && isdigit(ip[ip_length - 1]))) return false;

    int ip_parts_count = 0;

    while (*ip != '\0')
    {
        char ip_part[3] = {0,};
        for(int idx = 0; *ip != '.' && *ip != '\0'; idx++)
        {
            if(idx >= 3 || !isdigit(*ip)) return false;
            ip_part[idx] = *ip;
            ip += 1;
        }
        if(*ip == '\0') break;
        ip += 1;
        ip_parts_count += 1;
        if(ip_parts_count > 4 || atoi(ip_part) > IP_PART_LIMIT || atoi(ip_part) < 0) return false;
    }
    return true;
}

bool verify_port(char * port)
{
    if (port == NULL) return false;
    while (*port != '\0')
    {
        if(!isdigit(*port)) return false;
        port += 1;
    }
    if(atoi(port) > MAX_PORT || atoi(port) < 0) return false;
    return true;
}

connection_mode_t get_mode(char * mode)
{
    if(!strcmp("tcp",mode)) return tcp;
    else if(!strcmp("udp",mode)) return udp;
    else return undef;
}
