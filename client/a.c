#include <stdio.h>
#include<string.h>
typedef enum {
	YC_KET_BAN,
	YC_CHOI_GAME,
	CHOI_DO_VUI,
	LOG_USERNAME,
	LOG_PASSWORD,
	SIGN_UP_USERNAME,
	SIGN_UP_PASSWORD
}CODE;

typedef struct MESSAGE{
	CODE code;
	char mess[1042];
}MESSAGE;
int convert(char a[100]){
	if(strcmp(a,"YC_KET_BAN")==0){
		return 1;
	}
	else if(strcmp(a,"YC_CHOI_GAME")==0){
		return 2;
	}
	else if(strcmp(a,"CHOI_DO_VUI")==0){
		return 3;
	}
	else if(strcmp(a,"LOG_USERNAME")==0){
		return 4;
	}
	else if(strcmp(a,"LOG_PASSWORD")==0){
		return 5;
	}
	else if(strcmp(a,"SIGN_UP_USERNAME")==0){
		return 6;
	}
	else if(strcmp(a,"SIGN_UP_PASSWORD")==0){
		return 7;
	}
	else 
		return 0;



}
MESSAGE tachChuoi(char message[1024]){
	MESSAGE mess;
	char mama[100];
	char *token = strtok(message,"/");
	int k=convert(token);
	mess.code=k;
	while(token != NULL){
		// printf("gia tri cua token la %s\n",token );
		strcpy(mess.mess, token);

		strtok(NUsLL,"/");
		
		
	}
	printf("token la %s\n",token );
	
	return mess;
}
MESSAGE RECEVE( char messClient[1024]){
	MESSAGE mess;
	// recv(newSocket,messClient,1024,0);
	mess=tachChuoi(messClient);
	return mess;

}
int main(){
	char messClient[1024]="LOG_USERNAME/haha";
	MESSAGE mess=tachChuoi(messClient);
	printf("%s\n",mess.mess );
	printf("%d\n",mess.code );

}