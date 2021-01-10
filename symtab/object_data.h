#include <stdlib.h>
#include "error_invalid.h"
typedef enum {
	SIGNAL_NONE,
	SIGNAL_LOGIN,
	SIGNAL_SIGNUP,
	SIGNAL_RECV_CHAT_PRIVATE,
	SIGNAL_CHAT_PRIVATE,
	SIGNAL_RECV_LIST_GROUP,
	SIGNAL_ADD_ROOM,
	SIGNAL_CHAT_GROUP,
	SIGNAL_RECV_LIST_FRIEND,
	SIGNAL_CONFIRM_FRIEND,
	SIGNAL_NO_CONFIRM_FRIEND,
	SIGNAL_RECV_LIST_FRIEND_PRIVATE,
	SIGNAL_ADD_FRIEND,
	SIGNAL_CHANGE_PASSWORD,
	//SIGNAL_ADMIN,
	SIGNAL_LOGUOT
}Signals;

typedef struct {
	int id;
	char username[10];
	char name[10];
	char password[10];
	Error err;
}Login;

typedef struct {
	char name[10];
	char username[10];
	char password[10];
	char re_password[10];
}Signup;

typedef struct {
	char from_name[10];
	char to_name[10];
	char from_username[10];
	int from_id;
	int to_id;
	char to_username[10];
	char message[50];
	char create_at[10];
}Chat_Private;

typedef struct {
	char username[10];
	char new_password[10];
	char cur_password[10];
}Change_Password;

typedef struct {
	int ID;
	char name_friend[10];
	char username_friend[10];
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
typedef struct{
	Error err;
	int ID;
	int ID_Room;
	char name[10];
	char username[10];
}Add_Member;
typedef struct {
	Signals signal;
	Login login;
	Signup signup;
	Add_Friend add_friend;
	Chat_Private chat_private;
	Change_Password change_password;
	Chat_Group_OK chat_group;
	Add_Member add_member;
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




