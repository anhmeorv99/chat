#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

char * convert_timestamp_to_date(char* timestamp){
    time_t rawtime = (time_t)atoi(timestamp);
    struct tm  ts;
    char  *buf = (char*)malloc(100*sizeof(char));
                        // Format time, "ddd yyyy-mm-dd hh:mm:ss zzz"
    ts = *localtime(&rawtime);
    strftime(buf, 100*sizeof(buf), "%Y-%m-%d %H:%M:%S", &ts);
    return buf;
}

int main(void)
{
    char time_str[] = "1609729427";
    printf("time : %s\n", convert_timestamp_to_date(time_str));
    
}