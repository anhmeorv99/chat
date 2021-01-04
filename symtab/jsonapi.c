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


Data_base *getListFriend(char* element){
	Data_base *database = (Data_base*)malloc(sizeof(Data_base));
	Friend item;
	size_t length_eltype;
	size_t i;	
	// friend_db friend_;
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



friend_db getFriend(Friend friend){
	friend_db Eltype;
	user_db profile = getUser(NULL, json_object_get_int(friend._friend)); 
	Eltype.ID = json_object_get_int(friend._friend); //id friend
	Eltype.confirm = json_object_get_boolean(friend.confirm); //confirm
	strcpy(Eltype.name,profile.name);
	strcpy(Eltype.username, profile.username);
	return Eltype;
}

member_db getMember(int id){
	member_db Eltype;
	user_db profile = getUser(NULL,id);
	Eltype.ID = profile.ID_user;
	strcpy(Eltype.name,profile.name);
	strcpy(Eltype.username,profile.username);
	return Eltype;
}

message_db getMessage(Message message,user_db from_profile, user_db to_profile){
	message_db Eltype;

	strcpy(Eltype.from_username, from_profile.username);
	
	strcpy(Eltype.to_username, to_profile.username);
	strcpy(Eltype.from_name, from_profile.name);
	strcpy(Eltype.to_name, to_profile.name);
	strcpy(Eltype.message, json_object_get_string(message.message));
	strcpy(Eltype.create_at, json_object_get_string(message.created_at));
	return Eltype;
}

message_db getOneMessagePrivate(MessagePrivate message,user_db from_profile, user_db to_profile){
	message_db Eltype;
	strcpy(Eltype.from_username, from_profile.username);
	strcpy(Eltype.to_username, to_profile.username);
	strcpy(Eltype.from_name, from_profile.name);
	strcpy(Eltype.to_name, to_profile.name);
	strcpy(Eltype.message, json_object_get_string(message.message));
	strcpy(Eltype.create_at, json_object_get_string(message.created_at));
	return Eltype;
}

Data_base *getMessagePrivate(int from_user, int to_user)
{
	Data_base *database = (Data_base*)malloc(sizeof(Data_base));
	MessagePrivate item;
	size_t length_eltype;
	size_t i;	
	struct json_object *parsed_json;
	struct json_object *elementType;
	user_db profile_recv,profile;
	message_db msg_db;

	profile = getUser(NULL, from_user);
	profile_recv = getUser(NULL, to_user);
	char* url = (char*)malloc(100*sizeof(char));
	sprintf(url, "http://127.0.0.1:8000/api/private/?from_user=%d&to_user=%d", from_user, to_user);
	// get data
	char* element = (char*)malloc(100*sizeof(char));
	element = handle_url(url);
	
	parsed_json = json_tokener_parse(element);

	length_eltype = json_object_array_length(parsed_json);
	database->chat_private.length_message =length_eltype;
		for(i=0;i<length_eltype;i++) {
			elementType = json_object_array_get_idx(parsed_json, i);
			json_object_object_get_ex(elementType, "from_user", &item.from_user);
			json_object_object_get_ex(elementType, "message", &item.message);
			json_object_object_get_ex(elementType, "created_at", &item.created_at);
			json_object_object_get_ex(elementType, "to_user", &item.to_user);
			if (profile.ID_user == json_object_get_int(item.from_user)){
					msg_db = getOneMessagePrivate(item, profile, profile_recv);
			} else{
				msg_db = getOneMessagePrivate(item, profile_recv ,profile);
			}
			
			database->chat_private.msg_private[i] = msg_db;
	}
	
	return database;
}

char *convert_struct_to_json_message(int from_user, char* message, int room, int member[], int size){
    char *buffer = (char*)malloc(200*sizeof(char));
    char *id = (char*)malloc(sizeof(char));
    char *data = (char*)malloc(200*sizeof(char));
    int i;
    sprintf(buffer,"[");
    for (i=0;i<size;i++){
        if (i==size-1){
             sprintf(id,"%d]",member[i]);
             strcat(buffer,id);
        }else {
            sprintf(id,"%d, ",member[i]);
            strcat(buffer,id);
        }
      
    }
   
    sprintf(data,"{\"from_user\": %d, \"message\": \"%s\", \"room\": %d, \"member\": %s}", from_user,message,room,buffer);
    free(buffer);
    return data;
}

void postMessage(int from_user, int to_user, char* message){
     CURL *curl;
     CURLcode res;
	char* data = (char*)malloc(200*sizeof(char));
	sprintf(data,"{\"from_user\":%d, \"message\": \"%s\", \"to_user\":%d}",
	 from_user,message,to_user);
  /* In windows, this will init the winsock stuff */ 
  curl_global_init(CURL_GLOBAL_ALL);
 
  /* get a curl handle */ 
  curl = curl_easy_init();
  if(curl) {
    struct curl_slist *chunk = NULL; 
    chunk = curl_slist_append(chunk, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
    
    curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8000/api/private/");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
    res = curl_easy_perform(curl);
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
}


// Chat_Private_ getChatPrivate(message_db message,int index){
// 	Chat_Private_ Eltype;
// 	// Eltype.ID = message.ID;
// 	Eltype.msg_private[index] = message;
// 	return Eltype;
// }


int get_id_room_private(char* username1, char* username2){
	user_db id_user1 = getUser(username1, -1);
	user_db id_user2 = getUser(username2, -1);
	struct json_object *parsed_json;
	struct json_object *elementType;
	size_t length_eltype,i;
	Room item;
	char * url = (char*)malloc(100*sizeof(char));
	char * buffer = (char*)malloc(100*sizeof(char));
	sprintf(url, "http://127.0.0.1:8000/api/room/?room_status=1&member=%d,%d", id_user1.ID_user,id_user2.ID_user);
	buffer = handle_url(url);
	if (buffer){

	parsed_json = json_tokener_parse(buffer);

	length_eltype = json_object_array_length(parsed_json);
	if (length_eltype>0){
		for(i=0;i<length_eltype;i++) {
			elementType = json_object_array_get_idx(parsed_json, i);
			json_object_object_get_ex(elementType, "id", &item.id);
				return json_object_get_int(item.id);
			}	
		}
	}
	return -1;
}


// group_db getGroupDB(Room room, member_db member, message_db message, int index_member, int index_message){
// 	group_db Eltype;
// 	Eltype.ID_group = json_object_get_int(room.id);
// 	strcpy(Eltype.name,json_object_get_string(room.name));
// 	Eltype.id_admin = json_object_get_int(room.admin_room);
// 	Eltype.members[index_member] = member;
// 	Eltype.length_member = index_member;
// 	Eltype.msg_public[index_message] = message;
// 	Eltype.length_msg_public = index_message;
// 	return Eltype;
// }

// Data_base *getGroup(char* element){
// 	Data_base *database = (Data_base*)malloc(sizeof(Data_base));
// 	Room item;
// 	size_t length_eltype, length_member;
// 	size_t i,j;	
// 	group_db group;
// 	member_db member[12];
// 	message_db message;
// 	struct json_object *parsed_json;
// 	struct json_object *elementType;
// 	struct json_object *id_member;

// 	parsed_json = json_tokener_parse(element);

// 	length_eltype = json_object_array_length(parsed_json);

// 	for(i=0;i<length_eltype;i++) {
// 		elementType = json_object_array_get_idx(parsed_json, i);
// 		json_object_object_get_ex(elementType, "id", &item.id);
// 		json_object_object_get_ex(elementType, "name", &item.name);
// 		json_object_object_get_ex(elementType, "admin_room", &item.admin_room);
// 		json_object_object_get_ex(elementType, "member", &item.member);
// 		length_member = json_object_array_length(item.member);
// 		for(j=0;j<length_member;j++) {
// 			id_member = json_object_array_get_idx(item.member, j);
// 			member[j] = getMember(id_member);
// 	}	
// 	}	
// }

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
// int main(int argc, char **argv) {
	
	// convert_object_to_struct_user();
	// convert_object_to_struct_room();
	// convert_object_to_struct_message();
	// convert_object_to_struct_friend();

	// size_t i;
	// Data_base *db = (Data_base*)malloc(100*sizeof(Data_base));
	// char *buffer = (char*)malloc(1000*sizeof(char));
	// char * buffer_friend = (char*)malloc(1000*sizeof(char));
	// // char url_friend[] = "http://127.0.0.1:8000/api/friends/?user=1";
	// char url[]= "http://127.0.0.1:8000/api/message/?user=1";
 	// buffer = handle_url(url);
	// buffer_friend = handle_url(url_friend);
	// if (buffer){
	// 	db = getListRoom(buffer);
	// }
	
    // db = getMessagePrivate(1,2);
	
	// printf("%d\n",db->chat_private.length_message);
	// for (i =0 ; i< db->chat_private.length_message;i++)
	// {
	// 	printf("from %s to %s : %s\n",db->chat_private.msg_private[i].from_name,db->chat_private.msg_private[i].to_name, db->chat_private.msg_private[i].message);
	// }
	// // free(buffer);
	// free(db);

	// postMessage(1,2,"Day la test");
	
	// return 1;
// }