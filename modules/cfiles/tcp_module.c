#include "../headers/tcp_module.h"
#include "../headers/calculator.h"

void tcp_interrupt_handler(int noop)
{
    shutdown(*g_socket, SHUT_RDWR);
    close(*g_socket);
    exit(SUCCESS);
}

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
        strcat(msg,"\n");
    }
    else strcpy(msg,"BYE\n");
}

void tcp_update_fds(struct pollfd * fds,int * number_of_fds)
{
    for (int idx = 0; idx < *number_of_fds; idx++)
    {
        if(fds[idx].fd == -1)
        {
            //move it to the left
            for(int move = idx; move < *number_of_fds; move++)
            {
                fds[move].fd = fds[move + 1].fd;
            }
            //bcs we moved elements, so we did not check the one on idx
            idx -= 1;
            *number_of_fds -= 1;
        }
    }
}

int tcp_communication(int port)
{
    // set up
    int family = AF_INET;
    int type = SOCK_STREAM;
    int server_socket = socket(family, type, 0); 
    g_socket = &server_socket;
    if (server_socket <= 0)
    {
        perror("ERROR: socket");
        exit(EXIT_FAILURE); 
    }
    int enable = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));
    int flags = fcntl(server_socket, F_SETFL, 0);
    int rc = fcntl(server_socket, F_SETFL, flags | O_NONBLOCK);

    //bind FUNCTION
    unsigned short server_port = port; 
    struct sockaddr_in server_addr; 
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET; 
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
    server_addr.sin_port = htons(port);

    struct sockaddr *address = (struct sockaddr *) &server_addr; int address_size = sizeof(server_addr);
    if (bind(server_socket, address, address_size) < 0) {
        perror("ERROR: bind");
        exit(EXIT_FAILURE);
    }

    int nfds = SERVER_CLIENT_LIMIT;
    struct pollfd *polled_fds;
    polled_fds = calloc(nfds, sizeof(struct pollfd)); 

    int max_waiting_connections = SERVER_CLIENT_LIMIT;
    if (listen(server_socket, max_waiting_connections) < 0) {
        perror("ERROR: listen");
        exit(EXIT_FAILURE);
    }

    int updated_num_fds = 0;
    int ready_socket;
    int new_socket = -1;
    bool said_hi[SERVER_CLIENT_LIMIT] = {false,};
    bool fds_closed[SERVER_CLIENT_LIMIT] = {false,};
    bool close_all = false;

    polled_fds[0].fd = server_socket;
    polled_fds[0].events = POLLIN;

    char request_buffer[TCP_LIMIT];
    char * p_req_buffer = request_buffer;
    char response_buffer[TCP_LIMIT]; 

    while(true)
    {
        ready_socket = poll(polled_fds, nfds, -1);
        if (ready_socket < 0)
        {
            perror("ERROR: poll() failed");
            break;
        }
        if (ready_socket == 0) break;

        updated_num_fds = nfds;
        for(int idx = 0; idx < updated_num_fds; idx++)
        {
            if(polled_fds[idx].revents == 0) continue;

            if (polled_fds[idx].fd == server_socket)
            {
                    new_socket = accept(server_socket, NULL, NULL);
                    if (new_socket < 0)
                    {
                        perror("ERROR: accept");
                        fds_closed[idx] = true;
                    }
                    polled_fds[nfds].fd = new_socket;
                    polled_fds[nfds].events = POLLIN;
                    nfds += 1;
            }
            else
            {
                fds_closed[idx] = false;

                char res[TCP_LIMIT];
                int flags = 0;

                memset(p_req_buffer,0,TCP_LIMIT);
                memset(response_buffer,0,TCP_LIMIT);

                ready_socket = recv(polled_fds[idx].fd, p_req_buffer, TCP_LIMIT, flags);
                if(ready_socket < 0)
                {
                    perror("ERROR: recv");
                    fds_closed[idx] = true;
                }
                if(!strcmp(p_req_buffer,"BYE\n") || ready_socket == 0) fds_closed[idx] = true;

                bool req_is_ok = false;
                if(!said_hi[idx])
                {
                    said_hi[idx] = true;
                    req_is_ok = tcp_verify_begin(p_req_buffer);
                    tcp_setup_msg(response_buffer,"",req_is_ok,"HELLO");
                }
                else
                {
                    req_is_ok = tcp_verify_req(p_req_buffer);
                    p_req_buffer += (int)strlen("SOLVE ");
                    frac_t result = get_result(&(p_req_buffer));
                    frac_to_string_floored(result,res);
                    tcp_setup_msg(response_buffer,res, result.denominator == 0 || req_is_ok,"RESULT ");
                }
                memset(p_req_buffer,0,TCP_LIMIT);

                ready_socket = send(polled_fds[idx].fd, response_buffer, strlen(response_buffer), flags); 
                if (ready_socket <= 0) {
                    perror("ERROR: send");
                    fds_closed[idx] = true;
                }
                memset(response_buffer,0,TCP_LIMIT);

                if(fds_closed[idx])
                {
                    close(polled_fds[idx].fd);
                    // remove descriptor
                    polled_fds[idx].fd = -1;
                    fds_closed[idx] = false;
                    said_hi[idx] = false;
                    tcp_update_fds(polled_fds,&nfds);
                }


            }
            if(close_all) 
            {
                for(int idx = 0; idx < updated_num_fds; idx++)
                {
                    if(polled_fds[idx].fd >= 0) close(polled_fds[idx].fd);
                }
                break;
            }
        }
    }
    shutdown(server_socket,SHUT_RDWR);
    close(server_socket);
    return SUCCESS;
}
