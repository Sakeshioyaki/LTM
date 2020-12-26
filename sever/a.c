#include <time.h>
#include <stdio.h>
#include <string.h>
void format_time(){
    time_t rawtime;
    struct tm * timeinfo;
 
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
 
   printf("%d:%d \n", timeinfo->tm_min, timeinfo->tm_sec) ;
}
int main(void)
{
	format_time();
    // time_t mytime = time(NULL);
    // char * time_str = ctime(&mytime);
    // time_str[strlen(time_str)-1] = '\0';
    // printf("Current Time : %s\n", time_str);
 
    return 0;
}