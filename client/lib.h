#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>

typedef enum {
	YC_KET_BAN,
	YC_CHOI_GAME,
	CHOI_DO_VUI,
	LOG_USERNAME.
	LOG_PASSWORD,
	SIGN_UP_USERNAME,
	SIGN_UP_PASSWORD
}CODE;

typedef struct message{
	CODE code;
	char mess[1042];
}message;

message *createMessage(char buff[], CODE code){
	message *newMess =  (message*)malloc(sizeof(message));
	newMess->code = code;
	strcpy(newMess->mess, buff);
	return newMess;
}

int SEND(int clientSockfd, char mess[1042], CODE code){
	
	if(send(clientSockfd, newMess, sizeof(newMess), 0) == -1){
			printf("Loi khi send()!\n");
			exit(0);
		}
}

