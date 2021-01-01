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
#define NOTOK "NOT OK"
#define OK "OK"

//status = 1 : dang onine
typedef struct accout{
	char name[MAXLINE];
	int status;
}account;


account loginUser(int clientSocket){
	char userNameLogIn[50];
	char pass[20];
	MESSAGE mess;
	account user;
	int statuspass =0;
	int statususer = 0;
	getchar();

	while(statususer==0){
		fflush(stdout);
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
					strcpy(user.name,userNameLogIn);
					user.status = 1;
					
				}
			}
		}
	}
	return user;
}

account sigUp(int clientSocket){
	char name[MAXLINE];
	char password[MAXLINE];
	MESSAGE mess;
	account user;
	int check = 1;
	do{
		printf("Nhap ten dang nhap : \n");
		scanf("%s", name);
		strcpy(user.name,name);
		SEND(clientSocket, name, SIGN_UP_USERNAME);
		mess = RECEVE(clientSocket);
		printf("sever : %s\n",mess.mess );
		if(strcmp(mess.mess, NOTOK) == 0){
			printf("Ten tai khoan da ton tai ! Vui long chon ten khac\n");
			check = 0;
			strcpy(user.name,"\0");
		}else{
			printf("user name %s %s\n", user.name, name);
			printf("Nhap mat khau: \n");
			scanf("%s",password);
			SEND(clientSocket,password,SIGN_UP_PASSWORD);
			user.status = 1;
			check = 1;
		}
		
	}while(check == 0);
	return user;
}

void requestFriend(int clientSocket){
	char name[MAXLINE];
	MESSAGE mess;
	int check = 0;
	char ok[MAXLINE] = "OK";
	do{
		printf("Nhap ten nguoi ban muon ket ban : ");
		scanf("%s",name);
		printf(" name : %s\n", name);
		SEND(clientSocket, name,YC_KET_BAN);
		mess = RECEVE(clientSocket);
		if(strcmp(mess.mess, NOTOK) == 0){
			printf("Khong ton tai nguoi dung trong he thong !\n");
			printf("Vui long nhap ten hop le\n");
			printf("<<Nhan 1 de ket thuc / 0 de tiep tuc>>\n");
			scanf("%d",&check);
		}else{
			printf(" mess nhan dc %s\n", mess.mess);
			if(strcmp(mess.mess, ok) == 0){
				printf("Yeu cau cua ban da duoc gui!\n");
				printf("Vui long cho ban cua ban xac nhan \n");
			}
			return;
		}
	}while(check == 0);

}



int main(int argc, char const *argv[]){
	int clientSocket, ret,select;
	struct sockaddr_in serverAddr;
	char buffer[1024];
	char tmp[MAXLINE] = "hello";
	MESSAGE mess;
	
	int statususer=0;
			int statuspass=0;
	char namesignin[100];
	account myUser;
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
				myUser = loginUser(clientSocket);
				goto Layout1;	
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
		select = 0;
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
		printf("6: Xem yeu cau ket ban\n");
		printf("7: Log Out\n");
		printf("Nhap vao select : \n");
		scanf("%d",&select);
		printf("%d\n",select );
		switch(select){
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				printf("Bat dau ket ban\n");
				requestFriend(clientSocket);
				goto Layout2;
				break;
			case 5:
				break;
			case 6:
				SEND(clientSocket,tmp,YC_XEM_BAN_BE);
				mess = RECEVE(clientSocket);
				printf("so yeu cau ket ban la %s\n",mess.mess );
				goto Layout2;
				break;
			case 7:
				myUser.status = 0;
				strcpy(myUser.name,"\0");
				break;
			default:
				printf("nhap lua chon hop le !\n");
				break;
		}
					
	}//end while(1)
	close(clientSocket);
	return 0;
}