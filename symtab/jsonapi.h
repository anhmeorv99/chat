#include<stdio.h>
#include<json-c/json.h>
#include <string.h>
#include <stdlib.h>

struct url_data {
    size_t size;
    char* data;
};

typedef struct user {
    struct json_object *id;
    struct json_object *name;
    struct json_object *username;
    struct json_object *password;
    struct json_object *status;
    struct json_object *created_at;
    struct json_object *is_admin;
} User;

typedef struct friends {
    struct json_object *id;
    struct json_object *_friend;
    struct json_object *confirm;
    struct json_object *user;
} Friend;

typedef struct room {
	struct json_object *id;
	struct json_object *room_status;
 	struct json_object *name;
  	struct json_object *admin_room; 
  	struct json_object *member;
} Room;

typedef struct message {
	struct json_object *from_user;
 	struct json_object *message;
  	struct json_object *created_at; 
  	struct json_object *room;
} Message;

typedef struct messageprivate {
	struct json_object *from_user;
 	struct json_object *message;
  	struct json_object *created_at; 
  	struct json_object *to_user;
} MessagePrivate;

typedef enum{
	DB_NONE,
	SIGNAL_DB_LIST_FRIEND,
	DB_LIST_FRIEND,
	SIGNAL_DB_CHAT_PRIVATE,
	DB_CHAT_PRIVATE,
	DB_CHAT_PUBLIC,
	SIGNAL_RECV_DB_LIST_GROUP,
	SIGNAL_DB_LIST_FRIEND_PRIVATE
}Signal_db;

typedef struct {
	int ID; //khoa chinh
	int confirm;
	char username[30]; //username cua friend
	char name[30];
}friend_db;

typedef struct {
	int ID; 
	char username[30];
	char name[30];
}member_db;

typedef struct {
	int from_id;
	int to_id;
	char from_username[30];
	char to_username[30];
	char to_name[30];
	char from_name[30];
	char message[500];
	char create_at[50];
}message_db;

typedef struct {
	message_db msg_private[250];
	size_t length_message;
}Chat_Private_;

typedef struct {
	int ID_group;
	char name[30];
	int id_admin;
	member_db members[12];
	int length_member;
	message_db msg_public[500];
	int length_msg_public;
}group_db;

typedef struct {
	int ID_user;
	char name[30];
	char username[30];
	char password[30];
	int is_admin;
	int login_status;
}user_db;

typedef struct {
	friend_db list_friend[20];
	int length_list_friend;
} List_Friend_;

typedef struct {
	group_db group[10];
	int length_group;
} List_Group_;


typedef struct {
	Signal_db signal;
	user_db user;
	List_Friend_ list_friend;
	List_Group_ list_group;
	Chat_Private_ chat_private;
}Data_base;

Data_base *getListFriend(char* element);

Data_base * getGroup(char* element);

Data_base *getMessagePrivate(int from_user, int to_user);
Data_base *getMessageGroup(int room);

friend_db getFriend(Friend friend);

member_db getMember(int id);

message_db getOneMessageGroup(Message message,user_db from_profile);
message_db getOneMessagePrivate(MessagePrivate message,user_db from_profile, user_db to_profile);

char *convert_struct_to_json_message(int from_user, char* message, int room);
void postMessage(int from_user, int to_user, char* message);
void postMessageGroup(int from_user,char* message, int room);
// Chat_Private_ getChatPrivate(message_db message, int index);


group_db getGroupDB(Room room, member_db member, message_db message, int index_member, int index_message);

int get_id_room_private(char* username1, char* username2);

user_db getUserDB(User user);

// Data_base getDatabase(user_db user, friend_db *friend, int len_friend, Chat_Private_ *chat_private, 
// 						int len_chat_private, group_db *group, int len_group);

int check_user(char* username);

user_db getUser(char* username,int id);


size_t write_data(void *ptr, size_t size, size_t nmemb, struct url_data *data);
char *handle_url(char* url);

void postUser(user_db user);
void changePassword(char* username, char* password);
void loginStatus(char* username,int status);