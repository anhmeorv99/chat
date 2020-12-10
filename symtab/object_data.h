#include <glib.h>

enum Signals {
	SIGNAL_NONE,
	SIGNAL_LOGIN,
	SIGNAL_SIGNUP,
	//SIGNAL_CHAT_PRIVATE,
	//SIGNAL_CHAT_GROUP,
	//SIGNAL_LIST_FRIEND,
	//SIGNAL_CHANGE_PASSWORD,
	//SIGNAL_ADMIN,
	//SIGNAL_LOGUOT
};

struct Login_ {
	gchar *account;
	gchar *password;
};

typedef struct Login_ Login;

struct Signup_ {
	gchar *name;
	gchar *account;
	gchar *password;
	gchar *re_password;
};

typedef struct Signup_ Signup;

struct Object_ {
	enum Signals signals;
	Login *login;
	Signup *signup;
};

typedef struct Object_ Object;

//---------------------------LOGIN-------------------------
//cap phat struct Login
Login *new_login();
//giai phong Login
void free_login(Login *login);
//create Login
Login *make_login(gchar *account, gchar *password);
//copy Login
Login *duplicate_login(Login *login);
//so sanh 2 Login | giong : TRUE , khac : FALSE
gboolean equal_login(Login *login1,Login *login2);
//---------------------SIGNUP---------------------------
//cap phat Signup
Signup *new_signup();
//giai phong Signup
void free_signup(Signup *signup);
//create Signup
Signup *make_signup(gchar *name,gchar *account, 
					gchar *password, gchar *re_password);

//copy Signup
Signup *duplicate_signup(Signup *signup);
//so sanh 2 Signup
gboolean equal_signup(Signup *signup1, Signup *signup2);
//-------------------------Object------------------------
//cap phat tat ca trong Object
Object *new_object_all();
//giai phong tat ca trong Object
void free_object_all(Object *obj);
//cap phat Login - Object
Object *new_login_object();
//create Login - Object
Object *make_login_object(Login *login);
//cap phat Signup - Object
Object *new_signup_object();
//create Signup - Object
Object *make_signup_object(Signup *signup);
//copy Object
Object *duplicate_object(Object *object);
//giai phong Object
void free_object(Object *obj);



