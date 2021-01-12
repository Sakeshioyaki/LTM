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
#include "user.c"

#define MAX_CLIENTS 100
#define BUFFER_SZ 2048
#define NAME_LEN 32

int client_count = 0;  // dem so client hien tai dang online
int id = 0 ;

typedef struct
{
    int id;
    /* Luu tru id cua nguoi ma nguoi nay dang chat cung, or choi game cung
    *  su dung bien status de xac dinh xem 2 nguoi nay dang chat hay choi game
    * Neu status = 0 ; Nguoi nay dang thuc hien nhung tac vu khac
    */
    int fdId;
    int sockfd;
    /*
    *Luu tru diem cua nguoi choi nay khi ho choi game
    */
    int score;
    CODE status;
    char name[MAXLINE];
} Client_t;

//

Client_t *clients[MAX_CLIENTS];

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

void add_client(Client_t *cl)
{
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i] == NULL)
        {
            clients[i] = cl;
            client_count++;
            break;
        }
    }
}

void remove_client(char name[MAXLINE])
{
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i] != NULL && strcmp(clients[i]->name, name)==0)
        {
            clients[i] = NULL;
            client_count--;
            break;
        }
    }
}

void send_message(char *msg, Client_t *from)
{
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i] != NULL && clients[i]->id == from->fdId)
        {
            int wrt_status = write(clients[i]->sockfd, msg, strlen(msg));
            if (wrt_status < 0)
            {
                printf("ERROR: write to descriptor failed!\n");
                break;
            }
        }
    }
}

int isOnline(char name[MAXLINE]){
	int i;
	for(i=0;i<MAX_CLIENTS;i++){
		if(clients[i]==NULL){
			break;
		} 
		if(strcmp(clients[i]->name, name)==0){
			return clients[i]->fdId;
		}
	}
    return 0;
}

void printListUserOnline(){
    int i;
    printf("-----------List user online------\n");
    for(i=0;i<client_count;i++){
        printf("%d : %s \n", i, clients[i]->name);
    }
    printf("---------------------------------\n");
}

// void WriteToFile(char from[MAXLINE], char to[MAXLINE], char buffer[BUFFER_SZ]){
//     char fileName[MAXLINE];
//     strcpy(fileName,from);
//     strcat(fileName,"To");
//     strcat(fileName,to);
//     FILE *fb = fopen(fileName "a+");
//     fprintf(fb, "%s", buffer);
//     fclose(fb);
// }