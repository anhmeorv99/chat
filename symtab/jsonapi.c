#include "jsonapi.h"
#include <curl/curl.h>

char host[] = "http://192.168.43.215:8000";

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
    data.data = malloc(10000); /* reasonable size initial buffer */
    if(NULL == data.data) {
        fprintf(stderr, "Failed to allocate memory.\n");
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

			return NULL;
        } 
        curl_easy_cleanup(curl);

    }
    return data.data;
}


Data_base *getListFriend(int id){
	Data_base *database = (Data_base*)malloc(sizeof(Data_base));
	Friend item;
	size_t length_eltype, length_eltype1;
	size_t i;	
	char* url = (char*)malloc(200*sizeof(char));
	char* url1 = (char*)malloc(200*sizeof(char));
    sprintf(url, "%s/api/friends/?user=%d",host, id);
    sprintf(url1, "%s/api/friends/?friend=%d",host, id);

	// friend_db friend_;
	struct json_object *parsed_json, *parsed_json1;
	struct json_object *elementType, *elementType1;
	char* element = handle_url(url);	
	char* element1 = handle_url(url1);	
	parsed_json = json_tokener_parse(element);
	parsed_json1 = json_tokener_parse(element1);

// length
	length_eltype = json_object_array_length(parsed_json);
	length_eltype1 =  json_object_array_length(parsed_json1);
	int dem = 0;
// data ?user=%d
	if (length_eltype>0){
		for(i=0;i<length_eltype;i++) {
		elementType = json_object_array_get_idx(parsed_json, i);
		json_object_object_get_ex(elementType, "id", &item.id);
		json_object_object_get_ex(elementType, "friend", &item._friend);
		json_object_object_get_ex(elementType, "confirm", &item.confirm);
		json_object_object_get_ex(elementType, "user", &item.user);
		if(json_object_get_boolean(item.confirm) == TRUE){
			database->list_friend.list_friend[dem] = getFriend(item, 1);
			dem++;
			}
		}	
	}

	if (length_eltype1>0){
		for(i=0;i<length_eltype1;i++) {
		elementType1 = json_object_array_get_idx(parsed_json1, i);
		json_object_object_get_ex(elementType1, "id", &item.id);
		json_object_object_get_ex(elementType1, "friend", &item._friend);
		json_object_object_get_ex(elementType1, "confirm", &item.confirm);
		json_object_object_get_ex(elementType1, "user", &item.user);
		database->list_friend.list_friend[i+dem] = getFriend(item, 0);
		}	
	}
		database->list_friend.length_list_friend = dem + length_eltype1;
	return database;

}



friend_db getFriend(Friend friend, int mode){
	friend_db Eltype;
	if (mode == 1){
		user_db profile = getUser(NULL, json_object_get_int(friend._friend)); 
		Eltype.ID = json_object_get_int(friend._friend); //id friend
		Eltype.confirm = json_object_get_boolean(friend.confirm); //confirm
		Eltype.loginStatus = profile.login_status;
		strcpy(Eltype.name,profile.name);
		strcpy(Eltype.username, profile.username);
		return Eltype;
	}

	user_db profile = getUser(NULL, json_object_get_int(friend.user)); 
		Eltype.ID = json_object_get_int(friend.user); //id friend
		Eltype.confirm = json_object_get_boolean(friend.confirm); //confirm
		Eltype.loginStatus = profile.login_status;
		strcpy(Eltype.name,profile.name);
		strcpy(Eltype.username, profile.username);
		return Eltype;

}

void requestData(char*url, char*data, char* method){
	CURL *curl;
	CURLcode res = CURLE_OK;
	curl = curl_easy_init();
	curl_global_init(CURL_GLOBAL_ALL);
	 if(curl) {
			struct curl_slist *chunk = NULL; 
			chunk = curl_slist_append(chunk, "Content-Type: application/json");
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
			
			curl_easy_setopt(curl, CURLOPT_URL, url);
			curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);
			if (data!=NULL){
				curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
			}
			res = curl_easy_perform(curl);
			if(res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
					curl_easy_strerror(res));
			curl_easy_cleanup(curl);
			curl_slist_free_all(chunk);
			return;
		}
	
}

