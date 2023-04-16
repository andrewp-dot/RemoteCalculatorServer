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

/**
 * SOURCES:
 * https://www.programiz.com/c-programming/examples/hcf-gcd
 * https://www.programiz.com/c-programming/examples/lcm
 * https://www.sanfoundry.com/c-program-integer-to-string-vice-versa/
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <signal.h>

/* Custom modules */
#include "./modules/headers/calculator.h"
#include "./modules/headers/fraction.h"
#include "./modules/headers/udp_module.h"
#include "./modules/headers/tcp_module.h"
#include "./modules/headers/ipkcpd.h"

// void interrupt_handler(int noop);
int port = PORT_UNDEFINED;
connection_mode_t mode = undef;

int main(int argc, char ** argv)
{   
    signal(SIGINT,close_connection);
    signal(SIGTERM,close_connection);
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

    

    // testing_run: make run ARGS="-h 127.0.0.1 -p 2023 -m udp"

    // char char_arr[UDP_LIMIT] = {0,};
    // char * expr = char_arr;

    // char c;
    // int idx = 0;

    // // #ifdef TEST
    // FILE * fp = fopen("./tests/calc_test.txt","r");
    
    // while ((c = fgetc(fp)) != EOF)
    // {
    //     if(idx >= UDP_LIMIT) break;
    //     expr[idx] = c;
    //     idx++;
    //     if(c == '\n')
    //     {
    //         char only_equation[100];
    //         strcpy(only_equation,expr);
    //         only_equation[strlen(only_equation)-1] = '\0';
    //         printf("%s = ",only_equation);
    //         print_frac(get_result(&expr));
    //         printf("\n");
    //         idx = 0;
    //         memset(char_arr,0,strlen(expr)*sizeof(char));
    //     }
    // }

    // fclose(fp);
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

void close_connection(int num) 
{
    exit(SUCCESS);
}
