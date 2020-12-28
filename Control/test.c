#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include "../symtab/jsonapi.h"

int main(int argc, char const *argv[]){

size_t length_eltype;
	
	char *buffer;
	char url[]= "http://127.0.0.1:8000/api/friends/?user=1";
 	buffer = handle_url(url);
     if (buffer){
         printf("hello");
     }

}