void create_room_(char* name, int admin_room){
	char* data = (char*)malloc(100*sizeof(char));
	sprintf(data, "{\"name\": \"%s\", \"member\": [%d], \"admin_room\" : %d}", name, admin_room, admin_room);
	char *url = (char*)malloc(100*sizeof(char));
	sprintf(url,"%s/api/room/", host );
	

	// printf("%s\n", data);
	requestData(url, data, "POST");
}

int check_friend(int id_user, int id_friend){
	Friend item;
	char* url = (char*)malloc(100*sizeof(char));
	char* url1 = (char*)malloc(100*sizeof(char));
	char* buffer = (char*)malloc(100*sizeof(char));
	char* buffer1 = (char*)malloc(100*sizeof(char));
	sprintf(url, "%s/api/friends/?user=%d&friend=%d",host,id_user, id_friend);
	sprintf(url1, "%s/api/friends/?user=%d&friend=%d",host, id_friend, id_user);
	buffer = handle_url(url);
	buffer1 = handle_url(url1);
	int length_eltype,i;
	struct json_object *parsed_json;
	struct json_object *elementType;
	if (buffer){
		parsed_json = json_tokener_parse(buffer);
		length_eltype = json_object_array_length(parsed_json);
		if (length_eltype>0){
			for(i=0;i<length_eltype;i++) {
				elementType = json_object_array_get_idx(parsed_json, i);
				json_object_object_get_ex(elementType, "confirm", &item.confirm);
				if (json_object_get_boolean(item.confirm)==FALSE) return 2; // cho xac nhan
				else return 1; // la ban be	
			}
		}
	} 
	if (buffer1){
		parsed_json = json_tokener_parse(buffer1);
		length_eltype = json_object_array_length(parsed_json);
		if (length_eltype>0){
			for(i=0;i<length_eltype;i++) {
				elementType = json_object_array_get_idx(parsed_json, i);
				json_object_object_get_ex(elementType, "confirm", &item.confirm);
				if (json_object_get_boolean(item.confirm)==FALSE) return 2; // cho xac nhan
				else return 1; // la ban be	
			}
		}
	}
	
	return -1;
}

void update_confirm_friend(int user, int friend){

	Friend item_user;
	char* url_user = (char*)malloc(100*sizeof(char));
	char* buffer_user = (char*)malloc(100*sizeof(char));

	char* url_update_user = (char*)malloc(100*sizeof(char));

	char data[] = "{\"confirm\": true}";
	int i, length_eltype;

	sprintf(url_user, "%s/api/friends/?user=%d&friend=%d",host, friend,user);
	buffer_user = handle_url(url_user);
	struct json_object *parsed_json;
	struct json_object *elementType;
	if (buffer_user){
		parsed_json = json_tokener_parse(buffer_user);
		length_eltype = json_object_array_length(parsed_json);
		if (length_eltype>0){
			for(i=0;i<length_eltype;i++) {
			elementType = json_object_array_get_idx(parsed_json, i);
			json_object_object_get_ex(elementType, "id", &item_user.id);
			}	
		}
		sprintf(url_update_user, "%s/api/friends/%d/",host, json_object_get_int(item_user.id));
	}		
	requestData(url_update_user,data, "PATCH");

} 

void delete_confirm_friend(int user, int friend){

	Friend item_user;
	char* url_user = (char*)malloc(100*sizeof(char));
	char* buffer_user = (char*)malloc(100*sizeof(char));

	char* url_update_user = (char*)malloc(100*sizeof(char));

	int i, length_eltype;

	sprintf(url_user, "%s/api/friends/?user=%d&friend=%d",host, friend,user);
	buffer_user = handle_url(url_user);
	struct json_object *parsed_json;
	struct json_object *elementType;
	if (buffer_user){
		parsed_json = json_tokener_parse(buffer_user);
		length_eltype = json_object_array_length(parsed_json);
		if (length_eltype>0){
			for(i=0;i<length_eltype;i++) {
			elementType = json_object_array_get_idx(parsed_json, i);
			json_object_object_get_ex(elementType, "id", &item_user.id);
			}	
		}
		sprintf(url_update_user, "%s/api/friends/%d/",host, json_object_get_int(item_user.id));
	}		
	requestData(url_update_user, NULL, "DELETE");

} 

