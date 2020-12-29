#include "jsonapi.h"
#include <curl/curl.h>


size_t write_data(void *ptr, size_t size, size_t nmemb, struct url_data *data) {
    size_t index = data->size;
    size_t n = (size * nmemb);
    char* tmp;

    data->size += (size * nmemb);

#ifdef DEBUG
    fprintf(stderr, "data at %p size=%ld nmemb=%ld\n", ptr, size, nmemb);
#endif
    tmp = realloc(data->data, data->size + 1); /* +1 for '\0' */

    if(tmp) {
        data->data = tmp;
    } else {
        if(data->data) {
            free(data->data);
        }
        fprintf(stderr, "Failed to allocate memory.\n");
        return 0;
    }

    memcpy((data->data + index), ptr, n);
    data->data[data->size] = '\0';

    return size * nmemb;
}

char *handle_url(char* url) {
    CURL *curl;

    struct url_data data;
    data.size = 0;
    data.data = malloc(4096); /* reasonable size initial buffer */
    if(NULL == data.data) {
        fprintf(stderr, "Failed to allocate memory.\n");
        return NULL;
    }

    data.data[0] = '\0';

    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() failed: %s\n",  
                        curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);

    }
    return data.data;
}

void convert_object_to_struct_user(){
	User item;
	FILE *fp;
	char buffer[1024];
	size_t length_eltype;
	size_t i;	
	struct json_object *parsed_json;
	struct json_object *elementType;
	fp = fopen("data.json","r");
	fread(buffer, 1024, 1, fp);
	fclose(fp);

	parsed_json = json_tokener_parse(buffer);

	length_eltype = json_object_array_length(parsed_json);
	

	for(i=0;i<length_eltype;i++) {
		elementType = json_object_array_get_idx(parsed_json, i);
		json_object_object_get_ex(elementType, "id", &item.id);
		json_object_object_get_ex(elementType, "name", &item.name);
		json_object_object_get_ex(elementType, "username", &item.username);
		json_object_object_get_ex(elementType, "password", &item.password);
		json_object_object_get_ex(elementType, "status", &item.status);
		json_object_object_get_ex(elementType, "created_at", &item.created_at);
		json_object_object_get_ex(elementType, "is_admin", &item.is_admin);

        printf("id = %d, name = %s, username = %s, password = %s, status = %d, created_at = %s, is_admin = %d \n",
			json_object_get_int(item.id),
			json_object_get_string(item.name),
			json_object_get_string(item.username),
			json_object_get_string(item.password),
			json_object_get_boolean(item.status),
			json_object_get_string(item.created_at),
			json_object_get_boolean(item.is_admin));
	}	
	

}

Data_base *getListFriend(char* element){
	Data_base *database = (Data_base*)malloc(sizeof(Data_base));
	Friend item;
	size_t length_eltype;
	size_t i;	
	friend_db friend;
	struct json_object *parsed_json;
	struct json_object *elementType;

	parsed_json = json_tokener_parse(element);

	length_eltype = json_object_array_length(parsed_json);
	database->length_list_friend = length_eltype;
	if (length_eltype>0){
		for(i=0;i<length_eltype;i++) {
		elementType = json_object_array_get_idx(parsed_json, i);
		json_object_object_get_ex(elementType, "id", &item.id);
		json_object_object_get_ex(elementType, "friend", &item._friend);
		json_object_object_get_ex(elementType, "confirm", &item.confirm);
		json_object_object_get_ex(elementType, "user", &item.user);
		database->list_friend[i] = getFriend(item);
	}	
	}
	

	return database;

}

// Data_base *getListRoom(char* element){
// 	Data_base *database = (Data_base*)malloc(sizeof(Data_base));
// 	Room item;
// 	size_t length_eltype, length_member;
// 	size_t i,j;	
// 	struct json_object *parsed_json;
// 	struct json_object *elementType;
// 	struct json_object *id_member;

// 	parsed_json = json_tokener_parse(element);

// 	length_eltype = json_object_array_length(parsed_json);

// 	for(i=0;i<length_eltype;i++) {
// 		elementType = json_object_array_get_idx(parsed_json, i);
// 		json_object_object_get_ex(elementType, "id", &item.id);
// 		json_object_object_get_ex(elementType, "room_status", &item.room_status);
// 		json_object_object_get_ex(elementType, "name", &item.name);
// 		json_object_object_get_ex(elementType, "admin_room", &item.admin_room);
// 		json_object_object_get_ex(elementType, "member", &item.member);
//         printf("id = %d, room_status = %s, name = %s, admin_room = %d, ",
// 			json_object_get_int(item.id),
// 			json_object_get_string(item.room_status),
// 			json_object_get_string(item.name),
// 			json_object_get_int(item.admin_room));
		
