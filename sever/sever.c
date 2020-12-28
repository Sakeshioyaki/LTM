#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include "lib.c"
#define PORT 8080
#define fileName "account.txt"

#define MAXLINE 100
#define MAX 50 //max friend
// question
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
// user
typedef struct accout{
  char name[MAXLINE];
  char password[MAXLINE];
}account;

typedef struct userInfo{
  account acc;
  int status;
  struct userInfo *next;
}userInfo;

userInfo* root = NULL;

userInfo* createNewUser(account a){
  userInfo* node = (userInfo *)malloc(sizeof(userInfo)); 
  node->acc = a;
  node->next = NULL;
  return node;
}

void addUser(account a){
  userInfo* newUser = createNewUser(a);
  if(root == NULL){
    root = newUser;
  }else{
  newUser->next = root;
  root = newUser;
  }
}

userInfo* searchUser(char nameUser[]){
  if(root == NULL){
    printf("List user is NULL !\n");
    return NULL;
  }
  userInfo* tmp = root;
  while(tmp != NULL){
    if(strcmp(tmp->acc.name, nameUser) == 0 ){
      return tmp;
    }
    tmp = tmp->next;
  }
  return NULL;
}

// void rê(List *l){
//     for(Node *k=(*l).head;k!=NULL;k=k->next){
//         (*l).head=(*l).head->next;
//         free(k);
//     }
// }

void printListUser(){
  userInfo* tmp = root;
  printf("Nguoi dung\n");
  while(tmp != NULL){
    printf("%s\n", tmp->acc.name);
    tmp = tmp->next;
  }
}

void readFile(){
  account newUser;
  FILE *fb;
  fb = fopen(fileName,"r");
  if(fb == NULL) {
    printf("File NULL !! \n");
    return;
  }
    while(fscanf(fb, "%s%s", newUser.name, newUser.password) != EOF){
      addUser(newUser);
    }
    fclose(fb);

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



//EDIT
/*
*shjfdhjfhdjfhsjfhjsfhsjfd
*/


userInfo* loginUser(MESSAGE mess, int newSocket,int statususer,int statuspass){
 while(statususer==0){
           MESSAGE mess = RECEVE(newSocket);

          userInfo* user = searchUser(mess.mess); 
          if (user == NULL){
            char result[6] = "NOT OK";
            SEND(newSocket,result,mess.code);
          }else{
            statususer=1;
            char result[6] = "OK";
            SEND(newSocket,result,LOG_USERNAME);
            while(statuspass==0){
              mess=RECEVE(newSocket);
              if(strcmp(mess.mess,user->acc.password)==0){
                char login[30]="login success";
                SEND(newSocket,login,LOG_PASSWORD);
                statuspass=1;
                return user;
              }
              else{
                char login[30]="LOG_PASSWORD NOT OK";
                SEND(newSocket,login,LOG_PASSWORD);
              }
            }


          }
        }  
  
}

void sendQuestion(int newSocket,char solu[50]){
  QuestionList* tmp = ques;
  int u=lengList();
  int k=rand()%u+1;
  while(tmp != NULL){
    if(tmp->qs.id==k){
      SEND(newSocket,tmp->qs.ques,YC_CHOI_GAME);
      strcpy(solu,tmp->qs.solustion);
      break;
    }
    tmp = tmp->next;
  }

}
// thoi gian ma client tra loi 

void format_time(char *output){
    time_t rawtime;
    struct tm * timeinfo;
 
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    sprintf(output,"%d:%d\n",timeinfo->tm_min,timeinfo->tm_sec);
}

//EDIT
/*
*jahfjdhfjshfkdshfjsd
*/
// void ghilaivaofile(userInfo *l){
//     FILE *f;
//     f=fopen("account.txt","w");
//     for(Node *k=(*l).head;k!=NULL;k=k->next){
//         fprintf(f, "%s %s %d\n",k->acc.name,k->acc.password,k->acc.status );
//     }
//     fclose(f);
// }


// void setStatus(List l,account acc, char namestatus[30],int value){
//  for(Node *k=l.head;k!=NULL;k=k->next){
//          if(strcmp(k->acc.name,namestatus)==0){
//            k->acc.status=value;
//            break;
//          }
//        }
//        ghilaivaofile(&l);
// }

int main(int argc, char*argv[]){
  readFile();
  printListUser();
  MESSAGE mess;
  userInfo *user;
  // QuestionList *ques;
  readfileGame();
  printListQuestion();
  int sockfd, ret;
  struct sockaddr_in serverAddr;

  int newSocket;
  struct sockaddr_in newAddr;

  socklen_t addr_size;

  char buffer[1024];
  pid_t childpid;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0){
    printf("[-]Error in connection.\n");
    exit(1);
  }
  printf("[+]Server Socket is created.\n");

  memset(&serverAddr, '\0', sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(PORT);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
  if(ret < 0){
    printf("[-]Error in binding.\n");
    exit(1);
  }
  printf("[+]Bind to port %d\n", 4444);

  if(listen(sockfd, 10) == 0){
    printf("[+]Listening....\n");
  }else{
    printf("[-]Error in binding.\n");
  }

  char tmp[1024];

  while(1){

    newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
      if(newSocket < 0){
        exit(1);
      }
    printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
    if((childpid=fork())==0){
      close(sockfd);
      while(1){
        int statususer=0;
        int statuspass=0;
        char time[10];
        printf("bat dau ket noi !\n");
        char nameUser[256], password[30]; 
        // int diem=0;      
        // conti:   mess = RECEVE(newSocket);
        // if(mess.code == LOG_USERNAME){
          user=loginUser(mess,newSocket,statususer,statuspass);
        // }
        // else if(mess.code == 1){
        //   char result[50];
        //  sendQuestion(newSocket,result);
        //  mess=RECEVE(newSocket);
        //  format_time(time);
        //  printf("time: %s\n", time);
        //  if(strstr(result,mess.mess)!=NULL){
        //   char ph[20]="OK";
        //   diem++;
        //   printf("diem hien tai la %d\n",diem );
        //   SEND(newSocket,ph,YC_CHOI_GAME);
        //   goto conti;
          
        //  }
        //  else{
        //   char ph[50]="ban da thua";
        //   diem--;
        //    printf("diem hien tai la %d\n",diem );
        //   SEND(newSocket,ph,YC_CHOI_GAME);
        //   goto conti;
        //  }
        // }
        
      }
    }

}
  close(newSocket);


  return 0;
  
}
