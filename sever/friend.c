#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "lib.c"


#define MAXLINE 100

int countRequestFriend = 0;
int countFriend = 0;

typedef struct friend{
  char name[MAXLINE];
}friend;

typedef struct listFriend{
	friend myFriend;
	struct listFriend * next;
}listFriend;


listFriend *createNewFriend(char name[MAXLINE]){
	friend newFriend;
	strcpy(newFriend.name,name);
	listFriend *newFd = (listFriend*)malloc(sizeof(listFriend));
	newFd->myFriend = newFriend;
	newFd->next = NULL;
	return newFd;
}

void addFriend(listFriend **newFd, listFriend **listFd){

	if((*listFd) == NULL){
		(*listFd) = (*newFd);
	}else{
		((*newFd)->next) = (*listFd);
		(*listFd) = (*newFd);
	}
}

void writeToFriendFile(char fileName[MAXLINE], listFriend *newFd){

	FILE *fb = fopen(fileName,"a+");
	fprintf(fb, "%s\n", (newFd)->myFriend.name);
	fclose(fb);	

}

listFriend *searchFriend(char name[MAXLINE], listFriend *listFd){
	if(listFd == NULL){
		printf("Chua ket ban\n");
	}
	listFriend *tmp = listFd;
	while(tmp!=NULL){
		if(strcmp(tmp->myFriend.name, name) == 0){
			printf("Da tim thay trong danh sach ban be\n");
			return tmp;
		}else{
			tmp = tmp->next;
		}
	}
	return NULL;
}

void readFriendFile(char fileName[MAXLINE], listFriend **listFd){
	countFriend = 0;
	char name[MAXLINE];
	FILE *fb;
	fb = fopen(fileName,"r");
	if(fb == NULL){
		printf("Khong co ban be \n");
		return;
	}
	while(fscanf(fb,"%s", name) != EOF){
		printf("%d\n",countFriend );
		listFriend *new = createNewFriend(name);
		countFriend++;
		addFriend(&new, listFd);
		printf("%d\n",countFriend );
	}
	fclose(fb);
}

void requestFriend(char name[MAXLINE], char fileName[MAXLINE]){
	FILE *fb;
	fb = fopen(fileName, "a+");
	fprintf(fb, "\0");
	fprintf(fb, "%s\n", name);
	fclose(fb);
}

void readRequestFriend(char fileName[MAXLINE], listFriend **requestFd){
	FILE *fb;
	char name[MAXLINE];
	fb = fopen(fileName,"r");
	if(fb == NULL){
		printf("Khong co ban be \n");
		return;
	}
	while(fscanf(fb,"%s", name) != EOF){
		listFriend *new = createNewFriend(name);
		countRequestFriend+=1;
		addFriend(&new, requestFd);
	}
	fclose(fb);
	remove(fileName);
}


void xuLyYCKetBan(int newSocket,listFriend **requestFd, listFriend **listFd, char fileName[MAXLINE], char userName[MAXLINE]){
	listFriend *tmp = (*requestFd);
	listFriend *tmp2;

	char tmpName[MAXLINE];
	char fileName2[MAXLINE];
	char notok[MAXLINE] = "NOT OK";
	char ok[MAXLINE] = "OK";
	MESSAGE mess;

	while(tmp!=NULL){
	  printf("yeu cau ket ban tu : %s\n", tmp->myFriend.name );
	  strcpy(tmpName, tmp->myFriend.name);
	  SEND(newSocket,tmpName, YC_XEM_BAN_BE);
	  printf("da sen : %s\n",tmpName);
	  mess = RECEVE(newSocket);
	  if(strcpy(mess.mess, notok)==0) {
	    printf("dang rejectFriend\n");
	    (*requestFd) = tmp->next;
	    tmp->next = NULL;
	    tmp = (*requestFd);
		countRequestFriend-=1;
	    printf("so luong ban be la %d\n", countFriend );
	    printf("so luong request friend %d\n", countRequestFriend );
	  }
	  else {
	    printf("dang accept friend %s\n", tmp->myFriend.name);
	    writeToFriendFile(fileName,tmp);
	    (*requestFd) = tmp->next;
	    tmp->next = NULL;
	    addFriend(&tmp,listFd);
	    strcpy(fileName2,tmp->myFriend.name);
	    strcpy(fileName2,"BAN_BE.txt");
	    writeToFriendFile(fileName2, tmp);
	    tmp = (*requestFd);
		countFriend+=1;
		countRequestFriend-=1;
	    printf("so luong ban be la %d\n", countFriend );
	    printf("so luong request friend %d\n", countRequestFriend );
	  }
	}
}



