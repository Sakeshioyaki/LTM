#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLINE 100

typedef struct friend{
  char name[MAXLINE];
}friend;

typedef struct listFriend{
	friend myFriend;
	struct listFriend * next;
}listFriend;


listFriend *createNewFriend(char name[MAXLINE]){
	friend newFriend = (friend*)malloc(sizeof(friend));
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
	fprintf(fb, "\0");
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
	char name[MAXLINE];
	FILE *fb;
	fb = fopen(fileName,"r");
	if(fb == NULL){
		printf("Khong co ban be \n");
		return;
	}
	while(fscanf(fb,"%s", name) != EOF){
		listFriend *new = createNewFriend(name);
		addFriend(&new, listFd);
	}
}

void requestFriend(char name[MAXLINE]){

}

