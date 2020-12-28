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


int loginUser(MESSAGE mess, int clientSocket,int statususer,int statuspass){
	char userNameLogIn[50];
	char pass[20];
<<<<<<< Updated upstream
	int statususer=0;
	int statuspass=0;
	MESSAGE mess;
	account user;
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
					strcpy(user.name, userNameLogIn);
					user.status = 1;
				}
			}	
		}	
	}
	return user;
=======
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
							return statuspass;
						}
					}
					
				}
			}
>>>>>>> Stashed changes
}

account sigUp(int clientSocket){
	char name[MAXLINE];
	char password[MAXLINE];
	MESSAGE mess;
	account user;
	printf("Nhap ten dang nhap : \n");
	scanf("%s", name);
	strcpy(user.name,name);
	SEND(clientSocket, name, SIGN_UP_USERNAME);
	mess = RECEVE(clientSocket);
	printf("sever : %s\n",mess.mess );
	printf("Nhap mat khau: \n");
	scanf("%s",password);
	SEND(clientSocket,password,SIGN_UP_PASSWORD);
	user.status = 1;
	return user;
}


int main(int argc, char const *argv[]){
	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[1024];
	
	char namesignin[100];
	account  myUser;
	//chua dang nhap
	myUser.status = 0;
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
	char tmp[10] = "hello";
	char userName[MAXLINE];
	int statususer=0;
	int statuspass=0;


	while(1){
		printf("---------------------------------------------\n");
		printf("WELLCOME TO 'CHILL WITH YOU'\n");
		printf("---------------------------------------------\n");
		Layout1:
		printf("Login or sign up\n");
		printf("1: Login\n");
		printf("2: Sign up\n");
		printf("Nhap vao select : \n");
		scanf("%d",&select);
		printf("%d\n",select );
		switch(select){
			case 1 :
				SEND(clientSocket,tmp,LOG_USERNAME);
				statuspass= loginUser(mess,clientSocket,statususer,statuspass);
				printf("gia trij statuspass laf %d\n", statuspass);
				if(statuspass==1){
					goto Layout2;
				}
				break;
			case 2 :
				myUser = sigUp(clientSocket);
				if(myUser.status != 0){
					printf("Da tao tai khoan thanh cong ! Ban dang dang nhap \n");
				}else{
					printf("Loi khi tao tai khoan\n");
				}
				break;
			default:
				printf("Vui long nhap dau vao hop le !\n");
				goto Layout1;
				break;
		}
		Layout2:
		printf("---------------------------------------------\n");
		printf("WELLCOME TO 'CHILL WITH YOU'\n");
		if(myUser.status == 1){
			printf(" =>> Xin chao %s !\n",myUser.name);
		}
		printf("---------------------------------------------\n");		
		printf("1: Choi game voi may\n");
		printf("2: Choi game voi ban\n");
		printf("3: Chat voi ban\n");
		printf("4: Ket ban\n");
		printf("5: Xem danh sach ban be\n");
		printf("6: Log Out\n");
		printf("Nhap vao select : \n");
		scanf("%d",&select);
		printf("%d\n",select );
					
	}//end while(1)
	close(clientSocket);
	return 0;
}