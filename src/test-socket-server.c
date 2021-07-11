#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "../include/boolean.h"
#include "../include/posix_exitcodes.h"

#define BIND_ADDRESS                                        "0.0.0.0"
#define SOCKET_BACKLOG                                      5
#define SERVER_PORT                                         8080
int main()    
{
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t socklen;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket == EGENERAL)
    {
        printf("ERROR: socket() => %s!\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(SERVER_PORT);
    if(bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)))
    {
        printf("ERROR: bind() => %s!\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if(listen(server_socket, SOCKET_BACKLOG))
    {
        printf("ERROR: listen() => %s!\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    while(TRUE)
    {
        printf("WAITING FOR CLIENT CONNECTIONS ...\n");
        socklen = sizeof(client_address);
        client_socket = accept(server_socket, (struct sockaddr *) &client_address, &socklen);
        if(client_socket == EGENERAL)
        {
            printf("ERROR: accept() => %s!\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        else
        {
            printf("CLIENT CONNECTED ID [%d] ADDRESS [%s] PORT [%d]\n", client_socket, inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
            
            close(client_socket);
        }
    }
}
