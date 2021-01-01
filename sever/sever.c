#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "lib.c"
#include "user.c"
#include "question.c"
#include "userOnlineAndChatRoom.c"

#define PORT 8080
#define fileName "account.txt"
#define NOTOK "NOT OK"
#define OK "OK"
#define MAXLINE 100
#define MAX 50 //max friend

int main(int argc, char*argv[]){
  readFile();
  printListUser();
  MESSAGE mess;
  userInfo *user;
  userInfo *friend;

  //KHOI TAO SEVER
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

  /*
  *Bat dau ket noi
  */
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
        printf("bat dau ket noi !\n");
        char nameUser[256], password[30];       
        mess = RECEVE(newSocket);
        printf("Thong tin mess nhan dc %s - %d\n",mess.mess, mess.code );

        switch(mess.code){

          /*
          *LOGIN
          */
          case LOG_USERNAME:
          user=loginUser(mess,newSocket,statususer,statuspass);
            break;

          /*
          *SIGN UP
          */
          case SIGN_UP_USERNAME:
            printf("Dang tao tai khoan\n");
            user = searchUser(mess.mess);
            if(user != NULL){
              printf("Da ton tai ng nay trong he thong\n");
              char tmp[10] = "NOT OK";
              SEND(newSocket, tmp, SIGN_UP_USERNAME);
              continue;
            }else{
              user = singUp(mess, newSocket);
            } 
            break;

          /*
          *REQUEST FRIEND
          */
          case YC_KET_BAN :
            mess = RECEVE(newSocket);
            friend = searchUser(mess.mess);
            char resrult[MAXLINE] = "NOT OK"
            if(friend == NULL){
              printf("Khong tin thay ng nay \n");
              SEND(newSocket, resrult, YC_KET_BAN);
            }else{
              printf("Da tim thay nguoi dung!\n");
            }
            break;
          default:
            break;
        }
      }
    }
  }
  close(newSocket);


  return 0;
  
}
