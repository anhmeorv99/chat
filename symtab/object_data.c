
#include "object_data.h"
//---------------------------LOGIN-------------------------
//cap phat struct Login
Login *new_login(){
	Login *login = g_slice_new(Login);
	return login;
}
//giai phong Login
void free_login(Login *login){
	g_slice_free(Login,login);
}
//create Login
Login *make_login(gchar *account, gchar *password){
	Login *login = new_login();
	login->account = g_strdup(account);
	login->password = g_strdup(password);
	return login;
}
//copy Login
Login *duplicate_login(Login *login){
	Login *log_in = new_login();
	log_in->account = g_strdup(login->account);
	log_in->password = g_strdup(login->password);
	return log_in;
}
//so sanh 2 Login | giong : TRUE , khac : FALSE
gboolean equal_login(Login *login1,Login *login2){
	if(g_strcmp0(login1->account,login2->account) != 0)
		return FALSE;
	if(g_strcmp0(login1->password,login2->password) != 0)
		return FALSE;
	return TRUE;
}
//---------------------SIGNUP---------------------------
//cap phat Signup
Signup *new_signup(){
	Signup *signup = g_slice_new(Signup);
	return signup;
}
//giai phong Signup
void free_signup(Signup *signup){
	g_slice_free(Signup,signup);
}
//create Signup
Signup *make_signup(gchar *name,gchar *account, 
					gchar *password, gchar *re_password)
{
	Signup *signup = new_signup();
	signup->name = g_strdup(name);
	signup->account = g_strdup(account);
	signup->password = g_strdup(password);
	signup->re_password = g_strdup(re_password);
	return signup;
}
//copy Signup
Signup *duplicate_signup(Signup *signup){
	Signup *sign_up = new_signup();
	sign_up->name = g_strdup(signup->name);
	sign_up->account = g_strdup(signup->account);
	sign_up->password = g_strdup(signup->password);
	sign_up->re_password = g_strdup(signup->re_password);
	return sign_up;
}
//so sanh 2 Signup
gboolean equal_signup(Signup *signup1, Signup *signup2){
	if(g_strcmp0(signup1->name,signup2->name) != 0)
		return FALSE;
	if(g_strcmp0(signup1->account,signup2->account) != 0)
		return FALSE;
	if(g_strcmp0(signup1->password,signup2->password) != 0)
		return FALSE;
	if(g_strcmp0(signup1->re_password,signup2->re_password) != 0)
		return FALSE;
	return TRUE;
}
//-------------------------Object------------------------
//cap phat tat ca trong Object
Object *new_object_all(){
	Object *obj = g_slice_new(Object);
	obj->signals = SIGNAL_NONE;
	obj->login = new_login();
	obj->signup = new_signup();
	return obj;
}
//giai phong tat ca trong Object
void free_object_all(Object *obj){
	free_login(obj->login);
	free_signup(obj->signup);
	g_slice_free(Object,obj);
}


//cap phat Login - Object
Object *new_login_object(){
	Object *obj = g_slice_new(Object);
	obj->signals = SIGNAL_LOGIN;
	obj->login = new_login();
	return obj;
}
//create Login - Object
Object *make_login_object(Login *login){
	Object *obj = g_slice_new(Object);
	obj->signals = SIGNAL_LOGIN;
	obj->login = duplicate_login(login);
	return obj;
}
//cap phat Signup - Object
Object *new_signup_object(){
	Object *obj = g_slice_new(Object);
	obj->signals = SIGNAL_SIGNUP;
	obj->signup = new_signup();
	return obj;
}
//create Signup - Object
Object *make_signup_object(Signup *signup){
	Object *obj = g_slice_new(Object);
	obj->signals = SIGNAL_SIGNUP;
	obj->signup = duplicate_signup(signup);
	return obj;
}
//copy Object
Object *duplicate_object(Object *object){
	Object *obj = g_slice_new(Object);
	switch(object->signals){
		case SIGNAL_LOGIN:
			obj->signals = object->signals;
			obj->login = duplicate_login(object->login);
			break;
		case SIGNAL_SIGNUP:
			obj->signals = object->signals;
			obj->signup = duplicate_signup(object->signup);
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
		case SIGNAL_NONE:
			break;
	}
	g_slice_free(Object,obj);
}