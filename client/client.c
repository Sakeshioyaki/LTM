#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio_ext.h>
#include <pthread.h>
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

int game(int clientSocket){
	MESSAGE mess;
	char end[10]="END";
	char solution[40];
	int qus=0; 
	
	play:
	qus++;
	mess=RECEVE(clientSocket);
	if(strstr(mess.mess,end)!=NULL){
		printf("%s\n",mess.mess );
		return 0;
	}
	else{
		printf("Question %d : %s \n",qus,mess.mess );
		__fpurge(stdin);
		fgets(solution,sizeof(solution),stdin);
		solution[strlen(solution)-1]='\0';
		SEND(clientSocket,solution,YC_CHOI_GAME);
		mess=RECEVE(clientSocket);
		printf("server: %s\n",mess.mess );
		char request[10]="ok";
		SEND(clientSocket,request,YC_CHOI_GAME);
		goto play;
	}
	
	
}

void *msg_handler(int clientSocket){
	char message[100]="";
	while(1){
		int receive=recv(clientSocket,message,100,0);
		if(receive>0){
			printf("mess nhan duoc la %s\n", message);
			break;

		}
		printf("receive =0\n");
		
	}
}

int main(int argc, char const *argv[]){
	int clientSocket, ret, select, requestFd, i, tmp1;
	struct sockaddr_in serverAddr;
	pthread_t msg;
	char buffer[1024];
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
	char u[20]="choi game";
	int check,k;
	int statuspass=0;
	char namesignin[100];
	char friendName[MAXLINE];
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
				myUser = sigUp(clientSocket);
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
		printf("7: Log Out\n");
		printf("8: test\n");
		printf("Nhap vao select : \n");
		scanf("%d",&select);
		printf("%d\n",select );
		switch(select){
			case 1:
				SEND(clientSocket,u,YC_CHOI_GAME);
				k=game(clientSocket);
				if(k==0){
					goto Layout2;
				}
				break;
			case 2:
				break;
			case 3:
			    SEND(clientSocket,tmp,CHAT);
			    chat: 
				mess=RECEVE(clientSocket);
				if(mess.code==PHAN_HOI_CHAT){
					printf("vao duoc day roi nhes\n");
					printf("tin nhann la : %s\n",mess.mess );
					__fpurge(stdin);
					fgets(messreact,sizeof(messreact),stdin);
					messreact[strlen(messreact)-1]='\0';
					SEND(clientSocket,messreact,PHAN_HOI_CHAT);
					goto chat;
				}
				else if(mess.code==CHAT){
					printf("mess nhan duoc la %s\n", mess.mess);
					tieptuc:
						printf("Nhap ten nguoi ban muon chat cung : ");
						scanf("%s",friendName);
						system("cls");
						SEND(clientSocket, friendName, CHAT);
						printf("da send : %s\n", friendName);
						mess = RECEVE(clientSocket);
						printf("mess.mess nhan duoc la %s\n",mess.mess );
						if(strcmp(mess.mess, "NOT OK")==0){
							printf("Cac ban khong phai ban be !\n");
							// printf("1 :tiep tuc / 0 :dung\n");
							// scanf("%d", &check);
							// if(check==1){
							// 	goto Layout2;
							// }
											
						}
						else{
							printf("Da san sang de chat \n");
							printf("friendName-> nhap tin nhan \n");
							__fpurge(stdin);
							fgets(messageFriend,sizeof(messageFriend),stdin);
							messageFriend[strlen(messageFriend)-1]='\0';
							SEND(clientSocket,messageFriend,CHAT);	
						}


					}
					
				
				goto Layout2;
				break;
			case 4:
				printf("Bat dau ket ban\n");
				requestFriend(clientSocket);
				goto Layout2;
				break;
			case 5:
				printf("ban co %s ban be \n",mess.mess );
				break;
			case 6:
				SEND(clientSocket,tmp,YC_XEM_BAN_BE);
				mess = RECEVE(clientSocket);
				printf("so yeu cau ket ban la %d\n",atoi(mess.mess) );
				char gui[100]="gui ds";
				SEND(clientSocket,gui,YC_XEM_BAN_BE);
				// for(i=1; i<=requestFd; i++){
					xemban:
					mess = RECEVE(clientSocket);
					if(strcmp(mess.mess,"END")==0){
						goto Layout2;
					}
					else{
						printf("%d: %s\n",i, mess.mess );
						printf("1: dong y 0: tu choi\n");
						scanf("%d",&tmp1);
						if(tmp1 == 1){
							SEND(clientSocket, ok, YC_XEM_BAN_BE);
							goto xemban;
						}else{
							SEND(clientSocket, notok,YC_XEM_BAN_BE);
							goto xemban;
						}
					}
					
				// }
				printf("ra day chua\n");
				goto Layout2;
				break;
			case 7:
				SEND(clientSocket,logout,SIGN_OUT);
				myUser.status = 0;
				strcpy(myUser.name,"\0");
				goto Layout1;
				break;
			case 8: 
				printf("Nhap ten username can gui mess: ");
				scanf("%s",nameuser);
				SEND(clientSocket,nameuser,PLAY_GAME_WITH_FD);
				if(pthread_create(&msg,NULL,msg_handler(clientSocket),NULL)!=0){
					printf("error\n");
					exit(1);
				}


			default:
				printf("nhap lua chon hop le !\n");
				goto Layout2;
				break;
		}
					
	}//end while(1)
	close(clientSocket);
	return 0;
}