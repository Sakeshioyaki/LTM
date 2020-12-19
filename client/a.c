#include <stdio.h>
#include<string.h>
void taoMessage(char *mess,char *code){
	//char *save = mess;
	char resurlt[10];
	// char pc[1] = "/";
	strcpy(resurlt,code);
	printf("chuoi resurlt ls %s\n",resurlt );
	strcat(resurlt,"/");
	// printf("chuoi pc ls %s\n",pc );
	printf("chuoi resurlt lan thu 2 la %s\n",resurlt );
	strcat(resurlt,mess);
	// printf("chuoi sau ki noi mess %s\n", resurlt);
	// //mess = NULL;
	strcpy(mess, resurlt);
	// return mess;
}
int main(){

	char mess[5]="haha";
	char code[5]="kaka";
	// char sendmess[10];
	taoMessage(mess,code);
	printf("%s\n",mess );
}