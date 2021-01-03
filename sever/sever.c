#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
// #include "question.c"
#include "user.c"
#include "game.c"
#include "userOnlineAndChatRoom.c"

#define PORT 8080
#define NOTOK "NOT OK"
#define OK "OK"
#define MAX 50 //max friend

int isTrung(int mang[],int count, int k ){
  for (int i=0;i<count;i++){
    if(k==mang[i]){
      return 1;
    }
  }
  return 0;
}
// void ganqs(QuestionList *ques,QuestionList *tmp, int i, int k){
//   int u=0;
//   while(ques!=NULL){
//     u++;
//     if(u==i){
//       ques->qs=tmp->qs;
//     }

//   }
// }

// void layngaunhien15cau(QuestionList *ques, int n, QuestionList *tmp){
//   int k;
//   int mang[15];
//   srand(time(NULL));
//   for(int i=0;i<15;i++){
//     do{
//       k=rand()%n;
//     }while(isTrung(mang,i,k)==1);
//     mang[i]=k;
//     ganqs(ques,tmp,i,k);
//   }
// }
// int chaycauhoir(QuestionList *tmp,int newSocket){
//   for(int i=0;i<15;i++){
//     system("cls");
//     SEND(newSocket,tmp->qs.ques,YC_CHOI_GAME);

//   }
// }
void randomquestion(int mang[15]){
  int k;
  srand(time(NULL));
  for(int i=0;i<15;i++){
    do{
      k=rand()%15;
    }while(isTrung(mang,i,k)==1);
    mang[i]=k;
    
  }
  
}
void sendQuestion(int newSocket,char solu[50], int random,int mang[15]){
  QuestionList* tmp = ques;
  
  while(tmp != NULL){
    if(tmp->qs.id==mang[random]){
      SEND(newSocket,tmp->qs.ques,YC_CHOI_GAME);
      strcpy(solu,tmp->qs.solustion);
      break;
    }
    tmp = tmp->next;
  }

}

void playgame(int newSocket,char time[10]){
 MESSAGE mess;
 int diem=0;
 int doi=0;
 int mang[15];
 char result[100];
 randomquestion(mang);
 play:
 if(doi>=1&&doi<=15){
  doi++;
   mess=RECEVE(newSocket);
    sendQuestion(newSocket,result,doi,mang);
     mess=RECEVE(newSocket);
     printf("doi day la %d\n", doi);
     format_time(time);
     if(strstr(result,mess.mess)!=NULL){
        char ph[20]="OK";
        diem++;
        printf("diem hien tai la %d\n",diem );
        SEND(newSocket,ph,YC_CHOI_GAME);
        goto play;
              
      }
     else{
      char ph[50]="ban da thua";
      diem--;
       printf("diem hien tai la %d\n",diem );
      SEND(newSocket,ph,YC_CHOI_GAME);
      goto play;
     }

 }
 if(doi==0){
      doi++;
      sendQuestion(newSocket,result,doi,mang);
     mess=RECEVE(newSocket);
     format_time(time);
     if(strstr(result,mess.mess)!=NULL){
        char ph[20]="OK";
        diem++;
        printf("diem hien tai la %d\n",diem );
        SEND(newSocket,ph,YC_CHOI_GAME);
        // mess=RECEVE(newSocket);
        goto play;
              
      }
     else{
      char ph[50]="ban da thua";
      diem--;
       printf("diem hien tai la %d\n",diem );
      SEND(newSocket,ph,YC_CHOI_GAME);
      goto play;
     }
 }
}
int main(int argc, char*argv[]){
  readFile();
  printListUser();
  MESSAGE mess;
  userInfo *user;
  userInfo *friend;
  char time[10];
  char notok[MAXLINE] = "NOTOK";
  char ok[MAXLINE] = "OK";

  //KHOI TAO SEVER
  int sockfd, ret;
  struct sockaddr_in serverAddr;
  int newSocket;
  struct sockaddr_in newAddr;
  socklen_t addr_size;

  //bien tam
  char fileName[MAXLINE];

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
          SEND(newSocket,ok,LOG_USERNAME);
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
            printf("dang xu li yc ket ban\n");
            friend = searchUser(mess.mess);
            if(friend == NULL){
              printf("Khong tin thay ng nay \n");
              SEND(newSocket, notok, YC_KET_BAN);
            }else{
              printf("Da tim thay nguoi dung!\n");
              strcpy(fileName, friend->acc.name);
              strcat(fileName,"YC_KET_BAN\0");
              requestFriend(user->acc.name,fileName);
              SEND(newSocket, ok, YC_KET_BAN);
              printf("da send : %s\n", ok );
            }
            break;
            case YC_CHOI_GAME :
            readfileGame();
            playgame(newSocket,time);
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
