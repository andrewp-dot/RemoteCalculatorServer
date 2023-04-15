#include "../headers/udp_module.h"
#include "../headers/calculator.h"

bool udp_verify_req(char * msg)
{
    if(msg == NULL) return false;
    if(msg[0] != 0 || msg[2] != '(') return false;
    return true;
}

void udp_interrupt_handler(int * socket)
{
    close(*socket);
}

int udp_communication(int port)
{
    // char * arr = NULL;
    int server_socket = socket(AF_INET, SOCK_DGRAM, 0); if (server_socket <= 0)
    {
        perror("ERROR: socket");
        exit(EXIT_FAILURE); 
    }

    unsigned short server_port = port; 
    struct sockaddr_in server_addr; 
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET; 
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
    server_addr.sin_port = htons(port);

    struct sockaddr *address = (struct sockaddr *) &server_addr; 
    int address_size = sizeof(server_addr);
    if (bind(server_socket, address, address_size) < 0) {
        perror("ERROR: bind");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in client_address;
    socklen_t cl_address_size = sizeof(client_address);
    struct sockaddr * cl_address =  (struct sockaddr *) &client_address;

    char buffer[UDP_LIMIT];
    char response_buffer[UDP_LIMIT];
    char * p_buffer = buffer;

    while(true) 
    {
        memset(buffer,0,UDP_LIMIT*sizeof(char));
        memset(response_buffer,0,UDP_LIMIT*sizeof(char));
        int bytes_rx = recvfrom(server_socket, p_buffer, UDP_LIMIT, 0, cl_address, &cl_address_size);
        if (bytes_rx < 0) 
        {
            perror("ERROR: recvfrom");
        }

        char res[100];
        frac_t result = ERR_FRAC;
        if(udp_verify_req(p_buffer)) 
        {
            p_buffer += UDP_REQUEST_OFFSET;
            result = get_result(&p_buffer);
            frac_to_string(result,res);
        }
        else  p_buffer += UDP_REQUEST_OFFSET;

        memset(buffer,0,UDP_LIMIT*sizeof(char));
        
        // printf("res: %s\n",res);

        response_buffer[0] = 1;
        response_buffer[1] = (result.denominator == 0);
        if(response_buffer[1]) strcpy(res,"Unexpected token.");
        response_buffer[2] = strlen(res);
        strcpy(response_buffer + UDP_RESPONSE_OFFSET,res);

        printf("0:%d| 1: %d| 2: %d| 3:%s\n",response_buffer[0],response_buffer[1],response_buffer[2],response_buffer + UDP_RESPONSE_OFFSET);
        int bytes_tx = sendto(server_socket, response_buffer, response_buffer[2] + UDP_RESPONSE_OFFSET, 0, cl_address, cl_address_size);
        if (bytes_tx < 0) 
        {
            perror("ERROR: sendto");
        } 
        memset(response_buffer,0,UDP_LIMIT*sizeof(char));
    }

    return SUCCESS;
}
