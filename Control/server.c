#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../symtab/object_data.h"
#include "../symtab/error_invalid.h"

#define QUEUE_MAX 10
//test
typedef struct {
    char username[30];
    int sockfd;
}Elementtype;
typedef struct Node_ {
    Elementtype element;  
    struct Node_ *next;
}Node;

Node *new_node(Elementtype e){
    Node *new = (Node*)malloc(sizeof(Node));
    new->element = e;
    new->next = NULL;
    return new;
}

Node *insert_at_end(Node *root, Elementtype element){
    Node *new = new_node(element);
    if(root == NULL)
        return new;
    Node *cur = root;
    while(cur->next != NULL)
        cur = cur->next;
    cur->next = new;

    return root;
}

void printList(Node *root){
    if(root == NULL){
        printf("ds rong\n");
        return;
    }
    Node *cur = root;
    while(cur != NULL){
        printf("username: %s\tsockID: %d\n",cur->element.username,cur->element.sockfd);
        cur = cur->next;
    }
    

}
//
int checkNumber(char* str);

int main(int argc, char **argv){

    int sockfd, connfd, port, recvBytes;
    struct sockaddr_in servaddr, clieaddr;
    socklen_t len_serv, len_clie;
    int selectfd, maxfd, maxi = -1;
    int i = 0;
    fd_set readfds, masterfds;
    int client[QUEUE_MAX];
    //test
    Node *root = NULL;
    //
    if(argc != 2){
        printf("Error: sai cu phap\n");
        printf("Cu phap: ./server <port-number>\n");
        return 0;
    }
    
    if(checkNumber(argv[1]) == 0){
        printf("Error: parameter1 (port) phai la so\n");
        return 0;
    }

    port = atoi(argv[1]);
    for(; i < QUEUE_MAX; i++){
        client[i] = -1;
    }
    //create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        perror("Error: create socket !");
        return 0;
    }
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");//htonl(INADDR_ANY);

    len_serv = sizeof(servaddr);

    if(bind(sockfd, (struct sockaddr*)&servaddr,len_serv) < 0){
        perror("Error: bind!");
        return 0;
    }
    if(listen(sockfd,QUEUE_MAX) < 0){
        perror("Error: listen!");
        return 0;
    }

    FD_ZERO(&masterfds);
    FD_ZERO(&readfds);
    FD_SET(sockfd,&masterfds);
    maxfd = sockfd;

    puts("Server running...");

    do{
        memcpy(&readfds,&masterfds,sizeof(masterfds));
        if((selectfd = select(maxfd +1,&readfds,NULL,NULL,NULL)) < 0){
            perror("select");
            return 0;
        }else if(selectfd == 0){
            printf("timeout\n");
            sleep(3);
        }else{
            if(FD_ISSET(sockfd,&readfds)){
                int index_clie;
               
                len_clie = sizeof(clieaddr);
                if((connfd = accept(sockfd,(struct sockaddr*)&clieaddr,&len_clie)) < 0){
                    perror("Error: accept! ");
                    return 0;
                }
                //test
                Elementtype *element = (Elementtype*)malloc(sizeof(Elementtype));
                element->sockfd = connfd;
                printf("sock %d\n",element->sockfd);
                root = insert_at_end(root,*element);
                printList(root);
                printf("\n");
                //

                printf("New connect with client %d\n",connfd);
               
                index_clie = connfd - sockfd - 1;

                if(index_clie == QUEUE_MAX){
                    printf("Server day!\n");   
                }else{
                    if(client[index_clie] == -1){
                        client[index_clie] = connfd;
                    }
                }
                FD_SET(connfd,&masterfds);
                if(connfd > maxfd){
                        maxfd = connfd;
                }
                if(index_clie > maxi) 
                        maxi = index_clie;
            }

            for(i = 0; i<= maxi;i++){
                int sock_cl;
                if((sock_cl = client[i]) < 0) continue;
                if(FD_ISSET(sock_cl,&readfds)){
                    
                    Object *obj = (Object*)malloc(sizeof(Object));
                    
                    if((recvBytes = recv(sock_cl,obj,sizeof(Object),0)) < 0){
                        printf("ERROR! In socket %d\n",sock_cl);
                        perror("recv - message");
                        client[i] = -1;
                        FD_CLR(sock_cl,&masterfds);
                        close(sock_cl);
                        continue;
                    }
                    
                    switch(obj->signal){
                        case SIGNAL_LOGIN:

                            printf("------------LOGIN-------------\n");
                            //test
                            {
                                Node *cur_login = root;
                                while(cur_login != NULL){
                                    if(cur_login->element.sockfd == sock_cl){
                                        strcpy(cur_login->element.username, obj->login.username);
                                        break;
                                    }
                                    cur_login = cur_login->next;
                                }
                            }
                            //
                            printf("Account: %s\n",obj->login.username);
                            printf("Password: %s\n\n",obj->login.password);
                            //obj->signals = SIGNAL_NONE;
                            break;
                        case SIGNAL_SIGNUP:
                            
                            printf("------------SIGNUP-----------\n");
                            printf("Name: %s\n",obj->signup.name);
                            printf("Account: %s\n",obj->signup.username);
                            printf("Password: %s\n",obj->signup.password);
                            printf("Confirm Password: %s\n\n",obj->signup.re_password);
                            //obj->signals = SIGNAL_NONE;
                            break;
                        case SIGNAL_CHAT_PRIVATE:
                            
                            printf("%s : %s\n",obj->chat_private.from_username,obj->chat_private.message);
                            Node *cur_chat = root;
                            while(cur_chat != NULL){
                                if(strcmp(cur_chat->element.username,obj->chat_private.from_username) != 0){
                                    if(send(cur_chat->element.sockfd, obj->chat_private.message, strlen(obj->chat_private.message),0)<0){
                                        printf("ERROR! In socket %d\n",sock_cl);
                                        perror("recv - message");
                                        client[i] = -1;
                                        FD_CLR(sock_cl,&masterfds);
                                        close(sock_cl);
                                        return 0;
                                    }
                                }
                                cur_chat = cur_chat->next;
                            }
                            
                            break;
                        case SIGNAL_NONE:
                            break;
                    }
                    free_object(obj);
                  
                }
            }
            
        }
    }while(1);
    puts("server closed!\n");
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