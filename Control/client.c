#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../src/application.h"
#include <pthread.h>
#include <signal.h>
int sockfd;
int checkIP(char *str);
int checkNumber(char *str);

void catch_ctrl_c(int sig){
    
    Object *obj = (Object*)malloc(sizeof(Object));
    obj->signal = SIGNAL_LOGUOT;
    strcpy(obj->login.username, obj_login->login.username);
    if (send(sockfd, obj, sizeof(Object), 0) < 0){
        perror("error send ctrl + C");
        exit(0);
    }
    close(sockfd);
    exit(0);

}

int main(int argc, char **argv){
    int port;
    struct sockaddr_in servaddr;
    socklen_t len_serv;
    signal(SIGINT, catch_ctrl_c);
    
    if(argc != 3){
        printf("Error: cu phap khong hop le!\n");
        printf("CU phap: ./client <ip address> <port-number>\n");
        return 0;
    }
    
    if(checkIP(argv[1]) == 0){
        printf("Error: parameter1 (ip address) khong hop le!\n");
        return 0;
    }
    // if(strcmp(argv[1],"127.0.0.1") != 0){
    //     printf("Loi khuyen nen dung IP address : 127.0.0.1\n");
    //     return 0;
    // }
    if(checkNumber(argv[2]) == 0){
        printf("Error: parameter2 (port) phai la so!\n");
        return 0;
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0 ){
        perror("Error: loi tao socket!");
        return 0;
    }

    port = atoi(argv[2]);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);   
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);

    len_serv = sizeof(servaddr);
    if(connect(sockfd,(struct sockaddr*)&servaddr,len_serv) < 0){
        perror("Error ");
        return 0;
    }
    
    app(argc,argv,sockfd);
   
    close(sockfd);
    return 0;
}


int checkNumber(char* str){
    int i;
    if(strlen(str) == 0) return 0;
    for(i = 0; i< strlen(str); i++){
        if(!(str[i] >= '0' && str[i] <= '9'))
            return 0;
    }
    return 1;
}

int checkIP(char *str){

    int count = 0, number;
    char *token, buff[100];
    if(strlen(str) == 0) return 0;
    strcpy(buff,str);
    
    token = strtok(buff,".");
    
    while(token != NULL){
        if(checkNumber(token) == 0) return 0;
        number = atoi(token);
        if(number < 0 || number > 255 ) return 0;
        count++;
        token = strtok(NULL,".");
        
    }
    if(count != 4) return 0;
    return 1;

}