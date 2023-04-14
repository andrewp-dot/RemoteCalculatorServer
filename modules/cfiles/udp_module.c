#include "../headers/udp_module.h"
#include "../headers/exit_codes.h"
#include "../headers/calculator.h"

void udp_interrupt_handler(int noop)
{
    exit(SUCCESS);
}

int udp_communication(int port)
{
    // char * arr = NULL;
    signal(SIGINT,udp_interrupt_handler);
    signal(SIGTERM,udp_interrupt_handler);
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

        //validation
        printf("RECIEVED: %s\n",++p_buffer);    
        ++p_buffer;

        //prepare msg
        frac_t result = get_result(&p_buffer);
        print_frac(result);
        memset(buffer,0,UDP_LIMIT*sizeof(char));
        
        frac_to_string(result,buffer);
        
        response_buffer[0] = 1;
        response_buffer[1] = result.denominator == 0;
        response_buffer[2] = strlen(buffer);
        strcat(response_buffer,buffer);
        
        int bytes_tx = sendto(server_socket, response_buffer, strlen(response_buffer), 0, cl_address, cl_address_size);
        if (bytes_tx < 0) 
        {
            perror("ERROR: sendto");
        } 
        memset(response_buffer,0,UDP_LIMIT*sizeof(char));
    }


    return SUCCESS;
}