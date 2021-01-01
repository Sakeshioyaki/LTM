#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "lib.c"
#include "friend.c"
#define MAXLINE 100

typedef struct accout{
  char name[MAXLINE];
  char password[MAXLINE];
}account;

//status = 0 : offline
//status = 1 : online
typedef struct userInfo{
  account acc;
  int status;
  listFriend *listFd;
  listFriend *requestFd;
  struct userInfo *next;
}userInfo;

userInfo* root = NULL;

userInfo* createNewUser(account a){
  userInfo* node = (userInfo *)malloc(sizeof(userInfo)); 
  node->acc = a;
  node->next = NULL;
  node->listFd = NULL;
  return node;
}

void addUser(userInfo* newUser){
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
  fb = fopen("account.txt","r");
  if(fb == NULL) {
    printf("File NULL !! \n");
    return;
  }
    while(fscanf(fb, "%s%s", newUser.name, newUser.password) != EOF){
      addUser(createNewUser(newUser));
    }
    fclose(fb);

}

void WriteToFile(account newAccount){
  printf("dang viet vao %s %s\n",newAccount.name, newAccount.password );
  FILE *fb = fopen("account.txt", "a+");
  fprintf(fb, "\0");
  fprintf(fb, "%s\t%s\t\n", newAccount.name, newAccount.password);
  fclose(fb);
}

userInfo *singUp(MESSAGE mess,int newSocket){
  account newAccount;
  char tmp[10] = "OK";
  SEND(newSocket,tmp, SIGN_UP_PASSWORD);
  MESSAGE messs = RECEVE(newSocket);
  printf("pass :%s\n", messs.mess);
  strcpy(newAccount.name, mess.mess);
  printf("User name dang ki : %s\n",newAccount.name);
  printf("what the name : %s-%s\n", newAccount.name,mess.mess );
  strcpy(newAccount.password, messs.mess);
  printf("%s-%s\n", newAccount.password, messs.mess);
  userInfo *user = createNewUser(newAccount);
  printf("tai day : %s %s\n", newAccount.name, newAccount.password);
  addUser(user);
  WriteToFile(newAccount);
  printf("ok \n");
  return user;
}

userInfo* loginUser(MESSAGE mess, int newSocket,int statususer,int statuspass){
 while(statususer==0){
    MESSAGE mess = RECEVE(newSocket);
    userInfo* user = searchUser(mess.mess); 
    if (user == NULL){
      char result[6] = "NOT OK";
      SEND(newSocket,result,mess.code);
      return NULL;
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
          return NULL;
        }
      }
    }
  }    
}