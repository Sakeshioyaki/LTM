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
void gpds(List *l){
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
int tachchuoi(char *passmoi,char *number,char *word){
  int j=0,k=0;
  for(int i=0;i<strlen(passmoi);i++){
    char ch=passmoi[i];
    if(ch=='\0') break;
    if(ch >='0'&& ch <='9'){
      number[j]=ch;
      j++;
    }
    else if((ch >= 'a' && ch <= 'z')||(ch == ' ') || (ch>='A' && ch <='Z')){
      word[k]=ch;
      k++;
    }
    else 
      return 0;
  }
  word[k]='\0';
  number[j]='\0';
  return 1;
}



// profile* login(){
// 	char nameUser[50];
// 	char passwordUser[10];
// 	int countLogin = 0;
// 	printf("Ten dang nhap : \n");
// 	scanf("%s",nameUser);
// 	users* user =  searchUser(nameUser);
// 	if(user == NULL){
// 		printf("Khong tim thay nguoi dung !\n");
// 		return NULL;
// 	}
// 	do { 
// 		printf("Nhap password : ");
// 		scanf("%s",passwordUser);
// 		if(strcmp(passwordUser,user->acc.password) == 0){
// 			printf("Dang nhap thanh cong \n");
// 			return &user->acc;
// 		}else{
// 			countLogin++;
// 			printf("Password khong dung !\n");
// 			if(countLogin == 3){
// 				printf("Tai khoan tam thoi bi khoa do nhap mat khau sai qua 3 lan !\n");
// 				user->acc.status = 0;
// 				return NULL;
// 			}
// 		}
// 	}while(countLogin < 3);
// 	return NULL;
// }

// void WriteToFile(profile newAccount){
// 	FILE *fb = fopen(fileName, "a+");
// 	//fprintf(fb, "\0");
// 	fprintf(fb, "%s\t%s\t%d\n", newAccount.name, newAccount.password, newAccount.status);
// 	fclose(fb);
// }

// void Register(){
// 	char myPassword[10];
// 	char myName[50];
// 	profile newAccount;
// 	int tmp;
// 	printf("Nhap ten dang nhap : \n");
// 	scanf("%s",myName);
// 	if(searchUser(myName) != NULL){
// 		printf("Ten nay da duoc su dung !\n");
// 	}else{
// 		printf("Nhap password (toi da 5 ki tu):\n");
// 		scanf("%s",myPassword);
// 		strcpy(newAccount.name,myName);
// 		strcpy(newAccount.password,myPassword);
// 		newAccount.status = 3;
// 		WriteToFile(newAccount);
// 		addUser(newAccount);
// 		printf("Tao tai khoan thanh cong ! Vui long dang nhap lai. \n");
// 		}
// }
void kichhoatstatus(List l,account acc, char namestatus[30],int value){
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
	
		
		// printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
		
		bd:while(1){	// MESSAGE mess=RECEVE(newSocket,buffer);
		recv(newSocket,yc,1024,0);
		if(strcmp(yc,"LOG_USERNAME")==0){
			
			if(hu!=0){
				send(newSocket,"tai khoan da dang nhap",strlen("tai khoan da dang nhap")+1,0);
				goto bd;
			}
			send(newSocket,"nhap username",strlen("nhap username")+1,0);
			int statuuser=0;
	        int statupass=0;
				while(statuuser==0){
				recv(newSocket,buffer, 1024, 0);
				if(strcmp(buffer, ":exit") == 0){
					printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
					break;
				}else{

					printf("Client: %s\n", buffer);
					int k=kiemtratontai(l,buffer);
					if(k==1){
						send(newSocket,"NOT OK",strlen("NOT OK")+1,0);
					}                                
					else{
						statuuser=1;
						send(newSocket,"nhap pass",strlen("nhap pass")+1,0);
								
						while(statupass==0){
						recv(newSocket,u, 1024, 0);
						printf("%s\n",u );
						int m=kiemtratkhople(l,buffer,u);
						 if(m==1){
							send(newSocket,"NOT OK",strlen("NOT OK")+1,0);
							
						}
						else{
							strcpy(namesignin,buffer);
							kichhoatstatus(l,acc,buffer,1);
							hu=1;
							send(newSocket,"dang nhap thanh cong",strlen("dang nhap thanh cong")+1,0);

							statupass=1;
						}	
					}

					}
				}
			
			
			}
		}// close yc
		else if(strcmp(yc,"SIGN_UP")==0){
			char name[30],pass[30],passlai[30];char press[5];
			int k=0;
			
			y: send(newSocket,"nhap useneame:",strlen("nhap useneame:")+1,0);
			recv(newSocket,name,30,0);
			int t=kiemtratontai(l,name);
			if(t==0){
				send(newSocket,"tai khoan da ton tai (press y de tiep tuc, q de out",strlen("tai khoan da ton tai (press y de tiep tuc, q de out")+1,0);
				recv(newSocket,press,5,0);
				if(strcmp(press,"y")==0){
					goto y;
				}
				else{
					goto bd;
				}
			}
			send(newSocket,"nhap password:",strlen("nhap password:")+1,0);
			recv(newSocket,pass,30,0);
			send(newSocket,"nhap laij password:",strlen("nhap laij password:")+1,0);
			while(k==0){
			recv(newSocket,passlai,30,0);
			if(strcmp(pass,passlai)==0){
				k=1;
				strcpy(acc.name,name);
				strcpy(acc.password,pass);
				acc.status=1;
				Node *p=getNode(acc);
				themdulieu(&l,p);
				ghilaivaofile(&l);
				send(newSocket,"sign up success",strlen("sign up success")+1,0);
			}else{
				send(newSocket,"sign up that bai",strlen("sign up that bai")+1,0);
			}
		  }
			
		}
		else if(strcmp(yc,"SIGN_OUT")==0){
			char uname[100];
			char bye[20]="bye ";
			send(newSocket,"Nhap usename :",strlen("Nhap usename :")+1,0);
			recv(newSocket,uname,100,0);
			
			if(strcmp(uname,namesignin)!=0){
				send(newSocket,"Accoutn not signin",strlen("Accoutn not signin")+1,0);
			}

			else{
				strcpy(namesignin," ");
				hu=0;
				kichhoatstatus(l,acc,uname,0);
				strcat(bye,uname);
				send(newSocket,bye,strlen(bye),0);
			}

		}
	}

		
	}

}
gpds(&l);
	close(newSocket);


	return 0;
	
}

