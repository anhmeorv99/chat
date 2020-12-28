#include <string.h>
#include "object_data.h"



//cap phat Object void signal la none
Object *new_object(){
	Object *obj = (Object*)malloc(sizeof(Object));
	obj->signal = SIGNAL_NONE;
	return obj;
}
//cap phat Object voi signal la Login
Object *new_login_object(){
	Object *obj = (Object*)malloc(sizeof(Object));
	obj->signal = SIGNAL_LOGIN;
	return obj;
}
//cap phat Object voi signal la Signup
Object *new_signup_object(){
	Object *obj = (Object*)malloc(sizeof(Object));
	obj->signal = SIGNAL_SIGNUP;
	return obj;
}
//cap phat Object voi signal la Chat_Private
Object *new_chat_private_object(){
	Object *obj = (Object*)malloc(sizeof(Object));
	obj->signal = SIGNAL_CHAT_PRIVATE;
	return obj;
}
//cap phat Object voi signal la Change_Password
Object *new_change_password_object(){
	Object *obj = (Object*)malloc(sizeof(Object));
	obj->signal = SIGNAL_CHANGE_PASSWORD;
	return obj;
}
//sao chep Object
Object *duplicate_object(Object *obj){
	Object *new = (Object*)malloc(sizeof(Object));
	switch(obj->signal){
		case SIGNAL_LOGIN:
			new->signal = obj->signal;
			strcpy(new->login.username,obj->login.username);
			strcpy(new->login.password,obj->login.password);
			break;
		case SIGNAL_SIGNUP:
			new->signal = obj->signal;
			strcpy(new->signup.name,obj->signup.name);
			strcpy(new->signup.username,obj->signup.username);
			strcpy(new->signup.password,obj->signup.password);
			strcpy(new->signup.re_password,obj->signup.re_password);
			break;
		case SIGNAL_CHAT_PRIVATE:
			new->signal = obj->signal;
			strcpy(new->chat_private.from_username,obj->chat_private.from_username);
			strcpy(new->chat_private.to_username,obj->chat_private.to_username);
			strcpy(new->chat_private.message,obj->chat_private.message);
			strcpy(new->chat_private.create_at,obj->chat_private.create_at);
			break;
		case SIGNAL_CHANGE_PASSWORD:
			new->signal = obj->signal;
			strcpy(new->change_password.username, obj->change_password.username);
			strcpy(new->change_password.new_password, obj->change_password.new_password);
		case SIGNAL_ADD_FRIEND:
			new->signal = obj->signal;
			break;
		case SIGNAL_LOGUOT:
			new->signal = obj->signal;
			break;
		case SIGNAL_NONE:
			new->signal = obj->signal;
			break;
		
	}
	return new;
}

//giai phong Object
void free_object(Object *obj){
	free(obj);
}