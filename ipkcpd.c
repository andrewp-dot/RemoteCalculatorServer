/*
<one line to give the program's name and a brief idea of what it does.>
Copyright (C) 2023  Adrián Ponechal (xpone01@stud.fit.vutbr.cz)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <signal.h>

/* Custom modules */
#include "./modules/headers/calculator.h"
#include "./modules/headers/calc_stack.h"
#include "./modules/headers/fraction.h"
#include "./modules/headers/udp_module.h"
#include "./modules/headers/tcp_module.h"
#include "./modules/headers/ipkcpd.h"

int * g_socket = NULL;
connection_mode_t mode = undef;
struct pollfd * polled_fds = NULL;

int main(int argc, char ** argv)
{   
    int port = PORT_UNDEFINED;
    
    signal(SIGINT,interrupt_handler);
    signal(SIGTERM,interrupt_handler);
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

    #ifdef TEST_CALC
    char char_arr[UDP_LIMIT] = {0,};
    char * expr = char_arr;

    int c;
    int idx = 0;

    FILE * fp = fopen("./tests/calc_test.txt","r");
    
    while ((c = fgetc(fp)) != EOF)
    {
        if(idx >= UDP_LIMIT) break;
        expr[idx] = c;
        idx++;
        if(c == '\n')
        {
            char only_equation[UDP_LIMIT];
            char res[UDP_LIMIT];
            strcpy(only_equation,expr);
            only_equation[strlen(only_equation)-1] = '\0';
            printf("%s = ",only_equation);

            frac_t result = get_result(&expr);
            if(result.denominator == 0 || result.numerator < 0) strcpy(res,"ERR");
            else frac_to_string_floored(result,res);
            printf("%s",res);
            printf("\n");

            idx = 0;
            memset(expr,0,UDP_LIMIT);
            memset(res,0,UDP_LIMIT);
        }
    }

    
    fclose(fp);
    return SUCCESS;
    #endif

    #ifdef TEST_ARGS
    return SUCCESS;
    #endif

    if(mode == udp) return udp_communication(port);
    else if(mode == tcp) return tcp_communication(port);
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
        if(ip_parts_count >= 4 || atoi(ip_part) > IP_PART_LIMIT || atoi(ip_part) < 0) return false;
    }
    return true;
}

bool verify_port(char * port)
{
    if (port == NULL) return false;
    char * start = port;
    while (*port != '\0')
    {
        if(!isdigit(*port)) return false;
        port += 1;
    }
    if(atoi(start) > MAX_PORT || atoi(start) < 0) return false;
    
    return true;
}

connection_mode_t get_mode(char * mode)
{
    if(!strcmp("tcp",mode)) return tcp;
    else if(!strcmp("udp",mode)) return udp;
    else return undef;
}

void interrupt_handler(int num) 
{
    if(mode == udp) udp_interrupt_handler(num);
    else if (mode == tcp) tcp_interrupt_handler(num);
    else exit(SUCCESS);
}
