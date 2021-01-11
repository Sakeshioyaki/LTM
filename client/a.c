#include <stdio.h>
#include<string.h>

int main(){
	int i=1;
	char tmp[100];
	int sn = snprintf(tmp, sizeof(tmp), "Câu hỏi %d: %s\n ", i+1,"haha");
	printf("%s\n",tmp );
}





