#ifndef UDP_MOD_H
#define UDP_MOD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdbool.h>

#define UDP_LIMIT 256
#define UDP_RESPONSE_OFFSET 3

void udp_interrupt_handler(int noop);
bool verify_req(char * msg);
int udp_communication(int port);

#endif
