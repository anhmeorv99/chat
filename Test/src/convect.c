#include <stdio.h>
#include <glib.h>

int main(){
	gchar str1[] = "13124" ;
	char str2[] = "anh tao";
	//printf("%s\n",str1);
	
	g_stpcpy(str2,str1);
	g_print("%s\n",str2);
	return 0;
}