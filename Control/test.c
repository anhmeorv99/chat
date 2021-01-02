#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include "../symtab/jsonapi.h"

char *convert_struct_to_json_message(int from_user, char* message, int room, int member[], int size){
    char *buffer = (char*)malloc(200*sizeof(char));
    char *id = (char*)malloc(sizeof(char));
    char *data = (char*)malloc(200*sizeof(char));
    int i;
    sprintf(buffer,"[");
    for (i=0;i<size;i++){
        if (i==size-1){
             sprintf(id,"%d]",member[i]);
             strcat(buffer,id);
        }else {
            sprintf(id,"%d, ",member[i]);
            strcat(buffer,id);
        }
      
    }
   
    sprintf(data,"{\"from_user\": %d, \"message\": \"%s\", \"room\": %d, \"member\": %s}", from_user,message,room,buffer);
    free(buffer);
    return data;
}

void postMessage(char* data){
     CURL *curl;
  CURLcode res;
 
  /* In windows, this will init the winsock stuff */ 
  curl_global_init(CURL_GLOBAL_ALL);
 
  /* get a curl handle */ 
  curl = curl_easy_init();
  if(curl) {
    struct curl_slist *chunk = NULL; 
    chunk = curl_slist_append(chunk, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
    
    curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8000/api/message/");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
    res = curl_easy_perform(curl);
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
}



int main(int argc, char const *argv[]){

    int from_user=2;
    char message[]= "Chasn qua di thoi";
    int room = 2;
    int member[2] = {1,2};
    int size = 2;
    char *s = convert_struct_to_json_message(from_user,message,room,member,size);
    postMessage(s);

}