#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
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

void requestFriend(int clientSocket, char nameUser[MAXLINE]){
	char name[MAXLINE];
	MESSAGE mess;
	int check = 0;
	char ok[MAXLINE] = "OK";
	do{
		printf("Nhap ten nguoi ban muon ket ban : ");
		scanf("%s",name);
		printf(" name : %s\n", name);
		if(strcmp(name, nameUser)==0){
			printf("Vui long ket ban voi mot nguoi khac! Khong phai ban\n");
			check = 0;
		}else{
			SEND(clientSocket, name,YC_KET_BAN);
			mess = RECEVE(clientSocket);
			if(strcmp(mess.mess, NOTOK) == 0){
				printf("Khong ton tai nguoi dung trong he thong !\n");
				printf("Vui long nhap ten hop le\n");
				printf("<<Nhan 1 de ket thuc / 0 de tiep tuc>>\n");
				scanf("%d",&check);
			}else if(strcmp(mess.mess,"Da la ban be")==0){
				printf("Cac ban da la ban be\n");
				printf("<<Nhan 1 de ket thuc / 0 de tiep tuc>>\n");
				scanf("%d",&check);
			}
			else{
				printf(" mess nhan dc %s\n", mess.mess);
				if(strcmp(mess.mess, ok) == 0){
					printf("Yeu cau cua ban da duoc gui!\n");
					printf("Vui long cho xac nhan \n");
				}
				return;
			}
		}

	}while(check == 0);

}



