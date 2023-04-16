#include "../headers/tcp_module.h"
#include "../headers/calculator.h"

bool tcp_verify_begin(char * begin_msg)
{
    if(begin_msg == NULL) return CONNECTION_MSG_ERR;
    if(!strcmp(begin_msg,"HELLO\n")) return CONNECTION_MSG_OK;
    return CONNECTION_MSG_ERR;
}

bool tcp_verify_req(char * msg)
{
    if(msg == NULL) return CONNECTION_MSG_ERR;
    if(!strncmp(msg,"SOLVE ",strlen("SOLVE "))) return CONNECTION_MSG_OK;
    else return CONNECTION_MSG_ERR;
}

void tcp_setup_msg(char * msg,char * res,int status,const char * MSG)
{
    if(status == CONNECTION_MSG_OK)
    {
        char msg_format[TCP_LIMIT] = {0,};
        strcpy(msg_format,MSG);
        if(msg == NULL) return;
        strcat(msg_format,res);
        strcpy(msg,msg_format);
    }
    else strcpy(msg,"BYE\n");
}

int tcp_communication(int port)
{

    int family = AF_INET;
    int type = SOCK_STREAM;
    int welcome_socket = socket(family, type, 0); if (welcome_socket <= 0)
    {
        perror("ERROR: socket");
        exit(EXIT_FAILURE); 
    }
    int enable = 1;
    setsockopt(welcome_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));

    unsigned short server_port = port; 
    struct sockaddr_in server_addr; 
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET; 
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
    server_addr.sin_port = htons(port);

    struct sockaddr *address = (struct sockaddr *) &server_addr; int address_size = sizeof(server_addr);
    if (bind(welcome_socket, address, address_size) < 0) {
        perror("ERROR: bind");
        exit(EXIT_FAILURE);
    }

    int max_waiting_connections = 1;
    if (listen(welcome_socket, max_waiting_connections) < 0) {
        perror("ERROR: listen");
        exit(EXIT_FAILURE);
    }

    struct sockaddr *comm_addr;
    socklen_t comm_addr_size;

    char request_buffer[TCP_LIMIT];
    char * p_req_buffer = request_buffer;
    char response_buffer[TCP_LIMIT]; 


    bool hello_sent = false;
    while(true) 
    {
        memset(p_req_buffer,0,TCP_LIMIT);
        memset(response_buffer,0,TCP_LIMIT);
        int comm_socket = accept(welcome_socket,comm_addr, &comm_addr_size);
        if (comm_socket > 0)
        {
            char res[TCP_LIMIT];
            int flags = 0;
            int bytes_rx = recv(comm_socket, p_req_buffer, TCP_LIMIT, flags); 
            if (bytes_rx <= 0) {
                perror("ERROR: recv");
                exit(EXIT_FAILURE);
            }

            printf("req_buff: %s\n",p_req_buffer);
            bool req_is_ok = false;
            if(!hello_sent)
            {
                hello_sent = true;
                req_is_ok = tcp_verify_begin(p_req_buffer);
                tcp_setup_msg(response_buffer,"",req_is_ok,"HELLO\n");
            }
            else
            {
                printf("COMPUTING...\n");
                req_is_ok = tcp_verify_req(p_req_buffer);
                frac_t result = get_result(&p_req_buffer);
                frac_to_string(result,res);
                tcp_setup_msg(response_buffer,res, result.denominator == 0 && req_is_ok,"RESULT ");
            }

            memset(p_req_buffer,0,TCP_LIMIT);
            printf("res_buff: %s\n",response_buffer);
            int bytes_tx = send(comm_socket, response_buffer, TCP_LIMIT, flags); 
            if (bytes_tx <= 0) {
                perror("ERROR: send");
                exit(EXIT_FAILURE);
            }
            memset(response_buffer,0,TCP_LIMIT);
        }
        printf("SHUTDOWN\n");
        shutdown(comm_socket, SHUT_RDWR);
        close(comm_socket);
    }

    return SUCCESS;
}
