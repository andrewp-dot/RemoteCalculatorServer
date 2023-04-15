#ifndef TCP_MOD_H
#define TCP_MOD_H

#define TCP_LIMIT 65535
#define TCP_REQUEST_OFFSET 2
#define TCP_RESPONSE_OFFSET 3

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
int tcp_communication(int port);

#endif