int main(int argc, char const *argv[]){
	int clientSocket, ret, select, requestFd, i, tmp1, countFriend;
	struct sockaddr_in serverAddr;
	char tmp[MAXLINE] = "hello";
	char select1[MAXLINE];
	MESSAGE mess;
	char ok[MAXLINE] = "OK";
	char notok[MAXLINE] = "NOT OK";
	int statususer=0;
	int check;
	char playgame[30]="play game";
	int statuspass=0;
	char namesignin[100];
	char friendName[MAXLINE];
	char buffer[BUFFER_SZ];
	char message[BUFFER_SZ + NAME_LEN] = "";

    char offline[MAXLINE]= "offline";
    char notfriend[MAXLINE]="NOT FRIEND"; 
    char notUser[MAXLINE]="NOT USER";
    char sansang[MAXLINE]="SAN SANG";
    char notsansang[MAXLINE]="NOT SAN SANG";

    int doi;
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
		scanf("%s",select1);
		if(strlen(select1)!=1|| !isdigit(select1[0])) 
			break;
		select=atoi(select1);
		switch(select){
			case 1 :
			printf("vao login\n");
			SEND(clientSocket,tmp,LOG_USERNAME);
			mess=RECEVE(clientSocket);
			myUser=	loginUser(clientSocket,myUser, statususer,statuspass);
			if(myUser.status==1)
				goto Layout2;
			else 
			goto Layout1;	
				break;
			case 2 :
				myUser = singUp(clientSocket);
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
		printf("7: Gui tin nhan \n");
		printf("8: Log Out\n");
		printf("Nhap vao select : \n");
		scanf("%d",&select);
		printf("%d\n",select );
		switch(select){
			case 1:
				SEND(clientSocket,playgame,PLAY_GAME_WITH_SEVER);
				// mess=RECEVE(clientSocket);
				char ss[10]="ok";

				char answer[10];
				
				int count=0;

				while(count<5){
					char buff[MAXLINE]="";
					int K = recv(clientSocket,buff,MAXLINE,0);
					buff[K]='\0';
					if(K<=0){
						printf("loi khi nhan %d\n",K );
						exit(1);
					}
					printf("\n%s\n",buff );
					do{
						printf("Tra loi: \n");
						scanf(" %s", answer);
						// SEND(clientSocket,answer,PLAY_GAME_WITH_SEVER);

					}while(strcmp(answer,"A")!=0&&strcmp(answer,"B")!=0&&strcmp(answer,"C")!=0&&strcmp(answer,"D")!=0);
						send(clientSocket,answer,strlen(answer),0);
						char cho[100];
						recv(clientSocket,cho,100,0);
						printf("%s s\n",cho );
						count++;

					}
				char kiki[2]="";
				recv(clientSocket,kiki,2,0);
				printf("So cau tra loi dung la %s\n",kiki );
				goto Layout2;
				break;
			/*
			* choi game voi ban
			*/
			case 2:

				goto Layout2;
				break;

			/*
			* CHAT WITH FRIEND
			*/
			case 3:	
					printf("Nhap ten nguoi ban muon chat cung : ");
					scanf("%s",friendName);
					fflush(stdin);
					strcpy(tmp,friendName);
					SEND(clientSocket, tmp, CHAT);
					int receive = recv(clientSocket, buffer, BUFFER_SZ, 0);
					printf("CHUOI T DA NHAN DC LAAAAA : %s\n", buffer );
					if(strcmp(buffer,notUser)==0){
						printf("Khong ton tai nguoi nay trong he thong\n");
					}
					else if(strcmp(buffer,notfriend)==0){
						printf("2 ban khong phai ban be\n");
					}
					else if(strcmp(buffer,offline)==0){
						printf("User dang offline\n");
					}else if(strcmp(buffer,notsansang)==0){
						// printf("run to here\n");
						printf(">>%s\n",buffer);
						printf("Ban co muon doi ban cua minh khong ?\n");
						printf("1: co / 2: khong\n");
						scanf("%d", &doi);
						if(doi == 2){
							printf("OK ! BYE----\n");
							send(clientSocket,"KHONG DOI",strlen("KHONG DOI")+1,0);
						}else{
							bzero(buffer, BUFFER_SZ);
							send(clientSocket,"DOI",strlen("DOI")+1,0);
							printf("---------------Dang cho----------\n");
							receive = recv(clientSocket, buffer, BUFFER_SZ, 0);
							printf("318: %s\n", buffer);
							printf("Ban cua ban : %s\n",buffer);
							 pthread_t send_msg_thread;
						    if (pthread_create(&send_msg_thread, NULL, &send_msg_handler, &clientSocket) != 0)
						    {
						        printf("ERROR: pthread_create()");
						        exit(EXIT_FAILURE);
						    }

						    pthread_t recv_msg_thread;
						    if (pthread_create(&recv_msg_thread, NULL, &recv_msg_handler, &clientSocket ) != 0)
						    {
						        printf("ERROR: pthread_create()");
						        exit(EXIT_FAILURE);
						    }
						    while (1)
						    {
						        if(flag) 
						        {
						            printf("\nBye\n");
						            break;
						        }
						    }
						}
					}else{
						// receive = recv(clientSocket, buffer, BUFFER_SZ, 0);
						printf(" >> %s <<\n", buffer);
						pthread_t recv_msg_thread;
					    if (pthread_create(&recv_msg_thread, NULL, &recv_msg_handler, &clientSocket ) != 0)
					    {
					        printf("ERROR: pthread_create()");
					        exit(EXIT_FAILURE);
					    }
						pthread_t send_msg_thread;
					    if (pthread_create(&send_msg_thread, NULL, &send_msg_handler, &clientSocket) != 0)
					    {
					        printf("ERROR: pthread_create()");
					        exit(EXIT_FAILURE);
					    }
				        while (1)
					    {
					        if (flag) 
					        {
					            // printf("\nBye\n");
					            break;
					        }
					    }
					}
				goto Layout2;
				break;

			/*
			* KET BAN
			*/
			case 4:
				printf("Bat dau ket ban\n");
				requestFriend(clientSocket,myUser.name);
				goto Layout2;
				break;

			/*
			* XEM DANH SACH BA BE
			*/
			case 5:
				printf("dang muon xem danh sach ban be \n");
				strcpy(tmp, "");
				SEND(clientSocket,tmp,YC_XEM_DS_BAN_BE);
				mess = RECEVE(clientSocket);
				countFriend = atoi(mess.mess);
				printf("Ban hien co %d ban be\n", countFriend);
				for(i=1;i<=countFriend;i++){
					mess = RECEVE(clientSocket);
					printf("%d: %s \n",i, mess.mess );
					// mess = RECEVE(clientSocket);
					// printf("%-50s\n", mess.mess);
				}
				goto Layout2;
				break;

			/*
			* XEM YEU CAU KET BAN
			*/
			case 6:
				SEND(clientSocket,tmp,YC_XEM_BAN_BE);
				mess = RECEVE(clientSocket);
				requestFd = atoi(mess.mess);
				printf("So yeu cau ket ban la %d\n",requestFd );
				for(i=1; i<=requestFd; i++){
					printf("254: bat dau in \n");
					mess = RECEVE(clientSocket);
					printf("%d: %s\n",i, mess.mess );
					printf("1: dong y 0: tu choi\n");
					scanf("%d",&tmp1);
					printf("357 :da lua chon : %d\n",tmp1);
					if(tmp1 == 1){
						SEND(clientSocket, ok, YC_XEM_BAN_BE);
					}else{
						strcpy(notok,"NOT OK");
						SEND(clientSocket, notok,YC_XEM_BAN_BE);
						printf("381 : da gui %s\n", notok);
					}
				}
				goto Layout2;
				break;

			/*
			*GUI TIN NHAN
			*/	
			case 7:
				SEND(clientSocket,tmp,MESS);
				char messageFriend[MAXLINE];
				chat:
				mess=RECEVE(clientSocket);
				char messreact[MAXLINE];
				if(mess.code==PHAN_HOI_CHAT){
					printf("Tin nhan la :%s\n", mess.mess);
					fflush(stdin);
					fgets(messreact,sizeof(messreact),stdin);
					messreact[strlen(messreact)-1]='\0';
					SEND(clientSocket,messreact,PHAN_HOI_CHAT);
					goto chat;
				}
				else if(mess.code==MESS){
					printf("%s\n",mess.mess );
					char namefri[100];
					tieptuc:
					printf("nhap nguoi ban muon chat: ");
					scanf("%s",friendName);
					strcpy(namefri,friendName);
					SEND(clientSocket,friendName,MESS);
					mess=RECEVE(clientSocket);
					printf("%s\n",mess.mess );
					if(strcmp(mess.mess,"NOT OK")==0){
					printf("cacs ban khong phai ban be \n");
					}else{
						printf("%s-> ",namefri);
						fflush(stdin);
						fgets(messageFriend,sizeof(messageFriend),stdin);
						messageFriend[strlen(messageFriend)-1]='\0';
						SEND(clientSocket,messageFriend,MESS);
					}
				}
				goto Layout2;
				break;
			/*
			* LOGOUT
			*/
			case 8:
				myUser.status = 0;
				SEND(clientSocket,tmp,SIGN_OUT);
				strcpy(myUser.name,"\0");
				printf("-------------Bye-------\n");
				close(clientSocket);
				goto Layout1;

			default:
				printf("nhap lua chon hop le !\n");
				goto Layout2;
				break;
		}
					
	}//end while(1)
	close(clientSocket);
	return 0;
}