#ifndef UDP_MOD_H
#define UDP_MOD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>
#include "../headers/ipkcpd.h"

#define UDP_LIMIT 256
#define UDP_RESPONSE_OFFSET 3
#define UDP_REQUEST_OFFSET 2

void udp_interrupt_handler(int * socket);
bool udp_verify_req(char * msg);
int udp_communication(int port);

#endif