// 		printf("member = ");
// 		length_member = json_object_array_length(item.member);
// 		for(j=0;j<length_member;j++) {
// 			id_member = json_object_array_get_idx(item.member, j);
// 			printf("%d, ",json_object_get_int(id_member));
// 	}	
// 		printf("\n");

// 	}	
// }

Data_base *getMessagePrivate(char* element){
	Data_base *database = (Data_base*)malloc(sizeof(Data_base));
	Message item, member;
	size_t length_eltype, length_member;
	size_t i,j;	
	struct json_object *parsed_json;
	struct json_object *elementType, *profile_from_user;
	struct json_object *id_member;
	user_db profile_recv,profile;
	message_db msg_db;
	parsed_json = json_tokener_parse(element);

	length_eltype = json_object_array_length(parsed_json);
	database->length_chat_private = length_eltype;
	if (length_eltype>0){
		profile_from_user = json_object_array_get_idx(parsed_json, 0);
		json_object_object_get_ex(profile_from_user, "from_user", &member.from_user);
		json_object_object_get_ex(profile_from_user, "member", &member.member);

		profile = getUser(NULL, json_object_get_int(member.from_user)); 

		length_member = json_object_array_length(member.member);
			for(j=0;j<length_member;j++) {
				id_member = json_object_array_get_idx(member.member, j);

				if (json_object_get_int(id_member) != json_object_get_int(member.from_user)){
					
					profile_recv = getUser(NULL,json_object_get_int(id_member));
				}
			}
		// GET PROFILE of chat private

		for(i=0;i<length_eltype;i++) {
			elementType = json_object_array_get_idx(parsed_json, i);
			json_object_object_get_ex(elementType, "id", &item.id);
			json_object_object_get_ex(elementType, "from_user", &item.from_user);
			json_object_object_get_ex(elementType, "message", &item.message);
			json_object_object_get_ex(elementType, "created_at", &item.created_at);
			json_object_object_get_ex(elementType, "room", &item.room);
			json_object_object_get_ex(elementType, "member", &item.member);
			if (json_object_get_int(item.from_user)==profile.ID_user){
				msg_db = getMessage(item,profile);
				database->chat_private[i] =  getChatPrivate(msg_db,i,profile_recv);

			}else{ 
				msg_db = getMessage(item,profile_recv);
				database->chat_private[i] = getChatPrivate(msg_db,i,profile);
				
			}
	}
	
	

	}	
	return database;
}


const char *jsondumpsPassword(char *password){
	  	User user;
	    user.password = json_object_new_string(password);
		struct json_object *Eltype = json_object_new_object();
		json_object_object_add(Eltype,"password", user.password);
	return json_object_to_json_string(Eltype);
}

const char *convert_object_to_json_friend(Friend elememt){
	   struct json_object *Eltype = json_object_new_object();
	    json_object_object_add(Eltype,"id",elememt.id);
		json_object_object_add(Eltype,"friend",elememt._friend);
	    json_object_object_add(Eltype,"confirm",elememt.confirm);
	    json_object_object_add(Eltype,"user",elememt.user);
	return json_object_to_json_string(Eltype);
}

const char *convert_object_to_json_room(Room elememt){
	   struct json_object *Eltype = json_object_new_object();
	    json_object_object_add(Eltype,"id",elememt.id);
		json_object_object_add(Eltype,"room_status",elememt.room_status);
	    json_object_object_add(Eltype,"name",elememt.name);
	    json_object_object_add(Eltype,"admin_room",elememt.admin_room);
		json_object_object_add(Eltype,"member",elememt.member);
	return json_object_to_json_string(Eltype);
}

const char *convert_object_to_json_message(Message elememt){
	   struct json_object *Eltype = json_object_new_object();
	    json_object_object_add(Eltype,"id",elememt.id);
		json_object_object_add(Eltype,"from_user",elememt.from_user);
	    json_object_object_add(Eltype,"message",elememt.message);
	    json_object_object_add(Eltype,"created_at",elememt.created_at);
		json_object_object_add(Eltype,"room",elememt.room);
		json_object_object_add(Eltype,"member",elememt.member);
	return json_object_to_json_string(Eltype);
}

friend_db getFriend(Friend friend){
	friend_db Eltype;
	user_db profile = getUser(NULL, json_object_get_int(friend._friend)); 
	Eltype.ID = json_object_get_int(friend._friend); //id friend
	Eltype.confirm = json_object_get_boolean(friend.confirm); //confirm
	strcpy(Eltype.name,profile.name);
	strcpy(Eltype.username, profile.username);
	return Eltype;
}

