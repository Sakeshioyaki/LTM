#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>

#define MAX_CLIENTS 100
#define BUFFER_SZ 2048
#define NAME_LEN 32

void str_overwrite_stdout()
{
    printf("\r%s", "> ");
    fflush(stdout);
}

void str_trim_lf(char *arr, int len)
{
    for (int i = 0; i < len; i++)
    {
        if (arr[i] == '\n')
        {
            arr[i] = '\0';
            break;
        }
    }
}

void *recv_msg_handler(void *newSocket)
{
    int sockfd = *((int*)newSocket);
    char message[BUFFER_SZ] = "";
    while (1)
    {
        int receive = recv(sockfd, message, BUFFER_SZ, 0);
        if (receive > 0)
        {
            printf("%s", message);
            str_overwrite_stdout();
        }
        else if (receive == 0)
        {
            break;
        }
        bzero(message, BUFFER_SZ);
    }
}

void *send_msg_handler(void *newSocket)
{
    char buffer[BUFFER_SZ] = "";
    char message[BUFFER_SZ + NAME_LEN] = "";
    int sockfd = *((int*)newSocket);

    while (1) 
    {
        str_overwrite_stdout();
        fgets(buffer, BUFFER_SZ, stdin);
        str_trim_lf(buffer, BUFFER_SZ);

        if (strcmp(buffer, "exit") == 0)
        {
            break;
        }
        else 
        {
            sprintf(message, "yourfriend: %s\n", buffer);
            send(sockfd, message, strlen(message), 0);
        }
        bzero(buffer, BUFFER_SZ);
        bzero(message, BUFFER_SZ + NAME_LEN);
    }
}