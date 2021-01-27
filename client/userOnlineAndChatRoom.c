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

int flag;

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

void catch_exit()
{
    flag = 1;
}

void*  recv_msg_handler(void * sockfdd)
{
    int sockfd = *((int*)sockfdd); 
    char message[BUFFER_SZ] = "";
    while (1)
    {
        int receive = recv(sockfd, message, BUFFER_SZ, 0);
        // printf("chuoi nhan dc la : %s\n", message);
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

void* send_msg_handler(void * sockfdd)
{
    int sockfd = *((int*)sockfdd); 
    char buffer[BUFFER_SZ] = "";
    char message[BUFFER_SZ + NAME_LEN] = "";
    fflush(stdin);

    while (1) 
    {
        str_overwrite_stdout();
        fgets(buffer, BUFFER_SZ, stdin);
        str_trim_lf(buffer, BUFFER_SZ);

        if (strcmp(buffer, "exit") == 0)
        {
            // printf("76 : da send : %s\n",buffer );
            send(sockfd, buffer, strlen(buffer), 0);
            break;
        }
        else
        {
            sprintf(message, "ban cua ban: %s\n",  buffer);
            // printf("83 : da send : %s\n",message );
            send(sockfd, message, strlen(message)+1, 0);
        }
        bzero(buffer, BUFFER_SZ);
        bzero(message, BUFFER_SZ + NAME_LEN);
    }
    catch_exit();
}