member_db getMember(User profile){
	member_db Eltype;
	Eltype.ID = json_object_get_int(profile.id);
	strcpy(Eltype.name,json_object_get_string(profile.name));
	strcpy(Eltype.username, json_object_get_string(profile.username));
	return Eltype;
}

message_db getMessage(Message message,user_db profile){
	message_db Eltype;
	Eltype.id_username = profile.ID_user;
	strcpy(Eltype.from_username, profile.username);
	strcpy(Eltype.from_name, profile.name);
	strcpy(Eltype.message, json_object_get_string(message.message));
	strcpy(Eltype.create_at, json_object_get_string(message.created_at));
	return Eltype;
}

Chat_Private_ getChatPrivate(message_db message,int index, user_db profile){
	Chat_Private_ Eltype;
	// Eltype.ID = message.ID;
	strcpy(Eltype.to_username, profile.username);
	strcpy(Eltype.to_name, profile.name);
	
	Eltype.length_message = index;
	Eltype.msg_private[index] = message;

	return Eltype;
}

group_db getGroupDB(Room room){
	group_db Eltype;
	Eltype.ID_group = json_object_get_int(room.id);
	strcpy(Eltype.name,json_object_get_string(room.name));
	Eltype.id_admin = json_object_get_int(room.admin_room);
	// Eltype.members[index_member] = member;
	// Eltype.length_member = index_member;
	// Eltype.msg_public[index_message] = message;
	// Eltype.length_msg_public = index_message;

	return Eltype;
}

user_db getUserDB(User user){
	user_db Eltype;
	Eltype.ID_user = json_object_get_int(user.id);
	strcpy(Eltype.name, json_object_get_string(user.name));
	strcpy(Eltype.username, json_object_get_string(user.username));
	strcpy(Eltype.password, json_object_get_string(user.password));
	Eltype.is_admin = json_object_get_boolean(user.is_admin);
	Eltype.login_status = json_object_get_boolean(user.status);
	return Eltype;

}

// Data_base getDatabase(user_db user, friend_db *friend, int len_friend, Chat_Private_ *chat_private, 
// 						int len_chat_private, group_db *group, int len_group){
		
// 		Data_base Eltype;
// 		int i;
// 		Eltype.signal = DB_NONE;
// 		Eltype.user = user;
// 		for (i=0;i < len_friend;i++){
// 			Eltype.list_friend[i] = friend[i];
// 		}
// 		for (i=0;i < len_chat_private;i++){
// 			Eltype.chat_private[i] = chat_private[i];
// 		}
// 		for (i=0;i < len_group;i++){
// 			Eltype.group[i] = group[i];
// 		}

// 		Eltype.length_list_friend = len_friend;
// 		Eltype.length_chat_private = len_chat_private;
// 		Eltype.length_group = len_group;

// 		return Eltype;
// }

int check_user(char* username){
	size_t length_eltype;
	struct json_object *parsed_json;
	char *buffer;
	char url[]= "http://127.0.0.1:8000/api/user/?username=";
	strcat(url,username);
 	buffer = handle_url(url);

    if(buffer) {
        parsed_json = json_tokener_parse(buffer);

		length_eltype = json_object_array_length(parsed_json);
		if (length_eltype == 0) return -1;
		else return 1;
        free(buffer);
    }
	return -1;
}

user_db getUser(char* username, int id){
	User item;
	char *buffer;
	char *url = (char*)malloc(200*sizeof(char));
	size_t length_eltype;
	size_t i;	
	struct json_object *parsed_json;
	struct json_object *elementType;
	if (id == -1){
		sprintf(url,"http://127.0.0.1:8000/api/user/?username=%s",username);
		buffer = handle_url(url);
	if(buffer) {
	parsed_json = json_tokener_parse(buffer);

	length_eltype = json_object_array_length(parsed_json);
	
	for (i = 0; i < length_eltype ; i++){
		elementType = json_object_array_get_idx(parsed_json, i);
		json_object_object_get_ex(elementType, "id", &item.id);
		json_object_object_get_ex(elementType, "name", &item.name);
		json_object_object_get_ex(elementType, "username", &item.username);
		json_object_object_get_ex(elementType, "password", &item.password);
		json_object_object_get_ex(elementType, "status", &item.status);
		json_object_object_get_ex(elementType, "created_at", &item.created_at);
		json_object_object_get_ex(elementType, "is_admin", &item.is_admin);
		return getUserDB(item);
	}
	     free(buffer);
    }
	}
	else
	{
		sprintf(url,"http://127.0.0.1:8000/api/user/%d/",id);
		buffer = handle_url(url);
		if(buffer) {
		parsed_json = json_tokener_parse(buffer);
			elementType = json_object_get(parsed_json);
			json_object_object_get_ex(elementType, "id", &item.id);
			json_object_object_get_ex(elementType, "name", &item.name);
			json_object_object_get_ex(elementType, "username", &item.username);
			json_object_object_get_ex(elementType, "password", &item.password);
			json_object_object_get_ex(elementType, "status", &item.status);
			json_object_object_get_ex(elementType, "created_at", &item.created_at);
			json_object_object_get_ex(elementType, "is_admin", &item.is_admin);
			return getUserDB(item);
		
			free(buffer);
		}
	}
}

