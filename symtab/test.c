
#include <stdio.h>
#include "database.h"
#include <string.h>
int main (){
	//List_DB *root_db = NULL;
	int i = 0;
	Data_base *db = (Data_base*)malloc(sizeof(Data_base));
	while(i < 3){
	printf("Enter username:");gets(db->user.username);
	printf("Enter name:");gets(db->user.name);
	printf("Enter pass:");gets(db->user.password);
	printf("Enter chat:");gets(db->chat_private[0].msg_private[0].message);
	//printf("name: %s\nusername: %s\npassword: %s\nchat: %s\n\n",db->user.name,db->user.username,
	//	db->user.password,db->chat_private[0].msg_private[0].message);
	
	//root_db = insert_at_end_db(root_db,db);
	printf("name: %s\nusername: %s\npassword: %s\nchat: %s\n\n",db->user.name,db->user.username,
			db->user.password,db->chat_private[0].msg_private[0].message);
	i++;
	}
	puts("-------------\n");
	//List_DB *cur = root_db;
	//while(cur != NULL){
		
		//cur = cur->next;
	//}
	//free_list_db(root_db);
	//if(root_db = NULL)
		printf("NULL\n");
	return 0;
}