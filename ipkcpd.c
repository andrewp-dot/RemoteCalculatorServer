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
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

/* Custom modules */
#include "./modules/headers/calculator.h"
#include "./modules/headers/fraction.h"
#include "./modules/headers/udp_module.h"
#include "./modules/headers/tcp_module.h"


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

    // testing_run: make run ARGS="-h 123.123.45.23 -p 01 -m tcp"
    char * char_arr = (char *)malloc(100*sizeof(char));
    char * expr = char_arr;

    char c;
    int idx = 0;

    //testing file here
    FILE * fp = fopen("calc_test.txt","r");
    
    while ((c = fgetc(fp)) != EOF)
    {
        expr[idx] = c;
        idx++;
        if(c == '\n')
        {
            printf("%s = ",expr);
            print_frac(get_result(&expr));
            printf("\n");
            idx = 0;
            memset(char_arr,0,strlen(char_arr)*sizeof(char));
        }
    }

    fclose(fp);
    free(char_arr);

    // frac_t test;
    // frac_t test2;
    // test.numerator = 3;
    // test.denominator = 4;

    // test2.numerator = 4;
    // test2.denominator = 5;
    
    // print_frac(frac_add(test,test2));
    // printf("\n");
    // print_frac(frac_sub(test,test2));
    // printf("\n");
    // print_frac(frac_mul(test,test2));
    // printf("\n");
    // print_frac(frac_div(test,test2));
    // printf("\n");

    

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
