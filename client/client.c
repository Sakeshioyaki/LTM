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

int main(int argc, char const *argv[]){
	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[1024];
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
	char userNameLogIn[50];
	int select;


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
				getchar();
				printf("User Name : ");
				scanf("%s",userNameLogIn);
				printf("da send %s\n", userNameLogIn);
				SEND(clientSocket, userNameLogIn, LOG_USERNAME);
				RECEVE(clientSocket);

				printf("%s\n", );
				break;
			case 2 :
				printf("\n");
				break;
			default:
				printf("Vui long nhap dau vao hop le !\n");
				goto: Layout1;
				break;
		}

		Layout2:
		printf("---------------------------------------------\n");
		printf("WELLCOME TO 'CHILL WITH YOU'\n");
		if(myUser != NULL){
			printf(" =>> Xin chao %s !\n",myUser->name);
		}
		printf("---------------------------------------------\n");		printf("1: Login\n");
		printf("2: Sign up\n");
		printf("Nhap vao select : \n");
		scanf("%d",&select);
		printf("%d\n",select );
					
	}//end while(1)
	close(clientSocket);
	return 0;
}