#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void main(){
    char *s = (char*)malloc(100*sizeof(char));
    int ID_user = 2, status = 1;
    char url[]= "http://127.0.0.1:8000/api/user/";
	sprintf(s,"%s%d/?status=%d", url,ID_user, status);
    printf("%s",s);
}
