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

int main(){
	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[1024];

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Client Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Connected to Server.\n");

	while(1){
			printf("---------------------------------------------\n");
			printf("WELLCOME TO 'CHILL WITH YOU'\n");
			printf("---------------------------------------------\n");
//Layout 1 : Login and sign up
			Layout1:
			printf("Login or sign up\n");
			printf("1: Login\n");
			printf("2: Sign up\n");
			int select;
			if(scanf("%d",&select) == 0){
				printf("Vui long nhap dau vao hop le \n");
				goto Layout1;
			}
			else{
				if(select == 1){
					printf("--------------------LOGIN-------------------\n");
					char nameUser[1024];
					printf("Nhap ten user : \n");
					fgets(nameUser,sizeof(nameUser)-1,stdin);
					printf("buff1 : %s", nameUser);	
					char *p = strchr(nameUser, '\n');
					if(p != NULL){
						*p = '\0';
					}
					//send
					int Send = send(clientSocket, nameUser,sizeof(nameUser),0);
					if(Send == -1){
						printf("Loi khi send()!\n");
						exit(0);
					}
					printf("buff : %s", nameUser);

					char result[1024],password[1024];
					//char *p;
					int countPass = 0 ;
					recv(clientSocket, result, sizeof(result), 0);

					//printf("%s\n", result);
					// if(strcmp(result,"NOT OK") == 0){
					// 	printf("Khong ton tai nguoi dung trong he thong\n");
					// 	exit(0);
					// }else{
					// 	do{
					// 		countPass++;
					// 		printf("Moi nhap password: \n");
					// 		fgets(password, sizeof(password), stdin);
					// 		char *p = strchr(password, '\n');
					// 		if( p!= NULL){
					// 			*p = '\0';
					// 		}
					// 		Send = send(clientSocket, password,strlen(password),0);
					// 		if(Send == -1){
					// 			printf("Loi khi send()!\n");
					// 			exit(0);
					// 		}
					// 		recv(clientSocket, result, sizeof(result), 0);
					// 		if(strcmp(result,"NOT OK") == 0){
					// 			printf("Sai password\n");
					// 		}else{
					// 			printf("Dang nhap thanh cong\n");
					// 			char statuss[18];
					// 			recv(clientSocket, statuss, sizeof(statuss), 0);
					// 			if(strcmp(statuss, "Account not ready") == 0 ){
					// 				printf("%s\n", statuss );
					// 				exit(0); 
					// 			}else{
					// 				printf("%s\n", statuss);
					// 				char newpassword[30];
					// 				printf("Moi nhap mat khau moi <nhap 'bye' de ket thuc> : \n");
					// 				fgets(newpassword, sizeof(newpassword), stdin);
					// 				char *p = strchr(newpassword, '\n');
					// 				if( p!= NULL){
					// 					*p = '\0';
					// 				}
					// 				if(strcmp(newpassword, "bye")==0){
					// 					Send = send(clientSocket, newpassword,strlen(newpassword),0);
					// 					printf("OK bye!\n");
					// 					exit(0);
					// 				}

					// 				Send = send(clientSocket, newpassword,strlen(newpassword),0);
					// 				if(Send == -1){
					// 					printf("Loi khi send()!\n");
					// 					exit(0);
					// 				}

					// 				char checkPass[30];
					// 				recv(clientSocket, checkPass, sizeof(checkPass), 0);
					// 				if(strcmp(checkPass, "Password hop le") == 0){


					// 				}else{
					// 					printf("Password khong hop le !\n");
					// 				}
					// 			}
					// 			break;
					// 		}

					// 	}while(countPass<3);
					// }//end else

					// if(strcmp(buffer, ":exit") == 0){
					// 	close(clientSocket);
					// 	printf("[-]Disconnected from server.\n");
					// 	exit(1);
					// }

					// if(recv(clientSocket, buffer, 1024, 0) < 0){
					// 	printf("[-]Error in receiving data.\n");
					// }else{
					// 	printf("Server: \t%s\n", buffer);
					// }
				}// end if(select == 1)
				else{
					printf("---------------------SIGN UP------------------\n");

				}
			}
	}//end while(1)

	return 0;
}