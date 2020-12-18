#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef enum {
	YC_KET_BAN,
	YC_CHOI_GAME,
	CHOI_DO_VUI,
	LOG_USERNAME,
	LOG_PASSWORD,
	SIGN_UP_USERNAME,
	SIGN_UP_PASSWORD
}CODE;

typedef struct message{
	char code[30];
	char mess[1042];
}MESSAGE;


// char *createMessage(char buff[]){
// 	char *newMess =  (char*)malloc(sizeof(char));
// 	strcpy(newMess, buff);
// 	return newMess;
// }

int SEND(int clientSockfd, char mess[1024],int len,int flag){
	// char newMess = createMessage(mess);
	if(send(clientSockfd, mess, 1024, 0) == -1){
			printf("Loi khi send()!\n");
			return -1;
		}
		return 1;
}
char* RECEIVE(int clientSockfd, char message[1024]){
	MESSAGE mess;
	char *token=strtok(message,"/");
	strcpy(mess.code,token);
	while(token!=NULL){
		strcpy(mess.mess,token);
		strtok(NULL,"/");
	}
}