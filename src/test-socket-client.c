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

#define SERVER_ADDRESS                                      "127.0.0.1"
#define SERVER_PORT                                         8080
#define COMMAND_SIZE                                        20
int main()    
{
    int client_socket;
    struct sockaddr_in server_address;
    char command[COMMAND_SIZE + 1] = "";
    int bytes_sent, bytes_sent_total, message_size;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(client_socket == EGENERAL)
    {
        printf("ERROR: socket() => %s!\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_ADDRESS, &server_address.sin_addr);
    server_address.sin_port = htons(SERVER_PORT);
    if(connect(client_socket, (struct sockaddr *) &server_address, sizeof(server_address)))
    {
        printf("ERROR: connect() => %s!\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Connected to server [%s] port [%d]!\n", SERVER_ADDRESS, SERVER_PORT);
        while(strcmp(command, "exit"))
        {
            scanf("shell => %s", command);
            bytes_sent_total = 0;
            message_size = strlen(command);
            while(bytes_sent_total < message_size)
            {
                bytes_sent = send(client_socket, command + bytes_sent_total, message_size - bytes_sent_total, 0);
                if(bytes_sent == EGENERAL)
                {
                    printf("ERROR: send() => %s!\n", strerror(errno));
                    strcpy(command, "exit");
                }
                else
                {
                    bytes_sent_total += bytes_sent;
                }
            }
        }
        close(client_socket);
    }
}
