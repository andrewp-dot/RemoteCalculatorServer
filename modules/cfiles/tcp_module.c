#include "../headers/tcp_module.h"

bool tcp_verify_begin(char * begin_msg)
{
    if(begin_msg == NULL) return false;
    if(!strcmp(begin_msg + TCP_REQUEST_OFFSET,"HELLO\n")) return true;
    return false;
}

bool tcp_verify_req(char * msg)
{
    if(msg == NULL) return false;
    if(!strncmp(msg + TCP_REQUEST_OFFSET,"SOLVE ",7)) return true;
    else return false;
}

int tcp_communication(int port)
{
    char request_buffer[TCP_LIMIT]; 
    memset(request_buffer,0,TCP_LIMIT);

    // validation
    bool hello_sent = false;
    if(!hello_sent) tcp_verify_begin(request_buffer);
    else tcp_verify_req(request_buffer);

    return port;
}
