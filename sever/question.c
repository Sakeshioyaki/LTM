#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "lib.c"
#include "user.c"
#define fileName "account.txt"


#define MAXLINE 100
#define MAX 50 //max friend
// question
typedef struct question
{
  int id;
  char ques[200];
  char solustion[100];
}question;
typedef struct QuestionList
{
  question qs;
  struct QuestionList *next;
}QuestionList;
QuestionList *ques=NULL;
QuestionList *createNewQuestion(question question){
  QuestionList *node=(QuestionList *)malloc(sizeof(QuestionList));
  node->qs=question;
  node->next=NULL;
  return node;
}
void addQuestion(question question){
  QuestionList *newquestion=createNewQuestion(question);
  if(ques==NULL){
    ques=newquestion;
  }else{
    newquestion->next=ques;
    ques=newquestion;
  }
}
// user


void readfileGame(){
  question newquestion;
  FILE *f;
  f=fopen("question.txt","r");
  if(f==NULL){
    printf("file not exsist\n");
    return;
  }
  while(fscanf(f,"%d  %s  %s  ",&newquestion.id,newquestion.ques,newquestion.solustion)){
    printf("%d %s %s\n",newquestion.id,newquestion.ques,newquestion.solustion );
    addQuestion(newquestion);
  }
  fclose(f);
}