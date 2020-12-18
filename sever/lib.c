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
	LOG_USERNAME.
	LOG_PASSWORD,
	SIGN_UP_USERNAME,
	SIGN_UP_PASSWORD
}CODE;

typedef struct MESSAGE{
	CODE code;
	char mess[1042];
}MESSAGE;

MESSAGE *createMessage(char buff[], CODE code){
	MESSAGE *newMess =  (MESSAGE*)malloc(sizeof(MESSAGE));
	newMess->code = code;
	strcpy(newMess->mess, buff);
	return newMess;
}

MESSAGE tachChuoi(char [1024]){
	MESSAGE mess;
	char *token = strtok(message,"/");
	strcpy(mess.code,token);
	while(token != NULL){
		strcpy(mess.mess, token);
		strtok(NULL,"/");
	}
	return mess;
}

void taoMessage(char *mess, const char *code){
	//char *save = mess;
	char resurlt[100];
	char pc[1] = "/";
	strcpy(resurlt,code);
	strcat(resurlt,pc);
	strcat(resurlt,mess);
	//mess = NULL;
	strcpy(mess, resurlt);
}

int SEND(int clientSockfd, char mess[1042], CODE code){

	switch(code){
		case YC_KET_BAN:
			taoMessage(mess,"YC_KET_BAN");
			break;
		case YC_CHOI_GAME:
			taoMessage(mess,"YC_CHOI_GAME");
			break;
		case CHOI_DO_VUI:
			taoMessage(mess,"CHOI_DO_VUI");
			break;
		case LOG_USERNAME:
			taoMessage(mess,"LOG_USERNAME");
			break;
		case LOG_PASSWORD:
			taoMessage(mess,"LOG_PASSWORD");
			break;
		case SIGN_UP_USERNAME:
			taoMessage(mess,"SIGN_UP_USERNAME");
			break;
		case SIGN_UP_PASSWORD:
			taoMessage(mess,"SIGN_UP_PASSWORD");
			break;
		default:
			printf("SAI MA CODE !");
	}


	// MESSAGE *newMess = createMessage(mess, code);
	if(send(clientSockfd, mess, sizeof(mess), 0) == -1){
			printf("Loi khi send()!\n");
			exit(0);
		}
}