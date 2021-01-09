#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "question.c"
#include "userOnlineAndChatRoom.c"

#define PORT 8080
#define NOTOK "NOT OK"
#define OK "OK"
#define MAX 50 //max friend

int main(int argc, char*argv[]){
  readFile();
  printListUser();

  char notok[MAXLINE] = "NOTOK";
  char ok[MAXLINE] = "OK";

  //KHOI TAO SEVER
  int sockfd, ret, i;
  struct sockaddr_in serverAddr;
  int newSocket;
  struct sockaddr_in newAddr;
  socklen_t addr_size;

  char buffer[BUFFER_SZ];
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
        char fileNameFriend[MAXLINE];
        char count[MAXLINE];
        int count2;
          //bien tam
        char fileName[MAXLINE];
        char tmpName[MAXLINE];
        char tmp[MAXLINE];
        int leave_flag;
        int receive;
        Client_t *cli = (Client_t *)malloc(sizeof(Client_t));

        MESSAGE mess;

        userInfo *user;
        userInfo *friend;

        listFriend *tmp1;
        printf("bat dau ket noi !\n");
        char nameUser[256], password[30];       
        mess = RECEVE(newSocket);
        switch(mess.code){

          /*
          *LOGIN
          */
          case LOG_USERNAME:
            user=loginUser(mess,newSocket,statususer,statuspass);
            if(user != NULL){
            cli->address = newAddr;
            cli->sockfd = newSocket;
            cli->id = client_count+1;
            strcpy(cli->name, user->acc.name);
            // Add client to queue
            add_client(cli);
            user->status = 1;
            }
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
              if(user != NULL){
                cli->address = newAddr;
                cli->sockfd = newSocket;
                cli->id = client_count+1;
                strcpy(cli->name, user->acc.name);
                // Add client to queue
                add_client(cli);
                user->status = 1;
              }
            } 
            break;

          /*
          *REQUEST FRIEND
          */
          case YC_KET_BAN :
            cli->status = YC_KET_BAN;
            printf("dang xu li yeu cau ket ban\n");
            printf("Dang yeu cau ket ban voi %s\n", mess.mess);
            friend = searchUser(mess.mess);
            if(friend == NULL){
              printf("Khong tin thay ng nay \n");
              SEND(newSocket, notok, YC_KET_BAN);
            }else{
              printf("Da tim thay nguoi dung!\n");
              strcpy(fileName, friend->acc.name);
              strcat(fileName,"YC_KET_BAN.txt\0");
              requestFriend(user->acc.name,fileName);
              SEND(newSocket, ok, YC_KET_BAN);
              printf("da send : %s\n", ok );
            }
            break;

           /*
           *XEM YEU CAU KET BAN
           */ 
          case YC_XEM_BAN_BE:
            cli->status = YC_XEM_BAN_BE;
            printf("Client yeu cau xem yeu cau ket ban\n");
            readRequestFriend(fileNameFriend, &(user->requestFd));
            sprintf(count,"%d",countRequestFriend);            
            strcpy(fileNameFriend, user->acc.name);
            strcat(fileNameFriend,"YC_KET_BAN.txt\0");
            SEND(newSocket, count, YC_XEM_BAN_BE);
            strcpy(fileName, user->acc.name);
            strcat(fileName,"BAN_BE.txt\0");
            tmp1 = user->requestFd;
            count2 = countRequestFriend;
            for(i = 1; i<=count2; i++){
              printf("yeu cau ket ban tu : %s\n", tmp1->myFriend.name );
              strcpy(tmpName, tmp1->myFriend.name);
              SEND(newSocket,tmpName, YC_XEM_BAN_BE);
              mess = RECEVE(newSocket);
              if(strcpy(mess.mess, notok)==0) {
                printf("dang rejectFriend\n");
                rejectFriend(tmp1->myFriend.name, &(user->requestFd));
                printf("so luong ban be la %d\n", countFriend );
                printf("so luong request friend %d\n", countRequestFriend );
              }
              else {
                printf("dang accept friend %s\n", tmp1->myFriend.name);
                acceptFriend(tmp1->myFriend.name,&(user->requestFd), &(user->listFd), fileName, user->acc.name);
                printf("so luong ban be la %d\n", countFriend );
                printf("so luong request friend %d\n", countRequestFriend );
              }
              tmp1= tmp1->next;
            }
            break;

            /*
            *YEU CAU XEM DANH SACH BAN BE   
            */
          case YC_XEM_DS_BAN_BE:
            cli->status = YC_XEM_DS_BAN_BE;
            strcpy(fileName, user->acc.name);
            strcat(fileName,"BAN_BE.txt\0");
            readFriendFile(fileName, &(user->listFd));
            sprintf(count,"%d",countFriend);            
            sprintf(count,"%d",countRequestFriend);            
            strcpy(fileNameFriend, user->acc.name);
            SEND(newSocket, count, YC_XEM_DS_BAN_BE);
            break;

          /*
          * CHAT WITH FRIEND
          */
          case CHAT:
            printf("so nguoi dang online :%d\n", client_count );
            printf("yeu cau chat voi %s\n", mess.mess);
            friend = searchUser(mess.mess);
            if(friend != NULL){
              if(friend->status == 1){
                printf("User offline\n");
              }else{
                //kiem tra xem phai ban be k 
                if(isFriend(mess.mess, &user) ==1){
                  SEND(newSocket, ok, CHAT);
                  cli->status = CHAT;
                  //kiem tra xem co online ko
                  if(isOnline(friend->acc.name) == 0){
                    cli->fdId = isOnline(friend->acc.name);
                    printf("user hien tai dang offline");
                    SEND(newSocket,notok,CHAT);
                    while(1){
                      if (leave_flag)
                          break;
                      receive = recv(cli->sockfd, buffer, BUFFER_SZ, 0);
                      if (receive > 0){
                        send_message(buffer, cli);
                        str_trim_lf(buffer, strlen(buffer));
                        printf("%s\n", buffer);
                      }
                      else if (receive == 0 || strcmp(buffer, "exit") == 0){
                        sprintf(buffer, "%s has left\n", cli->name);
                        printf("%s", buffer);
                        cli->fdId = 0;
                        leave_flag = 1;
                      }
                      else{
                          printf("ERROR: -1\n");
                          leave_flag = 1;
                      }
                      bzero(buffer, BUFFER_SZ);
                      }
                    }
                  }else{
                    printf("Khong phai ban be \n");
                    strcpy(tmp,"NOT FRIEND");
                    SEND(newSocket, notok, CHAT);
                  }
                }
              }else{
                SEND(newSocket, notok, CHAT);
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