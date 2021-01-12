#include <stdio.h>
#include<string.h>

int main(){
	double k;
	k=(double)18/60-12/60;
	printf("%f\n",k );

}



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
              printf("thoi gian luc gui la %02d\n",tm.tm_sec );
              double ku1=(double)tm.tm_sec/60;
              printf("ku1 la %f\n",ku1 );
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
              double ku2=(double)tm1.tm_sec/60;
              printf("ku2 la %f\n",ku2);
              printf("thoi gian luc nhaan la %02d\n",tm1.tm_sec );
              // double ku=(tm1.tm_sec/60)-(tm.tm_sec/60);
              double ku=ku2-ku1;
              if(ku<0){
                ku=1+ku;
                printf("gia tri cua ku la %f moi vao day\n", ku);
              }
              double timesolution=ku*60;
              printf("thoi gian tra loi la %f\n",timesolution );
              if(strcmp(Question[c].TrueAnswer,solution)==0){
                char trueqs[50]="ban tra loi dung";
                char thongbao[MAXLINE];
                if(timesolution>10){
                  sprintf(thongbao,"ban tra loi dung. Thoi gian tra loi %f >10s.khong duoc chap nhan",timesolution);
                  send(newSocket,thongbao,strlen(thongbao)+1,0);
                }
                sprintf(thongbao,"ban tra loij dung!!Times: %f",timesolution);
                trueQuestion++;
                send(newSocket,thongbao,strlen(thongbao)+1,0);
              }
              else{
                char falsequs[50]="tra loi sai!Dap an dung la ";
                strcat(falsequs,Question[c].TrueAnswer);
                send(newSocket,falsequs,strlen(falsequs),0);


              }
              strcpy(messSend,"");
          }