void postUser(user_db user){
  CURL *curl;
  CURLcode res;
 
  /* In windows, this will init the winsock stuff */ 
  curl_global_init(CURL_GLOBAL_ALL);
 
  /* get a curl handle */ 
  curl = curl_easy_init();
  if(curl) {
    /* First set the URL that is about to receive our POST. This URL can
       just as well be a https:// URL if that is what should receive the
       data. */ 
    curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8000/api/user/");
    /* Now specify the POST data */ 
	char *data = (char*)malloc(200*sizeof(char)); 
	sprintf(data,"name=%s&username=%s&password=%s",user.name, user.username, user.password);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
 
    /* Perform the request, res will get the return code */ 
    res = curl_easy_perform(curl);
    /* Check for errors */ 
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
 
    /* always cleanup */ 
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
}


void changePassword(char* username, char* newpassword){
	user_db user = getUser(username,-1);
	CURL *curl;
	CURLcode res;
	long http_code;
	char url[]= "http://127.0.0.1:8000/api/user/";
	sprintf(url,"%s%d/", url,user.ID_user);
	char data[100];
	sprintf(data,"/?password=%s",newpassword);
	curl = curl_easy_init();
	curl_global_init(CURL_GLOBAL_ALL);

	if(curl){
		
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);


		//enable to spit out information for debugging
		//curl_easy_setopt(curl, CURLOPT_VERBOSE,1L); 

		res = curl_easy_perform(curl);

		if (res != CURLE_OK){
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res) );
		}

		printf("\nget http return code\n");
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
		printf("http code: %lu\n", http_code );


		curl_easy_cleanup(curl);
		curl_global_cleanup();

		}
}

void loginStatus(char* username,int status){
	user_db user = getUser(username,-1);
	CURL *curl;
	CURLcode res;
	long http_code;
	char *url= (char*)malloc(100*sizeof(char));
	sprintf(url,"http://127.0.0.1:8000/api/user/%d/?status=%d", user.ID_user, status);
	curl = curl_easy_init();
	curl_global_init(CURL_GLOBAL_ALL);

	if(curl){
		
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");

		//enable to spit out information for debugging
		//curl_easy_setopt(curl, CURLOPT_VERBOSE,1L); 

		res = curl_easy_perform(curl);

		if (res != CURLE_OK){
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res) );
		}

		printf("\nget http return code\n");
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
		printf("http code: %lu\n", http_code );


		curl_easy_cleanup(curl);
		curl_global_cleanup();

		}
	free(url);
}
int main(int argc, char **argv) {
	
	// convert_object_to_struct_user();
	// convert_object_to_struct_room();
	// convert_object_to_struct_message();
	// convert_object_to_struct_friend();

	size_t i;
	Data_base *db = (Data_base*)malloc(100*sizeof(Data_base));
	char *buffer = (char*)malloc(1000*sizeof(char));
	char * buffer_friend = (char*)malloc(1000*sizeof(char));
	char url_friend[] = "http://127.0.0.1:8000/api/friends/?user=1";
	char url[]= "http://127.0.0.1:8000/api/message/?room_id=2";
 	buffer = handle_url(url);
	buffer_friend = handle_url(url_friend);
	if (buffer_friend){
		db = getListFriend(buffer_friend);
	}
	for (i=0;i<db->length_list_friend;i++)
	{
		printf("%s \n", db->list_friend[i].name);
	}
    //  if (buffer){
    //      db = getMessagePrivate(buffer);
    //  }
	
	// printf("%d\n",db->length_chat_private);
	// for (i =0 ; i< db->length_chat_private;i++)
	// {
	// 	printf("from %s to %s : %s\n",db->chat_private[i].msg_private[i].from_name,db->chat_private[i].to_name, db->chat_private[i].msg_private[i].message);
	// }
	free(buffer);
	free(db);
	
	return 1;
}