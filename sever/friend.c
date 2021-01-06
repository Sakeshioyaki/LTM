#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
		countFriend++;
		addFriend(&new, listFd);
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


void acceptFriend(char name[MAXLINE], listFriend **requestFd, listFriend **listFd, char fileName[MAXLINE]){
	if(strcmp((*requestFd)->myFriend.name,name) == 0){
		addFriend(requestFd,listFd);
		countFriend+=1;
		countRequestFriend-=1;
		writeToFriendFile(fileName,(*requestFd));
		(*requestFd) = NULL;
		return;
	}
	listFriend *tmp = *requestFd;
	printf("wtf : %s\n",tmp->myFriend.name);
	listFriend *tmp2 = NULL;
	while(tmp->next != NULL){
		if(strcmp(tmp->next->myFriend.name, name) == 0 ){
			tmp2 = tmp->next;
			tmp->next = tmp->next->next;
			addFriend(&tmp2,listFd);
			countFriend+=1;
			countRequestFriend-=1;
			writeToFriendFile(fileName,tmp2);
			return;
		}
		tmp = tmp->next;
	}

	return;
}

void rejectFriend(char name[MAXLINE], listFriend **requestFd){
	if(*requestFd == NULL){
		printf("ko co yc ket ban nay \n");
	}
	listFriend *tmp = *requestFd;
	listFriend *tmp2 = NULL;
	while(tmp != NULL){
		if(strcmp(tmp->next->myFriend.name, name) == 0 ){
			tmp2 = tmp->next;
			tmp->next = tmp->next->next;
			countRequestFriend--;
			free(tmp2);
			return;
		}
		tmp = tmp->next;
	}
}



