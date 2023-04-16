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
#include <poll.h>
#include <fcntl.h>
#include "../headers/ipkcpd.h"

#define SERVER_CLIENT_LIMIT 100
extern int * g_socket;


void tcp_interrupt_handler(int noop);
bool tcp_verify_begin(char * begin_msg);
bool tcp_verify_req(char * msg);
void tcp_setup_msg(char * msg,char * res,int status,const char * MSG);

/* Communication setup */
void tcp_update_fds(struct pollfd * fds,int * number_of_fds);
int tcp_communication(int port);


#endif
