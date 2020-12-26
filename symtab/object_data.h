#include <stdlib.h>

typedef enum {
	SIGNAL_NONE,
	SIGNAL_LOGIN,
	SIGNAL_SIGNUP,
	SIGNAL_CHAT_PRIVATE,
	//SIGNAL_CHAT_GROUP,
	//SIGNAL_LIST_FRIEND,
	SIGNAL_ADD_FRIEND,
	SIGNAL_CHANGE_PASSWORD
	//SIGNAL_ADMIN,
	//SIGNAL_LOGUOT
}Signals;

typedef struct {
	char username[30];
	char password[30];
}Login;

typedef struct {
	char name[30];
	char username[30];
	char password[30];
	char re_password[30];
}Signup;

typedef struct {
	char from_username[30];
	char to_username[30];
	char message[200];
	char create_at[50];
}Chat_Private;

typedef struct {
	char username[30];
	char new_password[30];
	char cur_password[30];
}Change_Password;

typedef struct {
	char username[30];
	char username_friend[30];
}Add_Friend;

typedef struct {
	Signals signal;
	Login login;
	Signup signup;
	Add_Friend add_friend;
	Chat_Private chat_private;
	Change_Password change_password;
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




