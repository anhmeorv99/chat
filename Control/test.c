#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

int main(int argc, char const *argv[]){
CURL *curl;
CURLcode res;
long http_code;

curl = curl_easy_init();
curl_global_init(CURL_GLOBAL_ALL);

if(curl){

    curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8000/api/user/2/?status=0");
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
    


    //enable to spit out information for debugging
    //curl_easy_setopt(curl, CURLOPT_VERBOSE,1L); 

    res = curl_easy_perform(curl);

    if (res != CURLE_OK){
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res) );
    }

    printf("\nget http return code\n");
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    printf("http code: %lu\n", http_code );


    curl_easy_cleanup(curl);
    curl_global_cleanup();

    }
return 0;
}