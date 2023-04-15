#ifndef IPKCPD_H
#define IPKCPD_H

/* Exit codes */
#define SUCCESS 0
#define EXIT_FAILURE 1 
#define ERROR_EXIT_ARGS 10

#define USAGE "./ipcpd -h <host> -p <port> -m <mode>\n"

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

/* FUNCTION DECLARTIONS */
bool verify_ipv4(char * ip);
bool verify_port(char * port);
connection_mode_t get_mode(char * mode);
void close_connection(int num);

#endif
