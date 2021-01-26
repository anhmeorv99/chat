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
	SIGNAL_DB_LIST_FRIEND_PRIVATE,
	DB_ADMIN
}Signal_db;

typedef struct {
	int ID; //khoa chinh
	int confirm;
	int loginStatus;
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
	char message[100];
	char create_at[20];
	char len[4];
}message_db;

typedef struct {
	message_db msg_private[100]; //
	int length_message;
}Chat_Private_;

typedef struct {
	int ID_group;
	char name[30];
	int id_admin;
	member_db members[11];
	int length_member;
	message_db msg_public[100];
	int length_msg_public;
}group_db;

typedef struct {
	int ID_user;
	char name[30];
	char username[30];
	char password[30];
	int is_admin;
	int login_status;
	char created_at[20];
}user_db;

typedef struct {
	friend_db list_friend[10];
	int length_list_friend;
} List_Friend_;

typedef struct {
	group_db group[10];
	int length_group;
} List_Group_;

typedef struct {
	user_db user[20];
	int length_user;
} List_User_;

typedef struct {
	Signal_db signal;
	List_Friend_ list_friend;
	List_Group_ list_group;
}Data_base;

typedef struct {
	Signal_db signal;
	List_Friend_ list_friend;
	Chat_Private_ chat_private;
}Data_base_chat_private;


typedef struct {
	Signal_db signal;
	List_User_ Users;
}Data_base_user;

// HOST


Data_base *getListFriend(int id);

Data_base * getGroup(char* element);

Data_base_chat_private *getMessagePrivate(int from_user, int to_user);
Data_base *getMessageGroup(int room);

Data_base_user *getUserAdmin();

friend_db getFriend(Friend friend, int mode);
void update_confirm_friend(int user, int friend);

member_db getMember(int id);
int updateMember(int room,int member);
void requestData(char*url, char*data, char* method);

void create_room_(char* name, int admin_room);
void add_friend(int user, int friend);
void delete_confirm_friend(int user, int friend);
void delete_user(int user);
void updateUser(int id, char* newpassword, char* name);

message_db getOneMessageGroup(Message message,user_db from_profile);
message_db getOneMessagePrivate(MessagePrivate message,user_db from_profile, user_db to_profile);

char *convert_struct_to_json_message(int from_user, char* message, int room);
void postMessage(int from_user, int to_user, char* message);
void postMessageGroup(int from_user,char* message, int room);

user_db getUserDB(User user);

int check_user(char* username);
int check_friend(int id_user, int id_friend);

user_db getUser(char* username,int id);


size_t write_data(void *ptr, size_t size, size_t nmemb, struct url_data *data);
char *handle_url(char* url);

void postUser(user_db user);
void changePassword(int id, char* newpassword);
void loginStatus(char* username,int status);