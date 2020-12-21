#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "lib.c"
#define SERV_PORT 3000
#define MAXLINE 100

#define fileName "users.txt"
#define MAX 50 //max friend

typedef struct accout{
  char name[MAXLINE];
  char password[MAXLINE];
  int status;
}account;
typedef struct Node
{
  account acc;
  struct Node *next;
}Node;
typedef struct List{
  Node *head;
  Node *tail;
}List;
void khoitao(List *l){
  (*l).head=(*l).tail=NULL;
}
Node *getNode(account acc){
  Node *p;
  p=(Node*)malloc(sizeof(Node));
     if(p==NULL){
        printf("failed\n");
    }
    else{
        strcpy(p->acc.name,acc.name);
        strcpy(p->acc.password,acc.password);
        p->acc.status=acc.status;
        p->next=NULL;
    }
  return p;
}
void themdulieu(List *l,Node *p){
  if(l->head==NULL){
    l->head=p;
    l->tail=p;
  }
  else{
    l->tail->next=p;
    l->tail=p;
  }
}
void rê(List *l){
    for(Node *k=(*l).head;k!=NULL;k=k->next){
        (*l).head=(*l).head->next;
        free(k);
    }
}
int kiemtratontai(List l, char name[MAXLINE]){
  for(Node *k=l.head;k!=NULL;k=k->next){
            if(strcmp(k->acc.name,name)==0){
              return 0;
            }
        }
          return 1;
}
void ghilaivaofile(List *l){
    FILE *f;
    f=fopen("account.txt","w");
    for(Node *k=(*l).head;k!=NULL;k=k->next){
        fprintf(f, "%s %s %d\n",k->acc.name,k->acc.password,k->acc.status );
    }
    fclose(f);
}
void xuatds(List l){
    Node *k=NULL;
    for (k=l.head;k!=NULL;k=k->next){
      printf("%20s%20s%5d", k->acc.name,k->acc.password,k->acc.status);
      printf("\n");
      
    }
    printf("\n");
  }
int kiemtratkhople(List l, char name[MAXLINE],char password[MAXLINE]){
    int h=kiemtratontai(l,name);
    if(h==0){
        for (Node *k=l.head;k!=NULL;k=k->next){
            if(strcmp(k->acc.name,name)==0){
                if(strcmp(k->acc.password,password)==0){
                    return 0;
                   
                }
                else{
                      return 1;
                }
              
            }
        }
    }
    
}

void setStatus(List l,account acc, char namestatus[30],int value){
	for(Node *k=l.head;k!=NULL;k=k->next){
					if(strcmp(k->acc.name,namestatus)==0){
						k->acc.status=value;
						break;
					}
				}
				ghilaivaofile(&l);
}

int main(int argc, char*argv[]){

	if(argc != 2){
		printf("Thieu or thua du lieu ! \n");
		return 0;
	}
	FILE *f;
	account acc;
	List l;
	khoitao(&l);
	f=fopen("account.txt","r");
    if(f==NULL){
        printf("file khong ton tai \n");
    }
    while(!feof(f)){
        
        if(fscanf(f, "%s %s %d", acc.name,acc.password,&acc.status)!=EOF){

        Node *p=getNode(acc);
        themdulieu(&l,p);
    	}
	}
fclose(f);
	

	int PORT = atoi(argv[1]);
	if(PORT == 0){
		printf("Du lieu  nhap vao khong dung !\n");
		return 0;
	}
	
	char u2[10];
	char u[20];
	char yc[1024];
	char namesignin[100]=" ";
	int sockfd, ret;
	struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[1024];
	pid_t childpid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Server Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(atoi(argv[1]));
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in binding.\n");
		exit(1);
	}
	printf("[+]Bind to port %d\n", 4444);

	if(listen(sockfd, 10) == 0){
		printf("[+]Listening....\n");
	}else{
		printf("[-]Error in binding.\n");
	}
	// addr_size=sizeof(newAddr);
	while(1){
	newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			exit(1);
		}
	printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
	int hu=0;
	if((childpid=fork())==0){
		close(sockfd);
		
	}

}
gpds(&l);
	close(newSocket);


	return 0;
	
}

