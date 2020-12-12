#include <stdlib.h>
#include <string.h>
#include "object.h"
#define NUM_OF_SIGNAL 4
//-------------------------SIGNAL-------------------
struct Signal_Message_{
	Signals signal;
	char *message;
};

struct Signal_Message_ signal_message[NUM_OF_SIGNAL] = {
	{SIGNAL_NONE, "SIGNAL_NONE"},
	{SIGNAL_LOGIN, "SIGNAL_LOGIN"},
	{SIGNAL_SIGNUP, "SIGNAL_SIGNUP"},
	{SIGNAL_CHAT_PRIVATE, "SIGNAL_CHAT_PRIVATE"}
};
//convect Signal: enum -> char*
void convect_signal_to_message(Signals signal,char *message){
	int i;
	for(i = 0; i < NUM_OF_SIGNAL; i++){
		if(signal_message[i].signal == signal){
			strcpy(message,signal_message[i].message);
			return;
		}
	}
}
//convect Message: char* -> enum
void convect_message_to_signal(char *message,Object *obj){
	int i;
	for(i = 0; i < NUM_OF_SIGNAL;i++){
		if(strcmp(message,signal_message[i].message) == 0){
			obj->signals = signal_message[i].signal;
			return;
		}
	}
}
//---------------------------LOGIN-------------------------
//cap phat struct Login
Login *new_login(){
	Login *login = (Login*)malloc(sizeof(Login));
	return login;
}
//giai phong Login
void free_login(Login *login){
	free(login);
}

//create Login
Login *make_login(char *account, char *password){
	Login *login = new_login();
	strcpy(login->account, account);
	strcpy(login->password, password);
	return login;
}
//copy Login
Login *duplicate_login(Login *login){
	Login *log_in = new_login();
	strcpy(log_in->account, login->account);
	strcpy(log_in->password,login->password);
	return log_in;
}
//so sanh 2 Login | giong : TRUE , khac : FALSE
bool equal_login(Login *login1,Login *login2){
	if(strcmp(login1->account,login2->account) != 0)
		return false;
	if(strcmp(login1->password,login2->password) != 0)
		return false;
	return true;
}

//---------------------SIGNUP---------------------------

//cap phat Signup
Signup *new_signup(){
	Signup *signup = (Signup*)malloc(sizeof(Signup));
	return signup;
}
//giai phong Signup
void free_signup(Signup *signup){
	free(signup);
}
//create Signup

Signup *make_signup(char *name,char *account, 
					char *password, char *re_password)
{
	Signup *signup = new_signup();
	strcpy(signup->name,name);
	strcpy(signup->account ,account);
	strcpy(signup->password ,password);
	strcpy(signup->re_password ,re_password);
	return signup;
}
//copy Signup
Signup *duplicate_signup(Signup *signup){
	Signup *sign_up = new_signup();
	strcpy(sign_up->name,signup->name);
	strcpy(sign_up->account ,signup->account);
	strcpy(sign_up->password ,signup->password);
	strcpy(sign_up->re_password ,signup->re_password);
	return sign_up;
}
//so sanh 2 Signup
bool equal_signup(Signup *signup1, Signup *signup2){
	if(strcmp(signup1->name,signup2->name) != 0)
		return false;
	if(strcmp(signup1->account,signup2->account) != 0)
		return false;
	if(strcmp(signup1->password,signup2->password) != 0)
		return false;
	if(strcmp(signup1->re_password,signup2->re_password) != 0)
		return false;
	return true;
}
//----------------------Chat Private-------------------
//cap phat Chat_Private
Chat_Private *new_chat_private(){
	Chat_Private *chat = (Chat_Private*)malloc(sizeof(Chat_Private));
	return chat;
}
//giai phong Chat_private
void free_chat_private(Chat_Private *chat){
	free(chat);
}
//make chat_private
Chat_Private *make_chat_private(char *from_username, char *to_username, char *message){
	Chat_Private *chat = new_chat_private();
	strcpy(chat->from_username , from_username);
	strcpy(chat->to_username , to_username);
	strcpy(chat->message,message);
	return chat;
}
//copy chat private
Chat_Private *duplicate_chat_private(Chat_Private *chat){
	Chat_Private *new_chat = make_chat_private(chat->from_username,
		chat->to_username,chat->message);
	return new_chat;
}

//-------------------------Object------------------------

//cap phat tat ca trong Object
Object *new_object_all(){
	Object *obj = (Object*)malloc(sizeof(Object));
	obj->signals = SIGNAL_NONE;
	obj->login = new_login();
	obj->signup = new_signup();
	return obj;
}
//giai phong tat ca trong Object
void free_object_all(Object *obj){
	free_login(obj->login);
	free_signup(obj->signup);
	free(obj);
}


//cap phat Login - Object
Object *new_login_object(){
	Object *obj = (Object*)malloc(sizeof(Object));
	obj->signals = SIGNAL_LOGIN;
	obj->login = new_login();
	return obj;
}
//create Login - Object
Object *make_login_object(Login *login){
	Object *obj = (Object*)malloc(sizeof(Object));
	obj->signals = SIGNAL_LOGIN;
	obj->login = duplicate_login(login);
	return obj;
}
//cap phat Signup - Object
Object *new_signup_object(){
	Object *obj = (Object*)malloc(sizeof(Object));
	obj->signals = SIGNAL_SIGNUP;
	obj->signup = new_signup();
	return obj;
}
//create Signup - Object
Object *make_signup_object(Signup *signup){
	Object *obj = (Object*)malloc(sizeof(Object));
	obj->signals = SIGNAL_SIGNUP;
	obj->signup = duplicate_signup(signup);
	return obj;
}
//cap phat chat private - object
Object *new_chat_private_object(){
	Object *obj = (Object*)malloc(sizeof(Object));
	obj->signals = SIGNAL_CHAT_PRIVATE;
	obj->chat_private = new_chat_private();
	return obj;
}
//create chat private - object
Object *make_chat_private_object(Chat_Private *chat){
	Object *obj = (Object*)malloc(sizeof(Object));
	obj->signals = SIGNAL_CHAT_PRIVATE;
	obj->chat_private = duplicate_chat_private(chat);
	return obj;
}
//copy Object
Object *duplicate_object(Object *object){
	Object *obj = (Object*)malloc(sizeof(Object));
	switch(object->signals){
		case SIGNAL_LOGIN:
			obj->signals = object->signals;
			obj->login = duplicate_login(object->login);
			break;
		case SIGNAL_SIGNUP:
			obj->signals = object->signals;
			obj->signup = duplicate_signup(object->signup);
			break;
		case SIGNAL_CHAT_PRIVATE:
			obj->signals = object->signals;
			obj->chat_private = duplicate_chat_private(object->chat_private);
			break;
		case SIGNAL_NONE:
			break;
	}
	return obj;
}

//giai phong Object
void free_object(Object *obj){
	switch(obj->signals){
		case SIGNAL_LOGIN:
			free_login(obj->login);
			break;
		case SIGNAL_SIGNUP:
			free_signup(obj->signup);
			break;
		case SIGNAL_CHAT_PRIVATE:
			free_chat_private(obj->chat_private);
			break;
		case SIGNAL_NONE:
			break;
	}
	free(obj);
}

