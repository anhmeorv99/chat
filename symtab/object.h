

enum bool_{
	true,
	false
};
typedef enum bool_ bool;
enum Signals_ {
	SIGNAL_NONE,
	SIGNAL_LOGIN,
	SIGNAL_SIGNUP,
	SIGNAL_CHAT_PRIVATE
	//SIGNAL_CHAT_GROUP,
	//SIGNAL_LIST_FRIEND,
	//SIGNAL_CHANGE_PASSWORD,
	//SIGNAL_ADMIN,
	//SIGNAL_LOGUOT
};
typedef enum Signals_ Signals;


struct Login_ {
	char account[30];
	char password[30];
};

typedef struct Login_ Login;

struct Signup_ {
	char name[30];
	char account[30];
	char password[30];
	char re_password[30];
};

typedef struct Signup_ Signup;

struct Chat_Private_ {
	char from_username[30];
	char to_username[30];
	char message[200];
};

typedef struct Chat_Private_ Chat_Private;

struct Object_ {
	
	Signals signals;
	Login *login;
	Signup *signup;
	Chat_Private *chat_private;
};

typedef struct Object_ Object;

//-------------------------SIGNAL-------------------
//convect Signal: enum -> char*
void convect_signal_to_message(Signals signal,char *message);
//convect Message: char* -> enum
void convect_message_to_signal(char *message,Object *obj);
//----------------------Login--------------------
//cap phat Login
Login *new_login();
//giai phong Login
void free_login(Login *login);
//create Login
Login *make_login(char *account, char *password);
//copy Login
Login *duplicate_login(Login *login);
//so sanh 2 Login | giong : TRUE , khac : FALSE
bool equal_login(Login *login1,Login *login2);
//---------------------SIGNUP---------------------------
//cap phat Signup
Signup *new_signup();
//giai phong Signup
void free_signup(Signup *signup);
//create Signup
Signup *make_signup(char *name,char *account, 
					char *password, char *re_password);

//copy Signup
Signup *duplicate_signup(Signup *signup);
//so sanh 2 Signup
bool equal_signup(Signup *signup1, Signup *signup2);
//----------------------Chat Private-------------------
//cap phat Chat_Private
Chat_Private *new_chat_private();
//giai phong Chat_private
void free_chat_private(Chat_Private *chat);
//make chat_private
Chat_Private *make_chat_private(char *from_username, char *to_username, char *message);
//copy chat private
Chat_Private *duplicate_chat_private(Chat_Private *chat);
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
//cap phat chat private - object
Object *new_chat_private_object();
//create chat private - object
Object *make_chat_private_object(Chat_Private *chat);
//copy Object
Object *duplicate_object(Object *object);
//giai phong Object
void free_object(Object *obj);


