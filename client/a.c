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
	char *token;
	token = strtok(message,"/");
	// int k=convert(token);
	// mess.code=k;
	printf("gia tri mess.code la %s\n",token );
	
	while(token != NULL){
		k++;
		// printf("gia tri cua token la %s\n",token );
		strcpy(mess.mess, token);

		token=strtok(NULL,"/");
		
	}

	// printf("token mess.mess la %s\n",mess.mess );
	
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









void loginUser(int newSocket){
  int statususer=0;
  int statuspass=0;
  while (statususer==0)
  {
    MESSAGE mess= RECEVE(newSocket);
    printf("nhan xong r nhe\n");
    userInfo* user =  searchUser(mess.mess);
    if(user == NULL){
         SEND(newSocket,"LOG_USERNAME NOT OK",mess.code);
     }
     else{
       statususer=1;
       SEND(newSocket,"LOG_USERNAME OK",mess.code);
       while(statuspass==0){
         MESSAGE messpass= RECEVE(newSocket);
          if(strcmp(messpass.mess,user->acc.password)==0){
              SEND(newSocket,"login access",messpass.code);
              statuspass=1;
          }
          else{
            SEND(newSocket,"LOG_PASSWORD NOT OK",messpass.code);
          }
       }
     }
     
  }
  
  
}








client


void loginUser(int clientSocket){
	int statususer=0;
	int statuspass=0;
	while(statususer==0){
		char userNameLogIn[30];
		char passLogin[20]; 
		printf("User Name : ");
		__fpurge(stdin);
		fgets(userNameLogIn,sizeof(userNameLogIn),stdin);
		userNameLogIn[strlen(userNameLogIn)-1]='\0';
		SEND(clientSocket, userNameLogIn, LOG_USERNAME);
		MESSAGE mess=RECEVE(clientSocket);
		printf("day la %s",mess.mess);
		if(strcmp(mess.mess,"LOG_USERNAME OK")==0){
			statususer=1;
			while(statuspass==0){
				printf("User pass : ");
				// __fpuge(stdin);
				scanf("%s",passLogin);
				// fgets(passLogin,sizeof(passLogin),stdin);
				SEND(clientSocket, passLogin, LOG_PASSWORD);
				MESSAGE messpass=RECEVE(clientSocket);
				printf("%s",messpass.mess);
				if(strcmp(messpass.mess,"login access")==0){
					statuspass=1;
					printf("%s",messpass.mess);
				}
			}
			
		}
		
	}
}