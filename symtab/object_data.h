#include <stdlib.h>
#include "error_invalid.h"
typedef enum {
	SIGNAL_NONE,
	SIGNAL_LOGIN,
	SIGNAL_SIGNUP,
	SIGNAL_RECV_CHAT_PRIVATE,
	SIGNAL_CHAT_PRIVATE,
	SIGNAL_RECV_LIST_GROUP,
	SIGNAL_CHAT_GROUP,
	SIGNAL_RECV_LIST_FRIEND,
	SIGNAL_RECV_LIST_FRIEND_PRIVATE,
	SIGNAL_ADD_FRIEND,
	SIGNAL_CHANGE_PASSWORD,
	//SIGNAL_ADMIN,
	SIGNAL_LOGUOT
}Signals;

typedef struct {
	int id;
	char username[20];
	char name[20];
	char password[20];
	Error err;
}Login;

typedef struct {
	char name[20];
	char username[20];
	char password[20];
	char re_password[20];
}Signup;

typedef struct {
	char from_name[20];
	char to_name[20];
	char from_username[20];
	int from_id;
	int to_id;
	char to_username[20];
	char message[100];
	char create_at[20];
}Chat_Private;

typedef struct {
	char username[20];
	char new_password[20];
	char cur_password[20];
}Change_Password;

typedef struct {
	char username[20];
	char username_friend[20];
}Add_Friend;
typedef struct {
	int ID_Room;
	int to_id_member[12];
	int length_to_member;
	char message[100];
	char from_name[20];
	int from_id;
	char created_at[20];
}Chat_Group_OK;
typedef struct {
	Signals signal;
	Login login;
	Signup signup;
	Add_Friend add_friend;
	Chat_Private chat_private;
	Change_Password change_password;
	Chat_Group_OK chat_group;
}Object;

//cap phat Object void signal la none
Object *new_object();
//cap phat Object voi signal la Login
Object *new_login_object();
//cap phat Object voi signal la Signup
Object *new_signup_object();
//cap phat Object voi signal la Chat_Private
Object *new_chat_private_object();
//cap phat Object voi signal la Change_Password
Object *new_change_password_object();
//sao chep Object
Object *duplicate_object(Object *obj);
//giai phong Object
void free_object(Object *obj);




