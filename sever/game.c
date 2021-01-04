#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>
// #iBBnclude "lib.c"
// #include "unicode.c"
typedef struct question
{
  int id;
  char ques[800];
  char solustion[500];
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
void printListQuestion(){
  QuestionList* tmp = ques;
  printf("List question: \n");
  while(tmp != NULL){
    printf("%d   %s     %s\n", tmp->qs.id,tmp->qs.ques,tmp->qs.solustion);
    tmp = tmp->next;
  }
}

int lengList(){
  QuestionList* tmp = ques;
  int k=0;
  while(tmp != NULL){
    k++;
    tmp = tmp->next;
  }
  return k;
}

void readfileGame(){
  question newquestion;
  FILE *f;
  char result[1024];
  f=fopen("question.txt","r");
  if(f==NULL){
    printf("file not exsist\n");
    return;
  }
  while(fgets(result,1024,f)!= NULL){
      char *token;
      int k=0;
      token=strtok(result,"\t");
      while(token!=NULL){
        k++;
        if(k==1){
          newquestion.id=atoi(token);
        }
        else if(k==2){
          strcpy(newquestion.ques,token);
        }
        else if(k==3){
          strcpy(newquestion.solustion,token);
          addQuestion(newquestion);
        }
       
        token=strtok(NULL,"\t");
      }
  }
  fclose(f);
}

void format_time(char *output){
    time_t rawtime;
    struct tm * timeinfo;
 
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    sprintf(output,"%d:%d\n",timeinfo->tm_min,timeinfo->tm_sec);
}

