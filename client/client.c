#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "lib.c"
#include "userOnlineAndChatRoom.c"

#define PORT 8080
// #define MAXLINE 1000
#define NOTOK "NOT OK"
#define OK "OK"

//status = 1 : dang onine
typedef struct accout{
	char name[MAXLINE];
	int status;
}account;


account loginUser(int clientSocket,account user,int statususer,int statuspass ){
	MESSAGE mess;
	user.status=0;
	char userNameLogIn[50];
	char pass[20];
	while(statususer==0){
		system("clear");
		printf("User Name : ");
		scanf("%s",userNameLogIn);
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
				if(strcmp(mess.mess,"NHAP QUA SO LAN")==0){
						printf("user.name o day la %s\n", user.name);
						return user;
				}
				
				else if(strcmp(mess.mess,"login success")==0){
						statuspass=1;
						strcpy(user.name,userNameLogIn);
						user.status = 1;
						return user;
					}
			}
		}
		else if(strcmp(mess.mess,"NHAP QUA SO LAN")==0){
			return user;
		}
	}
}

account singUp(int clientSocket){
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
		if(strcmp(mess.mess,"NHAP QUA")==0){
			return user;
		}
		else if(strcmp(mess.mess, NOTOK) == 0){
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
// void requestChat(int clientSocket){
// 	char name[MAXLINE];
// 	MESSAGE mess;
// 	char messageFriend[1024];
// 	int check = 0;
// 	char ok[MAXLINE] = "OK";
// 	do{
// 		printf("Nhap ten nguoi ban muon chat : ");
// 		scanf("%s",name);
// 		printf(" name : %s\n", name);
// 		SEND(clientSocket, name,CHAT);
// 		mess = RECEVE(clientSocket);
// 		printf("mess ben client nhan duoc la %s\n",mess.mess );
// 		if(strcmp(mess.mess, NOTOK) == 0){
// 			printf("Cac ban khong phair ban be!Vui long ket ban !\n");
// 			printf("<<Nhan 1 de ket thuc / 0 de tiep tuc>>\n");
// 			scanf("%d",&check);
// 		}else{
			
// 			printf("name-> ");
// 			fgets(messageFriend,sizeof(messageFriend),stdin);
// 			SEND(clientSocket,messageFriend,CHAT);
			
// 			return;
// 		}
// 	}while(check == 0);
// }

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
		printf("mess ben client nhan duoc la %s\n",mess.mess );
		if(strcmp(mess.mess, NOTOK) == 0){
			printf("Khong ton tai nguoi dung trong he thong !\n");
			printf("Vui long nhap ten hop le\n");
			printf("<<Nhan 1 de ket thuc / 0 de tiep tuc>>\n");
			scanf("%d",&check);
		}else{
			
				printf("Yeu cau cua ban da duoc gui!\n");
				printf("Vui long cho xac nhan \n");
			
			return;
		}
	}while(check == 0);

}


