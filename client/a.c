#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
int isTrung(int mang[],int count, int k ){
  for (int i=0;i<count;i++){
    if(k==mang[i]){
      return 1;
    }
  }
  return 0;
}
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
int main(){
	// srand(time(NULL));
	// int res = rand()%16;
	// printf("%d\n",res );
	// int k;
  int mang[15];
  randomquestion(mang);
  printf("day in ra mang\n");
  for(int i=0;i<15;i++){
  	printf("%d ",mang[i] );
  }
}