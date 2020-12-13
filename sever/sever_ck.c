#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<sys/socket.h>
#include<errno.h>
#include<netdb.h>
#include<arpa/inet.h>
#define fileName "users.txt"
#define MAX 50 //max friend

typedef struct profile{
	int ID;
	char name[50];
	char password[10];
	int IDBFF[MAX];
	int status;
}profile;

/* status = 0 : offline
status = 1 : online
status = 2 : gaming
status = 3 : chat with friend */
typedef struct users{
	profile acc;
	struct users* next;
}users; 

users* listUser = NULL;

users* createNewUser(profile a){
	users* node = (users *)malloc(sizeof(users)); 
	node->acc = a;
	node->next = NULL;
	node->IDBFF = NULL;
	return node;
}

void addUser(profile a){
	users* newUser = createNewUser(a);
	if(listUser == NULL){
		listUser = newUser;
	}else{
	newUser->next = listUser;
	listUser = newUser;
	}
}

users* searchUser(char nameUser[]){
	if(listUser == NULL){
		printf("List user is NULL !\n");
		return NULL;
	}
	users* tmp = listUser;
	while(tmp != NULL){
		if(strcmp(tmp->acc.name, nameUser) == 0 ){
			return tmp;
		}
		tmp = tmp->next;
	}
	return NULL;
}

void printListUser(){
	users* tmp = listUser;
	printf("Nguoi dung\n");
	while(tmp != NULL){
		printf("%s\n", tmp->acc.name);
		tmp = tmp->next;
	}
}

void readFile(){
	profile newUser;
	FILE *fb;
	fb = fopen(fileName,"r");
	if(fb == NULL) {
		printf("File NULL !! \n");
		return;
	}
    while(fscanf(fb, "%s%s%d%s", newUser.name, newUser.password, &newUser.status, newUser.homePage) != EOF){
    	addUser(newUser);
    	printf("%s - %s\n",newUser.name, newUser.homePage);
    }
    fclose(fb);

}

profile* login(){
	char nameUser[50];
	char passwordUser[10];
	int countLogin = 0;
	printf("Ten dang nhap : \n");
	scanf("%s",nameUser);
	users* user =  searchUser(nameUser);
	if(user == NULL){
		printf("Khong tim thay nguoi dung !\n");
		return NULL;
	}
	do { 
		printf("Nhap password : ");
		scanf("%s",passwordUser);
		if(strcmp(passwordUser,user->acc.password) == 0){
			printf("Dang nhap thanh cong \n");
			return &user->acc;
		}else{
			countLogin++;
			printf("Password khong dung !\n");
			if(countLogin == 3){
				printf("Tai khoan tam thoi bi khoa do nhap mat khau sai qua 3 lan !\n");
				user->acc.status = 0;
				return NULL;
			}
		}
	}while(countLogin < 3);
	return NULL;
}

void WriteToFile(profile newAccount){
	FILE *fb = fopen(fileName, "a+");
	//fprintf(fb, "\0");
	fprintf(fb, "%s\t%s\t%d\n", newAccount.name, newAccount.password, newAccount.status);
	fclose(fb);
}

void Register(){
	char myPassword[10];
	char myName[50];
	profile newAccount;
	int tmp;
	printf("Nhap ten dang nhap : \n");
	scanf("%s",myName);
	if(searchUser(myName) != NULL){
		printf("Ten nay da duoc su dung !\n");
	}else{
		printf("Nhap password (toi da 5 ki tu):\n");
		scanf("%s",myPassword);
		strcpy(newAccount.name,myName);
		strcpy(newAccount.password,myPassword);
		newAccount.status = 3;
		WriteToFile(newAccount);
		addUser(newAccount);
		printf("Tao tai khoan thanh cong ! Vui long dang nhap lai. \n");
		}
}

int main(int argc, char*argv[]){

	f(argc != 3){
		printf("Thieu or thua du lieu ! \n");
		return 0;
	}

	readFile();

	int PORT = atoi(argv[1]);
	if(PORT == 0){
		printf("Du lieu  nhap vao khong dung !\n");
		return 0;
	}

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
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr(INADDR_ANY);

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


	while(1){
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
		// char FileName[100]; 
  //           strcpy(FileName, inet_ntoa(newAddr.sin_addr));
  //           char port[10];
  //           sprintf(port,"%d", ntohs(newAddr.sin_port));
  //           strcat(FileName,port);
  //           FILE *fb = fopen(FileName, "a+");
		// 	fprintf(fb, "\0");
		// 	fprintf(fb, "socket fd is %d , ip is : %s , port : %d \n",  newSocket , inet_ntoa(newAddr.sin_addr) , ntohs(newAddr.sin_port));
		// 	fclose(fb);


		if((childpid = fork()) == 0){
			close(sockfd);

			while(1){
				recv(newSocket, buffer, 1024, 0);
				if(strcmp(buffer, ":exit") == 0){
					printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
					break;
				}else{
					printf("Client: %s\n", buffer);
					send(newSocket, buffer, strlen(buffer), 0);
					bzero(buffer, sizeof(buffer));
				}
			}
		}

	}

	close(newSocket);


	return 0;
	
}

/*
profile *myUser = NULL;
	do{
		printf("--------------------------------\n");
		printf("USER MANAGERMENT PROGRAM\n");
		if(myUser != NULL){
			printf(" =>> Xin chao %s !\n",myUser->name);
		}
	}
*/
