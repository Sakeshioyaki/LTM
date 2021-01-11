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

void add_client(Client_t *cl);
void Writeuseronline(Client_t *cl);
//
int readFileonline(){
  client_count=0;
  FILE *f;
  f=fopen("useronline.txt","r");
  Client_t *newcl;
  while(fscanf(f,"%s %d",newcl->name,&newcl->sockfd)!=EOF){
    add_client(newcl);
    client_count++;
  }
  return client_count;
  fclose(f);
}
Client_t *clients[MAX_CLIENTS];
void add_client(Client_t *cl)
{
   int client_count=readFileonline();
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i] == NULL)
        {
            clients[i] = cl;
            client_count++;
            printf("49. client_count la %d\n",client_count );
            Writeuseronline(cl);
            break;
        }
    }
}
void Writeuseronline(Client_t *cl){
  FILE *f;
  f=fopen("useronline.txt","a+");
  fprintf(f, "%s %d\n",cl->name,cl->sockfd );
  fclose(f);
}



void removeUserOnline(char name[100]){
  FILE *f;
  char nameonl[100];
  int so;
  f=fopen("useronline.txt","a+");
  while(fscanf(f,"%s %d",nameonl,&so)!=EOF){
    if(strcmp(nameonl, name)==0){
      // printf("name remove la %s\n",nameonl );
      // strcpy(nameonl,"haha");
      // so=0;
      // strcpy(fprintf(f, "%s %d\n",nameonl,so ),NULL);
      // fprintf(f, "%s %d\n",nameonl,so )= NULL;
      fclose(f);
      return;
    }
  }
}



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

