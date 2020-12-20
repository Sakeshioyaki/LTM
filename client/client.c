#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "lib.c"
#include<stdio_ext.h>
#define PORT 8080

int main(int argc, char const *argv[]){
	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[1024];
	char namesignin[100];
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Client Socket is created.\n");

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(atoi(argv[2]));
	serverAddr.sin_addr.s_addr = inet_addr(argv[1]);

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Connected to Server.\n");

	bd: while(1){
			printf("---------------------------------------------\n");
			printf("WELLCOME TO 'CHILL WITH YOU'\n");
			printf("---------------------------------------------\n");
//Layout 1 : Login and sign up
			Layout1:
			printf("Login or sign up\n");
			printf("1: Login\n");
			printf("2: Sign up\n");
			printf("3. Sign out\n");
			printf("4. Ket ban\n");
			int select;

			printf("Nhap vao select : \n");
			scanf("%d",&select);
			if( select== 0){
				printf("Vui long nhap dau vao hop le \n");
				goto Layout1;
			}
			
			else if(select == 1){
				char nameUser[1024];
					char result[1024],password[1024],recevepass[100],receive[100];
					send(clientSocket,"LOG_USERNAME",strlen("LOG_USERNAME")+1,0);
					printf("--------------------LOGIN-------------------\n");
					
					recv(clientSocket,receive,100,0);

					if(strcmp(receive,"tai khoan da dang nhap")==0){
						goto bd;
					}
					int statususer=0;
					int statuspass=0;
				while(statususer==0){
					printf("%s : ",receive );
					__fpurge(stdin);
					fgets(nameUser,sizeof(nameUser),stdin);
					nameUser[strlen(nameUser)-1]='\0';
					int Send = send(clientSocket, nameUser,strlen(nameUser)+1,0);
					// int Send=SEND(clientSocket,nameUser,LOG_USERNAME);
					// if(Send == -1){
					// 	printf("Loi khi send()!\n");
					// 	exit(0);
					// }
					
					recv(clientSocket, result, 1024, 0);

					// printf("%s\n", result);
					if(strcmp(result,"NOT OK") == 0){
						printf("Khong ton tai nguoi dung trong he thong\n");
						printf("\n");
						
					}else{
						statususer=1;
						while(statuspass==0){
							printf("%s\n", result);
							__fpurge(stdin);
							fgets(password, sizeof(password), stdin);
							password[strlen(password)-1]='\0';
							send(clientSocket, password,strlen(password)+1,0);
							recv(clientSocket, recevepass, 100, 0);
							if(strcmp(recevepass,"NOT OK") == 0){
								printf("Sai password\n");
								
							}
							
							else{
								statuspass=1;
								printf("%s\n", recevepass);
								
							}
						}
					}


					
					}
				}// end if(select == 1)

				else if(select==2){
					printf("---------------------SIGN UP------------------\n");
					char nameUser[1024];
					char inputname[30],inputpass[30],inputpasslai[30],passwordlai[1024];
					char result[1024],password[1024],recevepass[100];
					char press[5];
					int k=0;
					send(clientSocket,"SIGN_UP",strlen("SIGN_UP")+1,0);
					
						y:recv(clientSocket,nameUser,1024,0);
						printf("%s ",nameUser );
						__fpurge(stdin);
						fgets(inputname,sizeof(inputname),stdin);
						inputname[strlen(inputname)-1]='\0';
						send(clientSocket,inputname,strlen(inputname)+1,0);//name
						// passs
						recv(clientSocket,password,1024,0);
						printf("password nhan duoc la %s\n",password );
						if(strcmp(password,"tai khoan da ton tai (press y de tiep tuc, q de out")==0){
							scanf("%s",press);
							send(clientSocket,press,strlen(press),0);
							if(strcmp(press,"y")==0){
								goto y;
							}
							else{
								goto bd;
							}
						}
						printf("%s ",password );
						__fpurge(stdin);
						fgets(inputpass,sizeof(inputpass),stdin);
						inputpass[strlen(inputpass)-1]='\0';
						send(clientSocket,inputpass,strlen(inputpass)+1,0);
						// nhap laij pass 
						recv(clientSocket,passwordlai,1024,0);
						while(k==0){
						printf("%s ",passwordlai );
						__fpurge(stdin);
						fgets(inputpasslai,sizeof(inputpasslai),stdin);
						inputpasslai[strlen(inputpasslai)-1]='\0';
						send(clientSocket,inputpasslai,strlen(inputpasslai)+1,0);
						// success 
						recv(clientSocket,result,1024,0);
						if(strcmp(result,"sign up success")==0){
							k=1;
							printf("%s\n",result );
						}
						else{
							printf("%s\n", result);
						}
				}

			}
			else if(select==3){
				char receive[100];
				char name[100],bye[100];
				send(clientSocket,"SIGN_OUT",strlen("SIGN_OUT")+1,0);
				recv(clientSocket,receive,100,0);
				printf("%s ",receive );
				__fpurge(stdin);
				fgets(name,sizeof(name),stdin);
				name[strlen(name)-1]='\0';
				send(clientSocket,name,strlen(name)+1,0);
				recv(clientSocket,bye,20,0);
				printf("%s\n",bye );
			}
			
			
	}//end while(1)
	close(clientSocket);
	return 0;
}