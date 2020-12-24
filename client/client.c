#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "lib.c"
#define PORT 8080
#define MAXLINE 100
// Layout1;

typedef struct accout{
	char name[MAXLINE];
	int status;
}account;


void loginUser(MESSAGE mess, int clientSocket,int statususer,int statuspass){
	char userNameLogIn[50];
	char pass[20];
	while(statususer==0){
				getchar();
				printf("User Name : ");
				scanf("%s",userNameLogIn);
				printf("da send %s\n", userNameLogIn);
				SEND(clientSocket, userNameLogIn, LOG_USERNAME);
				mess = RECEVE(clientSocket);
				printf("=>sever :%s\n", mess.mess);
				if(strcmp(mess.mess,"OK")==0){
					statususer=1;
					while(statuspass==0){
						printf("User pass:  ");
						scanf("%s",pass);
						SEND(clientSocket,pass,LOG_PASSWORD);
						mess=RECEVE(clientSocket);
						printf("server: %s\n", mess.mess);
						if(strcmp(mess.mess,"login success")==0){
							statuspass=1;
							
						}
					}
					
				}
			}
}
int main(int argc, char const *argv[]){
	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[1024];
	
	int statususer=0;
			int statuspass=0;
	char namesignin[100];
	account* myUser = NULL;
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Client Socket is created.\n");

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Connected to Server.\n");
	int select;
	MESSAGE mess;


	while(1){
		printf("---------------------------------------------\n");
		printf("WELLCOME TO 'CHILL WITH YOU'\n");
		printf("---------------------------------------------\n");
//Layout 1 : Login and sign up
		Layout1:
		printf("Login or sign up\n");
		printf("1: Login\n");
		printf("2: Sign up\n");
		printf("Nhap vao select : \n");
		scanf("%d",&select);
		printf("%d\n",select );
		// if(scanf("%d",&select) == 0 ){
		// 	select = 0;
		// }
		switch(select){
			case 1 :
			
			loginUser(mess,clientSocket,statususer,statuspass);
			goto Layout1;	
				break;
			case 2 :

				break;
			default:
				printf("Vui long nhap dau vao hop le !\n");
				goto Layout1;
				break;
		}

		Layout2:
		printf("---------------------------------------------\n");
		printf("WELLCOME TO 'CHILL WITH YOU'\n");
		if(myUser != NULL){
			printf(" =>> Xin chao %s !\n",myUser->name);
		}

					
	}//end while(1)
	close(clientSocket);
	return 0;
}