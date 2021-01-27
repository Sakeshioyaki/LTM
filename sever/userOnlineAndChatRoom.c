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

typedef struct room{
    Client_t user1;
    Client_t user2;
}room;

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
    // pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i] == NULL)
        {
            clients[i] = cl;
            client_count++;
            break;
        }
    }
    // pthread_mutex_unlock(&clients_mutex);
}

void remove_client(int uid)
{
    // pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i] != NULL && clients[i]->id == uid)
        {
            clients[i] = NULL;
            break;
        }
    }
    // pthread_mutex_unlock(&clients_mutex);
}

void send_message(char *msg, Client_t *from)
{
    // pthread_mutex_lock(&clients_mutex);
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
    // pthread_mutex_unlock(&clients_mutex);
}

int isOnline(char name[MAXLINE]){
	int i;
	for(i=0;i<MAX_CLIENTS;i++){
		if(clients[i]==NULL){
			break;
		} 
		if(strcmp(clients[i]->name, name)==0){
			return clients[i]->id;
		}
	}
    return 0;
}

void printListUserOnline(){
    int i;
    printf("-----------List user online------\n");
    for(i=0;i<client_count;i++){
        printf("%d : %s - id : %d\n", clients[i]->id, clients[i]->name, clients[i]->id);
    }
    printf("---------------------------------\n");
}

int kiemTraSanSang(int idOfFriend, int myId){
    int i;
    // printf("ID cua ban kia : %d\n", idOfFriend);
    // printf("My id : %d\n", myId);
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i] != NULL && clients[i]->id == idOfFriend)
        {
            printf("My Friend : ID : %d - FDiD : %d", clients[i]->id, clients[i]->fdId);
            if(clients[i]->fdId == myId){
                // printf("usser san sang ra : 1\n");
                return 1;
                break;
            }
        }
    }
    // printf("usser san sang ra : 0\n");
    return 0;
}

void chat(Client_t *cli ){
    int leave_flag=0;
    char buffer[BUFFER_SZ];
    while (1)
    {
        if (leave_flag)
            break;

        int receive = recv(cli->sockfd, buffer, BUFFER_SZ, 0);
        // printf("162 : chuoi nhan dc la : %s\n", buffer );
        if (receive > 0)
        {
            send_message(buffer, cli);
            // printf("166: da send : %s\n",buffer );
            str_trim_lf(buffer, strlen(buffer));
            printf("%s\n", buffer);
        }
        else if (receive == 0 || (strcmp(buffer, "exit") == 0))
        {
            // printf("171: chay vo day \n");
            sprintf(buffer, "%s has left\n", cli->name);
            printf("%s", buffer);
            send_message(buffer, cli);
            // printf("175: da send : %s\n",buffer );
            leave_flag = 1;
        }
        else
        {
            printf("ERROR: -1\n");
            leave_flag = 1;
        }
        bzero(buffer, BUFFER_SZ);
    }
    cli->fdId = 0;
    return;
}

void showListFriend(userInfo *user, int newSocket){
  listFriend *tmp = user->listFd;
  char name[MAXLINE];
  char offline[MAXLINE] = "OFFLINE";
  char online[MAXLINE] = "ONLINE";
  int i;
  if(tmp == NULL){
    printf("Ban khong co nguoi ban nao\n" );
  }else{
    for(i=1;i<=countFriend;i++){
      printf("%d : %s\n",i,tmp->myFriend.name);
      // strcpy(name, tmp->myFriend.name);
      // SEND(newSocket,name, YC_XEM_DS_BAN_BE);
      if(isOnline(tmp->myFriend.name) == 0){
        sprintf(name,"%s - OFFLINE\n", tmp->myFriend.name);
        SEND(newSocket, name, YC_XEM_DS_BAN_BE);
      }else{
        sprintf(name, "%s - ONLINE\n", tmp->myFriend.name);
        SEND(newSocket, name, YC_XEM_DS_BAN_BE);
      }
      tmp = tmp->next;
      sleep(1);
    }
  }
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