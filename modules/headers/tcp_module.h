#ifndef TCP_MOD_H
#define TCP_MOD_H

#define TCP_LIMIT 65535

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>
#include "../headers/ipkcpd.h"


bool tcp_verify_begin(char * begin_msg);
bool tcp_verify_req(char * msg);
void tcp_setup_msg(char * msg,char * res,int status,const char * MSG);
int tcp_communication(int port);

#endif
