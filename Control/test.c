#include "jsonapi.h"

int main(int argc, char **argv) {
	
	// convert_object_to_struct_user();
	// convert_object_to_struct_room();
	// convert_object_to_struct_message();
	// convert_object_to_struct_friend();
	User user;
	user.id = json_object_new_int(1);
	user.is_admin = json_object_new_boolean(FALSE);
	user.status = json_object_new_boolean(FALSE);
	user.created_at = json_object_new_string("12121212121");
	user.name = json_object_new_string("Tuan Anh");
	user.username = json_object_new_string("anh.nt");
	user.password = json_object_new_string("1234455");

	// const char *a = convert_object_to_json_user(user);
	// printf("%s", a);
	// user_db test = getUserDB(user);
	// printf("%s", test.username);
	
	user_db userdb;
	int check = check_user("hello");
	if (check==1){
		userdb = getUser("hello");
		printf("%s\n", userdb.username); 
	}
	
	return 1;
}