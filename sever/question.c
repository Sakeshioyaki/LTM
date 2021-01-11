#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include<ctype.h>
#include <time.h>
#define MAX_LEN 200
#define MAX_LINE 500
#define numberquestion 5
#define maxquestion 23
struct question
{
  char question[MAX_LEN];
  char A[MAX_LEN];
  char B[MAX_LEN];
  char C[MAX_LEN];
  char D[MAX_LEN];
  char TrueAnswer[10];
  
};
struct question Question[MAX_LEN];
void readQuestion(){
  FILE *f;
  f=fopen("question.txt","r");
  if(f==NULL){
    printf("error opening \n");
    exit(1);
  }
  for(int i=0;i<maxquestion;i++){
    char question[MAX_LEN];
    if(fgets(question,sizeof(question),f)!=NULL){
      strcpy(Question[i].question,question);
    }
    char A[MAX_LEN];
    if(fgets(A,sizeof(A),f)!=NULL){
      strcpy(Question[i].A,A);
    }
    char B[MAX_LEN];
    if(fgets(B,sizeof(B),f)!=NULL){
      strcpy(Question[i].B,B);
    }
    char C[MAX_LEN];
    if(fgets(C,sizeof(C),f)!=NULL){
      strcpy(Question[i].C,C);
    }
    char D[MAX_LEN];
    if(fgets(D,sizeof(D),f)!=NULL){
      strcpy(Question[i].D,D);
    }
    char TrueAnswer[MAX_LEN];
    if(fgets(TrueAnswer,sizeof(TrueAnswer),f)!=NULL){
      TrueAnswer[strlen(TrueAnswer)-1]='\0';
      strcpy(Question[i].TrueAnswer,TrueAnswer);
    }

  }
  fclose(f);
}
int isTrung(int mang[],int count, int k ){
  for (int i=0;i<count;i++){
    if(k==mang[i]){
      return 1;
    }
  }
  return 0;
}

char *strlwr(char *str)
{
  unsigned char *p = (unsigned char *)str;

  while (*p) {
     *p = tolower((unsigned char)*p);
      p++;
  }

  return str;
}
void randomquestion(int mang[numberquestion]){
  int k;
  srand(time(NULL));
  for(int i=0;i<numberquestion;i++){
    do{
      k=rand()%maxquestion;
    }while(isTrung(mang,i+1,k)==1);
    mang[i]=k;
  }
}

void printfquestion(){
  for(int i=0;i<numberquestion;i++){
    printf("%s\n",Question[i].question );
    printf("%s\n",Question[i].A );
    printf("%s\n",Question[i].B );
    printf("%s\n",Question[i].C );
    printf("%s\n",Question[i].D );
    printf("%s\n",Question[i].TrueAnswer );
  }
}
// int main(){
//   readQuestion();
//   Message mess;
//   // printfquestion();
//   int mang[15];
//   randomquestion(mang);
//   for(int i=0;i<15;i++){
//     printf("%d  ", mang[i]);
//   }
//   char messSend[MAX_LEN];
//   int trueQuestion=0;
//   // sprintf(mess,"Cau hoi %d:%s\nA. %s\nB. %s\nC. %s\nD. %s\n Dap an la : ",);
//   for(int i=0;i<15;i++){
//     int k=snprintf(messSend,sizeof(messSend),"Cau hoi %d: %s\nA. %s\nB. %s\nC. %s\nD. %s\n Dap an la : ",i+1,Question[i].question,Question[i].A,Question[i].B,Question[i].C,Question[i].D);
//     SEND(newSocket,messSend,PLAY_GAME_WITH_SEVER);
//     mess=RECEIVE(newSocket);
//     if(strcmp(Question[i],mess.mess)==0){
//       sprintf(messSend,"ban tra loi dung");
//       trueQuestion++;
//       SEND(newSocket,messSend,PLAY_GAME_WITH_SEVER);
//     }
//     else{
//       sprintf(messSend,"tra loi sai!Dap an dung la %s",Question[i].trueQuestion);
//       SEND(newSocket,messSend,PLAY_GAME_WITH_SEVER);

//     }

//   }

// }
