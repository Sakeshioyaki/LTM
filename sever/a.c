#include<stdio.h>
#include<string.h>
typedef struct message{
	char code[50];
	char mess[50];
}MESSAGE;

int main(){
	MESSAGE mess;
	char a[50]="hello/meo";
	char *token=strtok(a,"/");
	strcpy(mess.code,token);
	while(token!=NULL){
		strcpy(mess.mess,token);
		token=strtok(NULL," ");
	}
	printf("code in mess la %s\n",mess.code );
	printf("mess in mess la %s\n", mess.mess);
}