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
	SIGN_UP_PASSWORD,
	SIGN_OUT,
	YC_XEM_BAN_BE,
	YC_XEM_DS_BAN_BE,
	CHAT,
	PHAN_HOI_CHAT,
	PLAY_GAME_WITH_SEVER,
	PLAY_GAME_WITH_FD
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
MESSAGE tachChuoi(char message[1024]){
	MESSAGE mess;
	CODE code;
	char *token = strtok(message,"/");
	if(strcmp(token, "YC_KET_BAN") == 0){
		code = YC_KET_BAN;
	}
	if(strcmp(token, "YC_CHOI_GAME") == 0){
		code = YC_CHOI_GAME;
	}
	if(strcmp(token, "CHOI_DO_VUI") == 0){
		code = CHOI_DO_VUI;
	}
	if(strcmp(token, "LOG_USERNAME") == 0){
		code = LOG_USERNAME;
	}
	if(strcmp(token, "LOG_PASSWORD") == 0){
		code = LOG_PASSWORD;
	}
	if(strcmp(token, "SIGN_UP_USERNAME") == 0){
		code = SIGN_UP_USERNAME;
	}
	if(strcmp(token, "SIGN_UP_PASSWORD") == 0){
		code = SIGN_UP_PASSWORD;
	}
	if(strcmp(token, "SIGN_OUT") == 0){
		code = SIGN_OUT;
	}
	if(strcmp(token, "YC_XEM_BAN_BE") == 0){
		code = YC_XEM_BAN_BE;
	}
	if(strcmp(token, "YC_XEM_DS_BAN_BE") == 0){
		code = YC_XEM_BAN_BE;
	}
	if(strcmp(token, "CHAT") == 0){
		code = CHAT;
	}

	if(strcmp(token,"PHAN_HOI_CHAT")==0){
		code=PHAN_HOI_CHAT;
	}
	if(strcmp(token, "PLAY_GAME_WITH_SEVER") == 0){
		code = PLAY_GAME_WITH_SEVER;
	}
	if(strcmp(token, "PLAY_GAME_WITH_FD") == 0){
		code = PLAY_GAME_WITH_FD;
	}
	mess.code = code;
	while(token != NULL){
		strcpy(mess.mess, token);
		token = strtok(NULL,"/");
	}
	return mess;
}
MESSAGE RECEVE(int newSocket){
	MESSAGE mess;
	char messClient[1024];
	recv(newSocket,messClient,1024,0);
	mess=tachChuoi(messClient);
	return mess;

}

void taoMessage(char *mess, const char *code){
	char resurlt[100];
	strcpy(resurlt,code);
	strcat(resurlt,"/");
	strcat(resurlt,mess);
	strcpy(mess, resurlt);
}

int SEND(int clientSockfd, char *mess, CODE code){

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
		case SIGN_OUT:
			taoMessage(mess,"SIGN_OUT");
			break;
		case YC_XEM_BAN_BE:
			taoMessage(mess,"YC_XEM_BAN_BE");
			break;
		case YC_XEM_DS_BAN_BE:
			taoMessage(mess,"YC_XEM_DS_BAN_BE");
			break;
		case CHAT:
			taoMessage(mess,"CHAT");
			break;
		case PHAN_HOI_CHAT:
			taoMessage(mess,"PHAN_HOI_CHAT");
			break;
		case PLAY_GAME_WITH_SEVER:
			taoMessage(mess,"PLAY_GAME_WITH_SEVER");
			break;
		case PLAY_GAME_WITH_FD:
			taoMessage(mess,"PLAY_GAME_WITH_FD");
			break;
		default:
			printf("SAI MA CODE !");
	}


	// MESSAGE *newMess = createMessage(mess, code);
	int k=send(clientSockfd, mess, strlen(mess)+1, 0);
	return k;
}
