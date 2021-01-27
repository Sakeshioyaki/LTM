#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "friend.c"

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

userInfo* searchUser(char nameUser[]){
  //can cap nhat lai danh sach user tu file
  readFile();
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

void WriteToFile(account newAccount){
  printf("dang viet vao %s %s\n",newAccount.name, newAccount.password );
  FILE *fb = fopen("account.txt", "a+");
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
  strcpy(newAccount.password, messs.mess);
  printf("%s-%s\n", newAccount.password, messs.mess);
  userInfo *user = createNewUser(newAccount);
  addUser(user);
  WriteToFile(newAccount);
  user = searchUser(newAccount.name);
  printf("Da add = %s vo danh sach ng dung\n", user->acc.name);
  return user;
}

userInfo* loginUser(int newSocket,int statususer,int statuspass){
  MESSAGE mess;
  int countuser=0;
  int countpass=0;
  FILE *f;
  userInfo* user;
  char name[100]; 
 while(statususer==0){
    MESSAGE mess = RECEVE(newSocket);
    countuser++;
    printf("countuser o day la %d\n",countuser );
    strcpy(name,mess.mess);
    user = searchUser(mess.mess); 
    if (user == NULL){
      if(countuser>=3){
        char result1[30] = "NHAP QUA SO LAN";
        SEND(newSocket,result1,mess.code);
        return user;
      }
      else{
        char result[6] = "NOT OK";
        SEND(newSocket,result,mess.code);
      }
    }else{
      statususer=1;
      char result[6] = "OK";
      SEND(newSocket,result,LOG_USERNAME);
      while(statuspass==0){
        mess=RECEVE(newSocket);
        countpass++;
        if(strcmp(mess.mess,user->acc.password)==0){
          char login[30]="login success";
          SEND(newSocket,login,LOG_PASSWORD);
          statuspass=1;
        }
        else{
          char login[30]="LOG_PASSWORD NOT OK";
          if(countpass>=3){
            char result1[30] = "NHAP QUA SO LAN";
            SEND(newSocket,result1,mess.code);
            user=NULL;
            return user;
          }
          SEND(newSocket,login,LOG_PASSWORD);
          
        }
      }
    }
  }
  return user;    
}
// userInfo* loginUser(MESSAGE mess, int newSocket,int statususer,int statuspass){
//   int countuser=0;
//   int countpass=0;
//   userInfo* user;
//   char name[100]; 
//  while(statususer==0){
//     MESSAGE mess = RECEVE(newSocket);
//     countuser++;
//     // printf("countuser o day la %d\n",countuser );
//     strcpy(name,mess.mess);
//     user = searchUser(mess.mess); 
//     // printf("user la %s pass la %s\n",mess.mess, user->acc.password);
//     if (user == NULL){
//       if(countuser>=3){
//         char result1[30] = "NHAP QUA SO LAN";
//         SEND(newSocket,result1,mess.code);
//         return user;
//       }
//       else{
//         char result[6] = "NOT OK";
//         SEND(newSocket,result,LOG_USERNAME);
//       }
//     }else{
//       statususer=1;
//       char result[6] = "OK";
//       SEND(newSocket,result,LOG_USERNAME);
//       printf("pass dung la %s\n",user->acc.password);
//       while(statuspass==0){
//         mess=RECEVE(newSocket);
//         printf("pas da nhan duoc la %s\n",mess.mess );
//         countpass++;
//         printf("pass la %s\n", user->acc.password);
//         if(strcmp(mess.mess,user->acc.password)==0){
//           printf("172 pass dung\n");
//           char login[30]="login success";
//           SEND(newSocket,login,LOG_PASSWORD);
//           statuspass=1;
//         }
//         else{
//           char login[30]="LOG_PASSWORD NOT OK";
//           if(countpass>=3){
//             char result1[30] = "NHAP QUA SO LAN";
//             SEND(newSocket,result1,mess.code);
//             user=NULL;
//             return user;
//           }
//           SEND(newSocket,login,LOG_PASSWORD);
          
//         }
//       }
//     }
//   }
//   return user;    
// }

int isFriend(char name[MAXLINE], userInfo **user){
  if((*user)->listFd == NULL){
    char fileName[MAXLINE];
    strcpy(fileName, (*user)->acc.name);
        strcat(fileName,"BAN_BE.txt\0");
        readFriendFile(fileName, &((*user)->listFd));
        if((*user)->listFd == NULL){}
          printf("khong co ban be\n");
        return 0;
  }
  listFriend *friend = searchFriend(name, (*user)->listFd);
  if(friend == NULL){
    printf("Chua ket ban voi nguoi nay %s \n", name);
    return 0;
  }else{
    printf("232: tra ve 1 ma troi \n");
    return 1;
  }
    
}

// void showListFriend(userInfo *user, int newSocket){
//   listFriend *tmp = user->listFd;
//   char name[MAXLINE];
//   char offline[MAXLINE] = "OFFLINE";
//   CHAR online[MAXLINE] = "ONLINE";
//   int i;
//   if(tmp == NULL){
//     printf("Ban khong co nguoi ban nao\n" );
//   }else{
//     for(i=1;i<=countFriend;i++){
//       printf("%d : %s\n",i,tmp->myFriend.name);
//       strcpy(name, tmp->myFriend.name);
//       SEND(newSocket,name, YC_XEM_DS_BAN_BE);
//       if(isOnline(tmp->myFriend.name) == 0){
//         printf("user %s is offline\n", tmp->myFriend.name);
//         SEND(newSocket, offline, YC_XEM_DS_BAN_BE);
//       }else{
//         printf("User %s is online\n", tmp->myFriend.name);
//         SEND(newSocket, online, YC_XEM_DS_BAN_BE);
//       }
//       tmp = tmp->next;
//       sleep(1);
//     }
//   }
// }

char* itoa(int value, char* result, int base) {
        // check that the base if valid
        if (base < 2 || base > 36) { *result = '\0'; return result; }

        char* ptr = result, *ptr1 = result, tmp_char;
        int tmp_value;

        do {
            tmp_value = value;
            value /= base;
            *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
        } while ( value );

        // Apply negative sign
        if (tmp_value < 0) *ptr++ = '-';
        *ptr-- = '\0';
        while(ptr1 < ptr) {
            tmp_char = *ptr;
            *ptr--= *ptr1;
            *ptr1++ = tmp_char;
        }
        return result;
    }