void delete_user(int user){
	char* url_delete_user = (char*)malloc(100*sizeof(char));
	sprintf(url_delete_user, "%s/api/user/%d/",host, user);
	requestData(url_delete_user, NULL, "DELETE");
} 


member_db getMember(int id){
	member_db Eltype;
	user_db profile = getUser(NULL,id);
	Eltype.ID = profile.ID_user;
	strcpy(Eltype.name,profile.name);
	strcpy(Eltype.username,profile.username);
	return Eltype;
}

message_db getOneMessageGroup(Message message,user_db from_profile){
	message_db Eltype;
	Eltype.from_id = from_profile.ID_user;
	strcpy(Eltype.from_username, from_profile.username);
	strcpy(Eltype.from_name, from_profile.name);
	strcpy(Eltype.message, json_object_get_string(message.message));
	strcpy(Eltype.create_at, json_object_get_string(message.created_at));
	return Eltype;
}
Data_base *getMessageGroup(int room){
	Data_base *database = (Data_base*)malloc(sizeof(Data_base));
	Message item;
	size_t length_eltype;
	size_t i;	
	struct json_object *parsed_json;
	struct json_object *elementType;
	user_db profile;
	char* url = (char*)malloc(100*sizeof(char));
	sprintf(url, "%s/api/message/?room_id=%d",host, room);
	
	char* element = (char*)malloc(100*sizeof(char));
	element = handle_url(url);
	parsed_json = json_tokener_parse(element);
	
	length_eltype = json_object_array_length(parsed_json);
	
	database->list_group.group[0].length_msg_public = length_eltype;

		for(i=0;i<length_eltype;i++) {
			elementType = json_object_array_get_idx(parsed_json, i);
			json_object_object_get_ex(elementType, "from_user", &item.from_user);
			json_object_object_get_ex(elementType, "message", &item.message);
			json_object_object_get_ex(elementType, "created_at", &item.created_at);
			profile = getUser(NULL, json_object_get_int(item.from_user));
			database->list_group.group[0].msg_public[i] = getOneMessageGroup(item, profile);
	}
	return database;
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

Data_base_chat_private *getMessagePrivate(int from_user, int to_user)
{
	Data_base_chat_private *database = (Data_base_chat_private*)malloc(sizeof(Data_base_chat_private));
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
	sprintf(url, "%s/api/private/?from_user=%d&to_user=%d",host, from_user, to_user);
	// get data
	char* element = (char*)malloc(500*sizeof(char));
	char* len_str = (char*)malloc(4*sizeof(char));
	element = handle_url(url);
	
	parsed_json = json_tokener_parse(element);

	length_eltype = json_object_array_length(parsed_json);
	database->chat_private.length_message = length_eltype;
	sprintf(len_str,"%ld", length_eltype);
		for(i=0;i<length_eltype;i++) {
			elementType = json_object_array_get_idx(parsed_json, i);
			json_object_object_get_ex(elementType, "from_user", &item.from_user);
			json_object_object_get_ex(elementType, "message", &item.message);
			json_object_object_get_ex(elementType, "created_at", &item.created_at);
			json_object_object_get_ex(elementType, "to_user", &item.to_user);
			if (profile.ID_user == json_object_get_int(item.from_user)){
					msg_db = getOneMessagePrivate(item, profile, profile_recv);
					strcpy(msg_db.len, len_str);
			} else{
				msg_db = getOneMessagePrivate(item, profile_recv ,profile);
				strcpy(msg_db.len, len_str);
			}
			
			database->chat_private.msg_private[i] = msg_db;
	}
	free(element);
	return database;
}

char *convert_struct_to_json_message(int from_user, char* message, int room){
    char *data = (char*)malloc(200*sizeof(char));
    sprintf(data,"{\"from_user\": %d, \"message\": \"%s\", \"room\": %d }", from_user,message,room);
    return data;
}

void postMessageGroup(int from_user,char* message, int room){
	char* data = convert_struct_to_json_message(from_user,message,room);
	char* url = (char*)malloc(100*sizeof(char));
	sprintf(url, "%s/api/message/",host);
	requestData(url,data,"POST");
}


void add_friend(int user, int friend){
	char* data = (char*)malloc(100*sizeof(char));
	sprintf(data,"{\"user\": %d, \"friend\": %d, \"confirm\": false}",user, friend);
	char* url = (char*)malloc(100*sizeof(char));
	sprintf(url, "%s/api/friends/",host);
	requestData(url,data, "POST");
}

void postMessage(int from_user, int to_user, char* message){
	char* data = (char*)malloc(200*sizeof(char));
	sprintf(data,"{\"from_user\":%d, \"message\": \"%s\", \"to_user\":%d}",from_user,message,to_user);
	char* url = (char*)malloc(100*sizeof(char));
	sprintf(url, "%s/api/private/",host);
	requestData(url,data,"POST");
}


int get_id_room_private(char* username1, char* username2){
	user_db id_user1 = getUser(username1, -1);
	user_db id_user2 = getUser(username2, -1);
	struct json_object *parsed_json;
	struct json_object *elementType;
	size_t length_eltype,i;
	Room item;
	char * url = (char*)malloc(100*sizeof(char));
	char * buffer = (char*)malloc(100*sizeof(char));
	sprintf(url, "%s/api/room/?room_status=1&member=%d,%d",host, id_user1.ID_user,id_user2.ID_user);
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


Data_base *getGroup(char* element){
	Data_base *database = (Data_base*)malloc(sizeof(Data_base));
	Data_base *db_temp = (Data_base*)malloc(sizeof(Data_base));
	Room item;
	size_t length_eltype, length_member;
	size_t i,j,k;	
	struct json_object *parsed_json;
	struct json_object *elementType;
	struct json_object *id_member;

	parsed_json = json_tokener_parse(element);

	length_eltype = json_object_array_length(parsed_json);
	database->list_group.length_group = length_eltype;

	for(i=0;i<length_eltype;i++) {
		
		

		elementType = json_object_array_get_idx(parsed_json, i);
		json_object_object_get_ex(elementType, "id", &item.id);
		json_object_object_get_ex(elementType, "name", &item.name);
		json_object_object_get_ex(elementType, "admin_room", &item.admin_room);
		json_object_object_get_ex(elementType, "member", &item.member);
		length_member = json_object_array_length(item.member);
		database->list_group.group[i].ID_group = json_object_get_int(item.id);
		strcpy(database->list_group.group[i].name, json_object_get_string(item.name)); 
			// get message
		db_temp = getMessageGroup(json_object_get_int(item.id));
		database->list_group.group[i].length_msg_public = db_temp->list_group.group[0].length_msg_public;
		for (k=0;k<database->list_group.group[i].length_msg_public;k++){
			database->list_group.group[i].msg_public[k] = db_temp->list_group.group[0].msg_public[k];
		}
		
		// get member

		for(j=0;j<length_member;j++) {
			id_member = json_object_array_get_idx(item.member, j);
			database->list_group.group[i].members[j] = getMember(json_object_get_int(id_member));
			database->list_group.group[i].length_member = j+1;
		}	
	}	

	free(db_temp);
	return database;
}

user_db getUserDB(User user){
	user_db Eltype;
	Eltype.ID_user = json_object_get_int(user.id);
	strcpy(Eltype.name, json_object_get_string(user.name));
	strcpy(Eltype.username, json_object_get_string(user.username));
	strcpy(Eltype.password, json_object_get_string(user.password));
	Eltype.is_admin = json_object_get_boolean(user.is_admin);
	Eltype.login_status = json_object_get_boolean(user.status);
	strcpy(Eltype.created_at, json_object_get_string(user.created_at));
	return Eltype;

}

Data_base_user *getUserAdmin(){
	Data_base_user *database = (Data_base_user*)malloc(sizeof(Data_base_user));
	User item;
	size_t length_eltype;
	size_t i;	
	struct json_object *parsed_json;
	struct json_object *elementType;
	char* url = (char*)malloc(100*sizeof(char));
	sprintf(url, "%s/api/user/",host);
	// get data
	char* element = (char*)malloc(500*sizeof(char));
	element = handle_url(url);
	
	parsed_json = json_tokener_parse(element);

	length_eltype = json_object_array_length(parsed_json);
	database->Users.length_user = length_eltype;
		for(i=0;i<length_eltype;i++) {
			elementType = json_object_array_get_idx(parsed_json, i);
			json_object_object_get_ex(elementType, "id", &item.id);
			json_object_object_get_ex(elementType, "name", &item.name);
			json_object_object_get_ex(elementType, "username", &item.username);
			json_object_object_get_ex(elementType, "password", &item.password);
			json_object_object_get_ex(elementType, "status", &item.status);
			json_object_object_get_ex(elementType, "created_at", &item.created_at);
			json_object_object_get_ex(elementType, "is_admin", &item.is_admin);

			database->Users.user[i] = getUserDB(item);
	}
	free(element);
	return database;
}


int check_user(char* username){
	size_t length_eltype;
	struct json_object *parsed_json;
	char *buffer;
	char *url = (char*)malloc(100*sizeof(char)); 
	sprintf(url,"%s/api/user/?username=%s",host, username);
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


int updateMember(int room, int member){

	Room item;
	char* url = (char*)malloc(100*sizeof(char));
	char* buffer = (char*)malloc(100*sizeof(char));
	char *data = (char*)malloc(100*sizeof(char));

	int j, length_member;
	sprintf(url, "%s/api/room/%d/",host, room);
	buffer = handle_url(url);
	struct json_object *parsed_json;
	struct json_object *elementType, *id_member;
	if (buffer){

			parsed_json = json_tokener_parse(buffer);
			elementType = json_object_get(parsed_json);
	

			json_object_object_get_ex(elementType, "id", &item.id);
			json_object_object_get_ex(elementType, "admin_room", &item.admin_room);
			json_object_object_get_ex(elementType, "name", &item.name);
			json_object_object_get_ex(elementType, "member", &item.member);

			length_member = json_object_array_length(item.member);
			if (length_member >= 10) return 2; // nhom day
			for(j=0;j<length_member;j++) {
				id_member = json_object_array_get_idx(item.member, j);
				printf("id member : %d", json_object_get_int(id_member));

						if (json_object_get_int(id_member) == member) return 3; // id ton tai
			}	
		
		sprintf(data, "%s/api/room/%d/?add_member=%d&room=%d",host, room, member, room);
		requestData(data, NULL, "PATCH");
		return 1;
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
		sprintf(url,"%s/api/user/?username=%s",host,username);
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
		sprintf(url,"%s/api/user/%d/",host,id);
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
  
	char *data = (char*)malloc(200*sizeof(char)); 
	char *url = (char*)malloc(100*sizeof(char));
	sprintf(url,"%s/api/user/",host);
	sprintf(data,"{\"name\":\"%s\",\"username\":\"%s\",\"password\":\"%s\"}",user.name, user.username, user.password);
	requestData(url, data, "POST");
 
}


void changePassword(int id, char* newpassword){
	
	char *url= (char*)malloc(100*sizeof(char));
	char *data = (char*)malloc(100*sizeof(char));
	sprintf(url,"%s/api/user/%d/",host, id);
	sprintf(data,"{\"password\": \"%s\"}", newpassword);
	requestData(url,data, "PATCH");
	
}

void updateUser(int id, char* newpassword, char* name){
	char *url= (char*)malloc(100*sizeof(char));
	char *data = (char*)malloc(100*sizeof(char));
	sprintf(url,"%s/api/user/%d/",host, id);

	if (newpassword == NULL && name == NULL){

		sprintf(data,"{\"status\": \"%s\"}", "false");
	} else if (name==NULL){
		sprintf(data,"{\"status\": \"%s\",\"password\": \"%s\"}", "false",newpassword);
	}else
	{
		sprintf(data,"{\"status\": \"%s\",\"password\": \"%s\",\"name\": \"%s\"}", "false",newpassword, name);
	}
	
	
	requestData(url,data, "PATCH");
}

void loginStatus(char* username,int status){
	user_db user = getUser(username,-1);
	char *url= (char*)malloc(100*sizeof(char));
	sprintf(url,"%s/api/user/%d/",host, user.ID_user);
	char *data = (char*)malloc(100*sizeof(char));
	if (status == 1){
		sprintf(data,"{\"status\": true}");
	}else
	{
		sprintf(data,"{\"status\": false}");
	}
	
	requestData(url,data,"PATCH");

	
}




