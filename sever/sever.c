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
#include "game.c"

#define PORT 8080
#define NOTOK "NOT OK"
#define OK "OK"
#define MAX 2048 //max friend

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
void randomquestion(int mang[15]){
  int k;
  srand(time(NULL));
  for(int i=0;i<15;i++){
    do{
      k=rand()%15;
    }while(isTrung(mang,i+1,k)==1);
    mang[i]=k;
    
  }
  for(int i=0;i<15;i++){
    printf("%d  ",mang[i] );
  }
  
}
void sendQuestion(int newSocket,char solu[50], int random,int mang[15]){
  QuestionList* tmp = ques;
  
  while(tmp != NULL){
    if(tmp->qs.id==mang[random]){
      printf("question gui di la %s\n",tmp->qs.ques );
      SEND(newSocket,tmp->qs.ques,YC_CHOI_GAME);
      // return tmp->qs.ques;
      strcpy(solu,tmp->qs.solustion);
      break;
    }
    tmp = tmp->next;
  }

}


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

void playgame(int newSocket,char time[10]){
 MESSAGE mess;
 int diem=0;
 int doi=0;
 clock_t start,end;
 double time_used;
 int mang[15];
 char result[100];
 randomquestion(mang);
 play:
 if(doi>=0&&doi<15){
      start=clock();
      sendQuestion(newSocket,result,doi,mang);
      doi++;
     mess=RECEVE(newSocket);
     end=clock();
     time_used=((double)(end-start))/CLOCKS_PER_SEC;
     printf("thoi gian su duong de tra loi la %f\n", time_used);
     // format_time(time);
     
     if(strstr(strlwr(result),strlwr(mess.mess))!=NULL){
        char ph[20]="OK";
        diem++;
        printf("diem hien tai la %d\n",diem );
        SEND(newSocket,ph,YC_CHOI_GAME);
        mess=RECEVE(newSocket);
        printf("mess nhan duoc la %s\n",mess.mess );
        goto play;
              
      }
     else{
      char ph[50]="ban da thua";
      diem--;
       printf("diem hien tai la %d\n",diem );
      SEND(newSocket,ph,YC_CHOI_GAME);
      mess=RECEVE(newSocket);
        printf("mess nhan duoc la %s\n",mess.mess );
      goto play;
     }
 }
 else{
     char end[100]="END score: ";
     char a[5];
     itoa(diem,a,10);
     strcat(end,a);
     SEND(newSocket,end,YC_CHOI_GAME);
 }

}
// void createfilegame()

 
int main(int argc, char*argv[]){
  readFile();
  printListUser();
  pthread_t msg;
  MESSAGE mess;
  char time[10];
  int ko;
  userInfo *user;
  userInfo *friend;
  // char fileNameChat
  listFriend *tmp1;
  char notok[MAXLINE] = "NOT OK";
  char ok[MAXLINE] = "OK";
  int countuse=0;
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
        char notok[MAXLINE] = "NOTOK";
        char ok[MAXLINE] = "OK";
        int statususer=0, i;
        int statuspass=0;
        char fileNameFriend[MAXLINE];
        char fileNameChat[100];
        char fileNameMess[100];
        char fileNameMessReturn[100];
        char count[MAXLINE];
        int count2;
        char fileNamegame[100];
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
        bd:
        mess = RECEVE(newSocket);
        
        printf("SO LUONG USER DANG ONLINE : %d\n", client_count);
        switch(mess.code){

          /*
          *LOGIN
          */
          case LOG_USERNAME:
            user=loginUser(mess,newSocket,statususer,statuspass);
            if(user != NULL){
            printf("126: user name : %s\n", user->acc.name);
            strcpy(cli->name, user->acc.name);
            cli->sockfd = newSocket;
            cli->id = id++;
            add_client(cli);

            // Add client to queue
            user->status = 1;

            }
            else{
              printf("khong duoc them vaof haha\n");
            }
            break;

          /*
          *SIGN UP
          */
          case SIGN_UP_USERNAME:
          
            printf("Dang tao tai khoan\n");
            user = searchUser(mess.mess);
            if(user != NULL){
              countuse++;
              if(countuse>=3){
                char tmp1[10]="NHAP QUA";
                SEND(newSocket, tmp1, SIGN_UP_USERNAME);
                goto bd;
              }
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

            /* GAME VOI MAY*/
            case YC_CHOI_GAME:
            readfileGame();
            playgame(newSocket,time);
            break;
            // case PLAY_GAME_WITH_FD:
            // strcpy(fileNamegame,mess.mess);
            // strcat(fileNamegame,".txt");
            // break;




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
              SEND(newSocket,notok,YC_KET_BAN);
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
            tmp1 = user->requestFd;
            count2 = countRequestFriend;
            for(i = 1; i<=count2; i++){
              printf("yeu cau ket ban tu : %s\n", tmp1->myFriend.name );
              strcpy(tmpName, tmp1->myFriend.name);
              printf("chuoi ten gui di la %s\n",tmpName );
              SEND(newSocket,tmpName, YC_XEM_BAN_BE);
              mess = RECEVE(newSocket);
              printf("tmp1->myFriend.name o day la %s\n", tmp1->myFriend.name);
              printf("mess.mess o day ok hay khong la %s\n", mess.mess);
              if(strcmp(mess.mess, notok)==0) {
                printf("dang rejectFriend\n");
                // rejectFriend(tmp1->myFriend.name, &(user->requestFd));
                printf("so luong ban be la %d\n", countFriend );
                printf("so luong request friend %d\n", countRequestFriend );
              }
              else {
                printf("dang accept friend %s\n", tmp1->myFriend.name);
                // acceptFriend(tmp1->myFriend.name,&(user->requestFd), &(user->listFd), fileName, user->acc.name);
                printf("so luong ban be la %d\n", countFriend );
                printf("so luong request friend %d\n", countRequestFriend );
              }
              tmp1= tmp1->next;
            }
            char endfriend[10]="END";
            SEND(newSocket,endfriend,YC_XEM_BAN_BE);
            break;

            case CHAT:
            printf("mess.mess nhan duoc la %s\n",mess.mess );
            printf("client yeu cau xem tin nhan \n");
            FILE *f;
            char messages[MAXLINE];
            char messages1[MAXLINE];
            char fileNameChat1[100];
            strcpy(fileNameChat1,user->acc.name);
            strcat(fileNameChat1,"CHAT.txt\0");
            f=fopen(fileNameChat1,"r");
            if(f==NULL){
              printf("khong co file nhes\n");
              // fclose(f);
              printf("ra day de goto chuwa \n");
              goto tieptucchonban;
            }
            printf("lai ra day af\n");
            while(fgets(messages,1024,f)!=NULL){
              printf("mess la %s\n",messages );
              strcpy(messages1,messages);
              SEND(newSocket,messages,PHAN_HOI_CHAT);
              char *token=strtok(messages1,"->");
              strcpy(fileNameMessReturn,token);
              strcat(fileNameMessReturn,"CHAT.txt\0");
              mess= RECEVE(newSocket);
              writechatfile(fileNameMessReturn,user->acc.name,mess.mess);
            }
            remove(fileNameChat1);
            fclose(f);
            
            tieptucchonban:
            printf("nhay xuong day roi \n");
            char phanhoi[100]="khong co tin nhan nao";
            SEND(newSocket,phanhoi,CHAT);
            printf("chuoi gui di la %s\n",phanhoi );
            char namefriends[100];
            mess=RECEVE(newSocket);
            printf("nguoi muon chat cung nhan duoc ben server ;a %s\n",mess.mess );
            strcpy(fileName, user->acc.name);
            strcat(fileName,"BAN_BE.txt\0");
            strcpy(namefriends,mess.mess);
            int k=readFriendFilesearchFri(fileName,mess.mess);
            if(k==0){
              
              printf(" khong la ban be dau nhes\n");
              SEND(newSocket,notok,CHAT);
            }
            else{
              printf("%s la ban be cua %s\n",mess.mess,user->acc.name );
              SEND(newSocket,ok,CHAT);
              mess=RECEVE(newSocket);
              strcpy(fileNameChat, namefriends);
              strcat(fileNameChat,"CHAT.txt\0");
              printf("%s -> %s\n",user->acc.name,mess.mess );
              writechatfile(fileNameChat,user->acc.name,mess.mess);
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
            printf("so luong ban be la : %d\n",countFriend);         
            strcpy(fileNameFriend, user->acc.name);
            SEND(newSocket, count, YC_XEM_DS_BAN_BE);
            showListFriend(user, countFriend, newSocket);
            break;
          case SIGN_OUT:
          printf("chuoi nhan duoc la %s\n", mess.mess);
            // removeUserOnline(user->acc.name);
            break;
          case PLAY_GAME_WITH_FD:

          mess=RECEVE(newSocket);
         ko= isOnline(mess.mess);
         int online;
         online=readFileonline();
         printf("so nguoi onl trong file la %d\n",online );
         printf("so user onl hien tai %d\n", client_count);
         char on[10]="online";
         if(ko!=0){
          printf("usser online\n");
          int wrt_status=write(ko,on,strlen(on));
         }
         else{
          printf("sai\n");
         }
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
            for(int i=0;i<numberquestion;i++){
              int c=mang[i];
              int k=snprintf(messSend,sizeof(messSend),"Cau hoi %d: %s\nA. %s\nB. %s\nC. %s\nD. %s\n ",i+1,Question[c].question,Question[c].A,Question[c].B,Question[c].C,Question[c].D);
              printf("%s\n",messSend );
              int mama;
              mama=send(newSocket,messSend,strlen(messSend),0);
              if(mama<=0){
                printf("send error %d\n",mama);
                printf("send lai");
                send(newSocket,messSend,strlen(messSend),0);
                exit(-1);
              }
              recv(newSocket,solution,10,0);

              if(strcmp(Question[c].TrueAnswer,solution)==0){
                char trueqs[50]="ban tra loi dung";
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
            case PLAY_GAME_WITH_FD:
            break;

          default:
            break;
        }
      }
    }