int main(){
	int clientSocket, ret, select, requestFd, i, tmp1, countFriend;
	struct sockaddr_in serverAddr;
	char nameuser[30];
	char messreact[MAXLINE];
	char namegame[MAXLINE];
	char messageFriend[MAXLINE];
	char tmp[MAXLINE] = "hello";
	char logout[100]="logout";
	MESSAGE mess;
	char ok[MAXLINE] = "OK";
	char notok[MAXLINE] = "NOT OK";
	int statususer=0;
	char playgame[30]="play game";
	char u[20]="choi game";
	int check,k;
	int statuspass=0;
	char namesignin[100];
	char friendName[MAXLINE];
	char buffer[BUFFER_SZ];
	char message[BUFFER_SZ + NAME_LEN] = "";
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
		Layout1:
		printf("---------------------------------------------\n");
		printf("Login or sign up\n");
		printf("1: Login\n");
		printf("2: Sign up\n");
		printf("Nhap vao select : \n");
		scanf("%d",&select);
		printf("%d\n",select );
		switch(select){
			case 1 :
				SEND(clientSocket,tmp,LOG_USERNAME);
				mess=RECEVE(clientSocket);
				myUser = loginUser(clientSocket,myUser, statususer,statuspass);
				if(myUser.status==0){
					goto Layout1;
				}
				else{
				    goto Layout2;	
				}
				break;
			case 2 :
				myUser = singUp(clientSocket);
				if(myUser.status != 0){
					printf("Da tao tai khoan thanh cong !\n");
					goto Layout2;
				}else{
					printf("Loi khi tao tai khoan\n");
					goto Layout1;
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
		printf("7: Gui tin nhan \n");
		printf("8: Log Out\n");
		printf("Nhap vao select : \n");
		scanf("%d",&select);
		printf("%d\n",select );
		switch(select){
			case 1:
				SEND(clientSocket,playgame,PLAY_GAME_WITH_SEVER);
				char ss[10]="ok";

				char answer[10];
				
				int count=0;

				while(count<5){
					char buff[MAXLINE]="";
					int k = recv(clientSocket,buff,MAXLINE,0);
					buff[k]='\0';
					if(k<=0){
						printf("loi khi nhan %d\n",k );
						exit(1);
					}
					printf("%s\n",buff );
					
					scanf(" %[^\n]", answer);
						send(clientSocket,answer,strlen(answer),0);
						printf("da send %s\n",answer );
						// mess=RECEVE(clientSocket);
						char cho[100];
						recv(clientSocket,cho,100,0);
						printf("%s\n",cho );
						count++;
						// strcpy(buff,"");

					}
				goto Layout2;
				break;
			case 2:
				SEND(clientSocket,tmp,PLAY_GAME_WITH_FD);
				break;
				break;
			case 3:
				do{
					printf("Nhap ten nguoi ban muon chat cung : ");
					scanf("%s",friendName);
					SEND(clientSocket, friendName, CHAT);
					printf("da send : %s\n", friendName);
					mess = RECEVE(clientSocket);
					if(strcmp(mess.mess, notok) == 0){
						printf("Khong ton tai nguoi nay torng he thong");
						printf("1 :tiep tuc / 0 :dung\n");
						scanf("%d", &check);
					}
					else if(strcmp(mess.mess, "NOT FRIEND")){
						printf("Cac ban khong phai ban be !\n");
						printf("1 :tiep tuc / 0 :dung\n");
						scanf("%d", &check);					
					}
					else{
						printf("Da san sang de chat \n");
						check = 0;
					}
				}while(check == 1);
				mess = RECEVE(clientSocket);
				if(strcmp(mess.mess,notok)==0){
					printf("Ban cua ban hien offline. Vui long chuyen qua send message\n");
					break;
				}else{
					printf("Ban cua ban hien dang online!\n");
				}
				goto Layout2;
				break;
			case 4:
				printf("Bat dau ket ban\n");
				requestFriend(clientSocket);
				goto Layout2;
				break;
			case 5:
				SEND(clientSocket,tmp,YC_XEM_DS_BAN_BE);
				mess = RECEVE(clientSocket);
				countFriend = atoi(mess.mess);
				printf("Banj hien co %d ban be\n", countFriend);
				for(i=1;i<=countFriend;i++){
					mess = RECEVE(clientSocket);
					printf("%d: %s\n",i, mess.mess );
				}
				goto Layout2;
				break;
			case 6:
				SEND(clientSocket,tmp,YC_XEM_BAN_BE);
				mess = RECEVE(clientSocket);
				requestFd = atoi(mess.mess);
				printf("so yeu cau ket ban la %d\n",requestFd );
				for(i=1; i<=requestFd; i++){
					printf("254: bat dau in \n");
					mess = RECEVE(clientSocket);
					printf("256: da nhan dc la : %s\n", mess.mess);
					printf("%d: %s\n",i, mess.mess );
					printf("1: dong y 0: tu choi\n");
					scanf("%d",&tmp1);
					if(tmp1 == 1){
						SEND(clientSocket, ok, YC_XEM_BAN_BE);
					}else{
						SEND(clientSocket, notok,YC_XEM_BAN_BE);
					}
				}
				goto Layout2;
				break;
			case 7 :
				break;
			case 8:
				SEND(clientSocket,logout,SIGN_OUT);
				myUser.status = 0;
				SEND(clientSocket,tmp,SIGN_OUT);
				strcpy(myUser.name,"\0");
				printf("-------------Bye-------\n");
				goto Layout1;
				break;
			default:
				printf("nhap lua chon hop le !\n");
				goto Layout2;
				break;
		}
					
	}//end while(1)
	close(clientSocket);
	return 0;
}