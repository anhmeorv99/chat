
typedef enum {
	false,
	true
}bool;

typedef struct {
	bool status = false;
	char friends[30];
}List_Friend;

typedef struct {
	char from_username[30];
	char message[200];
	char create_at[50];
}Message;

typedef struct {
	
	char to_username[30];
	Message msg_private[250];
	int length_message;
}Chat_Private;

typedef struct {
	Message msg_public;
	List_Friend to_username[12]; //co the co
}Chat_Public;

typedef struct {
	int ID_group;
	char name[30];
	char admin[30];
	List_Friend member[20];
	int length_member;
	Chat_Public chat_public[500];
	int length_chat_public;
}Group;

typedef struct {
	int ID_user;
	char name[30];
	char username[30];
	char password[30];
	bool is_admin = false;
	bool login_status = false;
	List_Friend list_friend[100];
	Chat_Private chat_private;
	Group group[20];
}User;