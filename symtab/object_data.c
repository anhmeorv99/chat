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
			new->login.id = obj->login.id;
			new->login.is_admin = obj->login.is_admin;
			strcpy(new->login.name,obj->login.name);
			strcpy(new->login.username,obj->login.username);
			strcpy(new->login.password,obj->login.password);
			new->login.err = obj->login.err;
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
			new->chat_private.from_id = obj->chat_private.from_id;
			new->chat_private.to_id = obj->chat_private.to_id;
			strcpy(new->chat_private.from_name, obj->chat_private.from_name);
			strcpy(new->chat_private.to_name, obj->chat_private.to_name);
			strcpy(new->chat_private.from_username,obj->chat_private.from_username);
			strcpy(new->chat_private.to_username,obj->chat_private.to_username);
			strcpy(new->chat_private.message,obj->chat_private.message);
			strcpy(new->chat_private.create_at,obj->chat_private.create_at);
			break;
		case SIGNAL_CHANGE_PASSWORD:
			new->signal = obj->signal;
			strcpy(new->change_password.username, obj->change_password.username);
			strcpy(new->change_password.new_password, obj->change_password.new_password);
		case SIGNAL_CHAT_GROUP:
			new->signal = obj->signal;
			new->chat_group.ID_Room = obj->chat_group.ID_Room;
			new->chat_group.from_id = obj->chat_group.from_id;
			new->chat_group.length_to_member = obj->chat_group.length_to_member;
			strcpy(new->chat_group.from_name,obj->chat_group.from_name);
			strcpy(new->chat_group.message, obj->chat_group.message);
			if(obj->chat_group.length_to_member > 0){
				int i;
				for(i = 0; i < obj->chat_group.length_to_member; i++){
					new->chat_group.to_id_member[i] = obj->chat_group.to_id_member[i];
				}
			}
			break;
			
		case SIGNAL_RECV_ADD_MEMBER:
			new->signal = obj->signal;
			break;
		case SIGNAL_CREATE_ROOM:
			new->signal = obj->signal;
			break;
		case SIGNAL_CONFIRM_FRIEND:
			new->signal = obj->signal;
			break;
		case SIGNAL_NO_CONFIRM_FRIEND:
			new->signal = obj->signal;
			break;
		case SIGNAL_ADD_ROOM:
			new->signal = obj->signal;
			break;
		case SIGNAL_ADD_FRIEND:
			new->signal = obj->signal;
			break;
		case SIGNAL_LOGUOT:
			new->signal = obj->signal;
			break;
		case SIGNAL_RECV_LIST_FRIEND:
			new->signal = obj->signal;
			break;
		case SIGNAL_RECV_LIST_FRIEND_PRIVATE:
			new->signal = obj->signal;
			break;
		case SIGNAL_RECV_CHAT_PRIVATE:
			new->signal = obj->signal;
			break;
		case SIGNAL_ADMIN:
			new->signal = obj->signal;
			break;
		case SIGNAL_RECV_LIST_GROUP:
			new->signal = obj->signal;
			break;
		case SIGNAL_UPDATE_ADMIN:
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