#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h> 
#include <ctype.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <unistd.h> 
#include <pthread.h>
#include <errno.h> 
#include <time.h> 
#include "question.c"
#include "userOnlineAndChatRoom.c"

#define PORT 8080
#define NOTOK "NOT OK"
#define OK "OK"
#define MAX 2048 //max friend

int main(int argc, char*argv[]){
  readFile();
  printListUser();

  char notok[MAXLINE] = "NOTOK";
  char ok[MAXLINE] = "OK";

  //KHOI TAO SEVER
  int sockfd, ret, i,opt= 1;
  struct sockaddr_in serverAddr;
  int newSocket;
  struct sockaddr_in newAddr;
  socklen_t addr_size;

  pthread_t tid;
  pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

  void *MAIN(void *socketfd);

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

  if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt))<0){
    perror("setsockopt() fail");
    close(sockfd);
    exit(-1);
  }


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
      perror("Connect error");
      continue;
    }
    printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
      pthread_mutex_lock(&clients_mutex);
      pthread_create(&tid,NULL, &MAIN, &newSocket);
      pthread_mutex_unlock(&clients_mutex);
    }
    close(newSocket);
    return 0;
  }

void *MAIN(void *socketfd){
  int newSocket = *((int*)socketfd); 
      while(1){
        char notok[MAXLINE] = "NOT OK";
        char ok[MAXLINE] = "OK";
        int statususer=0, i;
        int statuspass=0;
        char fileNameFriend[MAXLINE];
        char count[MAXLINE];
        char fileNameMessReturn[100];
        int count2;
          //bien tam
        char fileName[MAXLINE];
        char buffer[BUFFER_SZ];
        char tmpName[MAXLINE];
        char tmp[MAXLINE];
        int leave_flag;
        int receive;

        MESSAGE mess;

        userInfo *user;
        userInfo *friend;
        Client_t *cli = (Client_t *)malloc(sizeof(Client_t));

        listFriend *tmp1;
        printf("bat dau ket noi !\n");
        char nameUser[256], password[30];       
        mess = RECEVE(newSocket);

        
        printf("SO LUONG USER DANG ONLINE : %d\n", client_count);
        printListUserOnline();

        switch(mess.code){

          /*
          *LOGIN
          */
          case LOG_USERNAME:
          SEND(newSocket,ok,LOG_USERNAME);
            user=loginUser(newSocket,statususer,statuspass);
            if(user != NULL){
            printf("126: user name : %s\n", user->acc.name);
            strcpy(cli->name, user->acc.name);
            cli->sockfd = newSocket;
            cli->id = id++;
            add_client(cli);

            // Add client to queue
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
                strcpy(cli->name, user->acc.name);
                cli->sockfd = newSocket;
                cli->id = id++;
                add_client(cli);

                // Add client to queue
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
            strcpy(fileNameFriend, user->acc.name);
            strcat(fileNameFriend,"YC_KET_BAN.txt\0");
            readRequestFriend(fileNameFriend, &(user->requestFd));
            sprintf(count,"%d",countRequestFriend);
            SEND(newSocket, count, YC_XEM_BAN_BE);
            strcpy(fileName, user->acc.name);
            strcat(fileName,"BAN_BE.txt\0");            
            xuLyYCKetBan(newSocket,&(user->requestFd), &(user->listFd), fileName, user->acc.name);
            break;

            /*
            *YEU CAU XEM DANH SACH BAN BE   
            */
          case YC_XEM_DS_BAN_BE:
            printf("Dang xem danh sách bạn bè \n");
            cli->status = YC_XEM_DS_BAN_BE;
            strcpy(fileName, user->acc.name);
            strcat(fileName,"BAN_BE.txt\0");
            readFriendFile(fileName, &(user->listFd));
            sprintf(count,"%d",countFriend);   
            printf("so luong ban be la : %d\n",countFriend);         
            strcpy(fileNameFriend, user->acc.name);
            SEND(newSocket, count, YC_XEM_DS_BAN_BE);
            showListFriend(user, newSocket);
            break;

          /*
          * CHAT WITH FRIEND
          */
          case CHAT:
            cli->status = CHAT;
            printf("so nguoi dang online :%d\n", client_count );
            printf("yeu cau chat voi %s\n", mess.mess);
            friend = searchUser(mess.mess);
            if(friend != NULL){
                //kiem tra xem phai ban be k 
              if(isFriend(mess.mess, &user) ==1){
                SEND(newSocket, ok, CHAT);
                cli->status = CHAT;
                //kiem tra xem co online ko
                if(isOnline(friend->acc.name) == 0){
                  cli->fdId = isOnline(friend->acc.name);
                  printf("user hien tai dang offline");
                  SEND(newSocket,notok,CHAT);
                  }
                }else{
                  printf("Khong phai ban be \n");
                  strcpy(tmp,"NOT FRIEND");
                  SEND(newSocket, notok, CHAT);
                }
              }else{
                SEND(newSocket, notok, CHAT);
                  }
            break;

          case PLAY_GAME_WITH_SEVER:
            printf("dang choi game vs may\n");
            cli->status = YC_XEM_DS_BAN_BE;
             int mang[numberquestion];
            readQuestion();
            randomquestion(mang);
            char solution[10];
            char messSend[MAX];
            char messQues[MAXLINE+1];
            int trueQuestion=0;
            printf("208>>numberquestion la %d\n",numberquestion );
            for(int i=0;i<numberquestion;i++){
              int c=mang[i];
              printf(">>>>>>>>Cau hoi %d: %s\nA. %s\nB. %s\nC. %s\nD. %s\n ",i+1,Question[c].question,Question[c].A,Question[c].B,Question[c].C,Question[c].D);
              int k=snprintf(messSend,sizeof(messSend),"Cau hoi %d: %s\nA. %s\nB. %s\nC. %s\nD. %s\n ",i+1,Question[c].question,Question[c].A,Question[c].B,Question[c].C,Question[c].D);
              printf("strlen chuoi gui  %ldla\n", strlen(messSend));
              int mama; 
               time_t t = time(NULL);
              struct tm tm = *localtime(&t);
              double ku1=(double)tm.tm_sec/60;
              mama=send(newSocket,messSend,strlen(messSend),0);
              if(mama<=0){
                printf("send error %d\n",mama);
                printf("send lai");
                send(newSocket,messSend,strlen(messSend),0);
                exit(-1);
              }
              recv(newSocket,solution,10,0);
              time_t t1 = time(NULL);
              struct tm tm1 = *localtime(&t1);
              double ku2=(double)tm.tm_sec/60;
              double ku=ku2-ku1;
              double timesolution=ku*60;

              if(strcmp(Question[c].TrueAnswer,solution)==0){
                // char u[]
                char trueqs[50]="ban tra loi dung. Times: ";
                // strcat(trueqs,(char)timesolution);
                trueQuestion++;
                send(newSocket,trueqs,strlen(trueqs)+1,0);
              }
              else{
                char falsequs[50]="tra loi sai!Dap an dung la ";
                strcat(falsequs,Question[c].TrueAnswer);
                send(newSocket,falsequs,strlen(falsequs),0);


              }
              strcpy(messSend,"");
            }
            break;
            case MESS:
              cli->status = MESS;
              printf("mess nhan duoc la %s\n",mess.mess );
              printf("client yeu cau xem tin nhan \n");
              FILE *f;
              char messages[MAXLINE];
              char messages1[MAXLINE];
              char fileNameChat[100];
              char fileNameChat1[100];
              char phanhoi[100]="khong co tin nhan nao ";
              strcpy(fileNameChat1,user->acc.name);
              strcat(fileNameChat1,"CHAT.txt");
              printf("ten file chat de doc la %s\n",fileNameChat1 );
              f=fopen(fileNameChat1,"r");
              if(f==NULL){
                printf("not exsits file\n");
                goto tieptucchonban;
              }
              while(fgets(messages,1024,f)!=NULL){
                printf("mess la %s\n", messages);
                strcpy(messages1,messages);
                SEND(newSocket,messages,PHAN_HOI_CHAT);
                char *token=strtok(messages1,"->");
                strcpy(fileNameMessReturn,token);
                strcat(fileNameMessReturn,"CHAT.txt");
                mess=RECEVE(newSocket);
                writechatfile(fileNameMessReturn,user->acc.name,mess.mess);
              }
            break;
          case MESS:
            SEND(newSocket,phanhoi,MESS);
            printf("chuoi gui di la %s\n",phanhoi );
            char namefriends[100];
            mess=RECEVE(newSocket);
            printf("nguoi muon chat cung nhan duoc ben server la %s\n",mess.mess );
            // strcpy(fileName,user->acc.name);
            // strcat(fileName,"BANBE.txt\0");
            strcpy(namefriends,mess.mess);
            int cho=isFriend(mess.mess,&user);
            if(cho==0){
              printf("%s khong la ban be cua %s\n",user->acc.name,mess.mess);
              SEND(newSocket,notok,MESS);

            }
            else{
              printf("%s la ban be cua %s\n",mess.mess,user->acc.name );
              SEND(newSocket,ok,MESS);
              mess=RECEVE(newSocket);
              strcpy(fileNameChat,namefriends);
              strcat(fileNameChat,"CHAT.txt");
              printf("file fileNameChat la %s\n",fileNameChat );
              printf("%s -> %s\n",user->acc.name,mess.mess );
              writechatfile(fileNameChat,user->acc.name,mess.mess);
          break;

          /*
          * LOGOUT
          */
          case SIGN_OUT:
            printf(" User %s exit.....!!\n", user->acc.name);
            user = NULL;
            remove_client(cli->name);
            printf("So user dang online hien tai %d\n", client_count);
            break;

          default:
            break;
        }
      }
    }
