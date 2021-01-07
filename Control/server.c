#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../symtab/object_data.h"
// #include "../symtab/error_invalid.h"
// #include "../symtab/database.h"
#include "../symtab/jsonapi.h"
#define QUEUE_MAX 10
//test
typedef struct {
    int id;
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
Node *delete_node(Node* root, int sock){
    Node *cur = root;
    if(root->element.sockfd == sock){
        root = root->next;
        return root;
    }

    while(cur->next != NULL){

        if(cur->next->element.sockfd == sock){
            Node *p = cur->next;
            cur->next = p->next;
            free(p);
            break;
        }
        cur = cur->next;
    }
    return root;
}
void printList(Node *root){
    if(root == NULL){
        printf("ds rong\n");
        return;
    }
    Node *cur = root;
    while(cur != NULL){
        printf("id: %d\tsockID: %d\n",cur->element.id,cur->element.sockfd);
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
    // List_DB *root_db = NULL;
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
    char check_url[] ="http://127.0.0.1:8000/api/user";
    char * buff_check_server = (char*)malloc(100*sizeof(char));
    FD_ZERO(&masterfds);
    FD_ZERO(&readfds);
    FD_SET(sockfd,&masterfds);
    maxfd = sockfd;
    buff_check_server = handle_url(check_url);
    if (buff_check_server==NULL) {
        printf("Cant connect to database server \n");
        free(buff_check_server);
        close(sockfd);
        return 0;
    }
   
    puts("Server running...");

    do{
        memcpy(&readfds,&masterfds,sizeof(masterfds));
        if((selectfd = select(maxfd +1,&readfds,NULL,NULL,NULL)) < 0){
            perror("select");
            return 0;
        }else if(selectfd == 0){
            printf("timeout\n");
            //sleep(3);
        }else{
            if(FD_ISSET(sockfd,&readfds)){
                int index_clie;             
                len_clie = sizeof(clieaddr);
                if((connfd = accept(sockfd,(struct sockaddr*)&clieaddr,&len_clie)) < 0){
                    perror("Error: accept! ");
                    return 0;
                }
                

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

                        buff_check_server = handle_url(check_url);
                        if (buff_check_server==NULL) {
                            free(buff_check_server);
                            close(sockfd);
                            return 0;
                    }
                if((sock_cl = client[i]) < 0) continue;
                if(FD_ISSET(sock_cl,&readfds)){   
                    Object *obj = (Object*)malloc(sizeof(Object));
                    if((recvBytes = recv(sock_cl,obj,sizeof(Object),0)) < 0){
                        printf("ERROR! In socket %d\n",sock_cl);
                        perror("recv - message");
                        client[i] = -1;
                        FD_CLR(sock_cl,&masterfds);
                        close(sock_cl);
                        return 0;
                    }  
                                  
                    switch(obj->signal){
                        case SIGNAL_LOGIN:
                            {
                            printf("------------LOGIN-------------\n");
                            
                            int check_login = -1;
                            Error err_login;
                            user_db userdb;
                            // char msg_err_login[100];
                            printf("Account: %s\n",obj->login.username);
                            printf("Password: %s\n\n",obj->login.password);  
                            //check loi
                             
                                check_login = check_user(obj->login.username); 
                                 
                              
                                if(check_login == 1){
                                    userdb = getUser(obj->login.username, -1);
                                   
                                    if(strcmp(userdb.password, obj->login.password) != 0){ // sai pass
                                        err_login = ERR_CAN_NOT_PASSWORD;
                                    }else{ //dang nhap thanh cong
                                        if(userdb.login_status == 0){
                                            //----them node computer----
                                            Elementtype *element = (Elementtype*)malloc(sizeof(Elementtype));
                                            element->sockfd = connfd;
                                            element->id = userdb.ID_user;
                        
                                            root = insert_at_end(root,*element);
                                            printList(root);
                                                                                            
                                            //---------
                                            strcpy(obj->login.name, userdb.name);
                                            obj->login.id = userdb.ID_user;
                                            err_login = ERR_NONE;
                                            loginStatus(obj->login.username, 1);
                                           
                                        }else{
                                            err_login = ERR_USERNAME_LOGIN;
                                        }
                                    }

                                }else{ //sai username
                                    err_login = ERR_NOT_USERNAME;
                                }
                                
                                obj->login.err = err_login;
                                // error_to_string(err_login,msg_err_login);
                                if(send(sock_cl,obj,sizeof(Object),0) < 0){
                                    printf("ERROR! In socket %d\n",sock_cl);
                                    perror("send - msg_err login");
                                    client[i] = -1;
                                    FD_CLR(sock_cl,&masterfds);
                                    close(sock_cl);
                                    return 0;
                                }
                                             
                            break;
                            }
                        case SIGNAL_SIGNUP: 
                        {             

                            printf("------------SIGNUP-----------\n");
                            printf("Name: %s\n",obj->signup.name);
                            printf("Account: %s\n",obj->signup.username);
                            printf("Password: %s\n",obj->signup.password);
                            printf("Confirm Password: %s\n\n",obj->signup.re_password);
                     
                            //Data_base *db = (Data_base*)malloc(sizeof(Data_base));
                            Error err = ERR_NONE;
                            char msg_err[100];
                            user_db user;
                            strcpy(user.name,obj->signup.name);
                            strcpy(user.username,obj->signup.username);
                            strcpy(user.password,obj->signup.password);
                            user.login_status = 0;
                            
                                // List_DB *cur_db = root_db;
                            if (check_user(obj->signup.username) == -1){
                                postUser(user);
                                err = ERR_NONE;
                            }else{
                                err = ERR_HAS_USERNAME;
                            }
                            
                            error_to_string(err,msg_err);
                            if(send(sock_cl,msg_err,strlen(msg_err),0) < 0){
                                printf("ERROR! In socket %d\n",sock_cl);
                                perror("send - err");
                                client[i] = -1;
                                FD_CLR(sock_cl,&masterfds);
                                close(sock_cl);
                                return 0;
                            }
                            
                            //obj->signals = SIGNAL_NONE;
                            break;
                        }
                        case SIGNAL_CHAT_GROUP:
                        {
                            printf("-----------chat group\n");
                            // luu db
                            postMessageGroup(obj->chat_group.from_id,obj->chat_group.message, obj->chat_group.ID_Room);
                            int i;
                            Node *cur_chat_group = root;
                            while(cur_chat_group != NULL){
                                for(i = 0; i < obj->chat_group.length_to_member; i++){
                                    if(obj->chat_group.to_id_member[i] == cur_chat_group->element.id){
                                        if(send(cur_chat_group->element.sockfd, obj,sizeof(Object), 0) < 0){
                                           printf("ERROR! In socket %d\n",sock_cl);
                                            perror("send - err");
                                            client[i] = -1;
                                            FD_CLR(sock_cl,&masterfds);
                                            close(cur_chat_group->element.sockfd);
                                            return 0; 
                                        }
                                        break;
                                    }
                                }

                                cur_chat_group = cur_chat_group->next;
                            }
                            break;
                        }
                        case SIGNAL_CHAT_PRIVATE:
                            printf("-----------chat private\n");
                            printf("%s : %s\n",obj->chat_private.from_username,obj->chat_private.message);
                            
                            postMessage(obj->chat_private.from_id, obj->chat_private.to_id,obj->chat_private.message);
                    
                            Node *cur_chat = root;
                            Object *obj_chat_private;
                            while(cur_chat != NULL){
                                if(cur_chat->element.id==obj->chat_private.to_id){
                                    //char message_[250];
                                    //sprintf(message_,"[%s]:\n%s",obj->chat_private.from_username,obj->chat_private.message);
                                    obj_chat_private = duplicate_object(obj);
                                    if(send(cur_chat->element.sockfd,obj_chat_private, 
                                        sizeof(Object),0)<0){
                                        printf("ERROR! In socket %d\n",sock_cl);
                                        perror("recv - message");
                                        client[i] = -1;
                                        FD_CLR(sock_cl,&masterfds);
                                        close(sock_cl);
                                        return 0;
                                    }
                                    break;
                                }
                                cur_chat = cur_chat->next;
                            }
                            
                            break;
                        case SIGNAL_RECV_CHAT_PRIVATE:
                        {
                            printf("-----------recv chat private\n");
                            Data_base *db_chat_private = (Data_base*)malloc(sizeof(Data_base));
                            // user_db  profile_recv;
                            // profile = getUser(obj->chat_private.from_username, -1);
                            // sleep(0.3); 
                            // profile_recv = getUser(obj->chat_private.to_username, -1);
                            // sleep(0.3); 
                            db_chat_private = getMessagePrivate(obj->chat_private.from_id, obj->chat_private.to_id);
                            db_chat_private->signal = SIGNAL_DB_CHAT_PRIVATE;
                            if(send(sock_cl,db_chat_private, sizeof(Data_base), 0) < 0){
                                printf("ERROR! In socket %d\n",sock_cl);
                                perror("recv - message");
                                client[i] = -1;
                                FD_CLR(sock_cl,&masterfds);
                                close(sock_cl);
                                return 0;
                            }
                            break;
                        }
                        case SIGNAL_RECV_LIST_FRIEND:
                        {
                                printf("-----------recv list friend\n");
                            Data_base *db_list_friend = (Data_base*)malloc(sizeof(Data_base));
                            
                            user_db user = getUser(obj->login.username, -1);
                           
                            char* url = (char*)malloc(200*sizeof(char));
                            sprintf(url, "http://127.0.0.1:8000/api/friends/?user=%d", user.ID_user);
                            char* buffer = handle_url(url);
                            if (buffer){
                                db_list_friend =getListFriend(buffer);
                              
                                db_list_friend->signal = SIGNAL_DB_LIST_FRIEND;
                            }

                            if(send(sock_cl,db_list_friend,sizeof(Data_base),0) < 0){
                                printf("ERROR! In socket %d\n",sock_cl);
                                perror("send recv list friend");
                                client[i] = -1;
                                FD_CLR(sock_cl,&masterfds);
                                close(sock_cl);
                                return 0;
                            }
                            break;
                        }
                        case SIGNAL_RECV_LIST_FRIEND_PRIVATE:
                        {
                            printf("-----------recv list friend private\n");
                            Data_base *db_list_friend = (Data_base*)malloc(sizeof(Data_base));
                            
                            user_db user = getUser(obj->login.username, -1);
                            
                            char* url = (char*)malloc(200*sizeof(char));
                            sprintf(url, "http://127.0.0.1:8000/api/friends/?user=%d", user.ID_user);
                            char* buffer = handle_url(url);
            
                            if (buffer){
                                db_list_friend =getListFriend(buffer);
                            
                                db_list_friend->signal = SIGNAL_DB_LIST_FRIEND_PRIVATE;
                            }
                          
                

                            if(send(sock_cl,db_list_friend,sizeof(Data_base),0) < 0){
                                printf("ERROR! In socket %d\n",sock_cl);
                                perror("send recv list friend");
                                client[i] = -1;
                                FD_CLR(sock_cl,&masterfds);
                                close(sock_cl);
                                return 0;
                            }
                            break;
                        }
                        case SIGNAL_CHANGE_PASSWORD:
                        {    printf("------Change password-----------\n");
                            printf("Username: %s\n",obj->change_password.username);
                            printf("New Password: %s\n",obj->change_password.new_password);
                            // List_DB *db_change_pass = root_db;
                            changePassword(obj->change_password.username, obj->change_password.new_password);
                           
                            break;
                        }
                        case SIGNAL_ADD_FRIEND:
                            
                            break;
                        case SIGNAL_RECV_LIST_GROUP:
                        {
                            printf("-----------SIGNAL_RECV_LIST_GROUP\n");
                             Data_base *db = (Data_base*)malloc(sizeof(Data_base));
                            char* buffer = (char*)malloc(200*sizeof(char));
                            char* url = (char*)malloc(100*sizeof(char));
                            // int i,j;
                            sprintf(url,"http://127.0.0.1:8000/api/room/?user=%d", obj->login.id);
                            buffer = handle_url(url);
                            if (buffer){
                                db = getGroup(buffer);

                            }
                            // for(i = 0; i < db->list_group.length_group;i++){
                            //     for (j=0;j<db->list_group.group[i].length_msg_public;j++){

                            //         printf("group :%ld , message : %s \n", i, db->list_group.group[i].msg_public[j].message);
                            //     }
	                        // }
                            db->signal = SIGNAL_RECV_DB_LIST_GROUP;
                            if(send(sock_cl,db,sizeof(Data_base),0) < 0){
                                printf("ERROR! In socket 123 %d\n",sock_cl);
                                        perror("recv - list group");
                                        client[i] = -1;
                                        FD_CLR(sock_cl,&masterfds);
                                        close(sock_cl);
                                        return 0;
                            }
                            break;
                        }
                        case SIGNAL_ADD_ROOM:
                        {
                            int check = check_user(obj->add_member.username);
                            if (check == -1){
                                obj->add_member.err = ERR_NOT_USERNAME;
                                
                            } else {
                                user_db user = getUser(obj->add_member.username, -1);
                                strcpy(obj->add_member.name, user.name);
                                obj->add_member.ID = user.ID_user;
                                obj->add_member.err = ERR_NONE;
                                //---them member vao database
                            }
                            if(send(sock_cl,obj,sizeof(Object),0) < 0){

                            }
                            break;
                        }
                        case SIGNAL_CONFIRM_FRIEND:
                        {
                            // update status friend
                            
                            break;
                        }
                        case SIGNAL_NO_CONFIRM_FRIEND:
                        {
                            // delete friend 
                            break;
                        }
                        case SIGNAL_LOGUOT:
                        {
                           printf("----------- SIGNAL_LOGUOT\n");
                           root = delete_node(root,sock_cl);
                            loginStatus(obj->login.username, 0);
                          
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
    //free_list_db(root_db);
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