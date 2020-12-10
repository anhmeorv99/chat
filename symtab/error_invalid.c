#include "error_invalid.h"
#include <string.h>
#define NUM_ERROR 9
#define NUM_INVALID 4

struct Error_Message_ {
	Error error;
	char *message;
};

typedef struct Error_Message_ Error_Message;

struct Invalid_Message_ {
	Invalid invalid;
	char *message;
};

typedef struct Invalid_Message_ Invalid_Message;

Error_Message error_message[NUM_ERROR] = {
	{ERR_NONE , "none"},
	{ERR_PASSWORD , "Password khong hop le!"},
	{ERR_MIN_PASSWORD , "Password phai 6 ky tu tro len!"},
	{ERR_CAN_NOT_PASSWORD , "Password khong dung!"},
	{ERR_ACCOUNT , "Account khong hop le!"},
	{ERR_NOT_ACCOUNT , "Account khong ton tai!"},
	{ERR_HAS_ACCOUNT , "Account da ton tai!"},
	{ERR_CAN_NOT_ACCOUNT , "Account khong dung!"},
	{ERR_ACCOUNT_PASSWORD , "Account hoac password khong dung!"}
};

Invalid_Message invalid_message[NUM_INVALID] = {
	{OK_NONE, "none"},
	{OK_LOGIN , "Dang nhap thanh cong"},
	{OK_SIGNUP , "Dang ky thanh cong"},
	{OK_CHANGE_PASSWORD , "Thay doi mat khau thanh cong"}
};
//convect Error:  enum  -> message
void error_to_string(Error error, char *message){
	int i;
	for(i = 0; i < NUM_ERROR; i++){
		if(error == error_message[i].error){
			strcpy(message,error_message[i].message);
			break;
		}
	}
}
//convect Error:  message -> enum
void error_to_enum(char *message, Error error){
	int i;
	for(i = 0; i < NUM_ERROR ; i++){
		if(strcmp(message,error_message[i].message) == 0){
			error = error_message[i].error;
			break;
		}
	}
}
// convect Invalid: enum -> message
void invalid_to_string(Invalid invalid, char *message){
	int i;
	for(i = 0; i < NUM_INVALID; i++){
		if(invalid == invalid_message[i].invalid){
			strcpy(message,invalid_message[i].message);
			break;
		}
	}
}
//convect Invalid: message -> enum 
void invalid_to_enum(char *message, Invalid invalid){
	int i;
	for(i = 0; i < NUM_INVALID; i++){
		if(strcmp(message,invalid_message[i].message) == 0){
			invalid = invalid_message[i].invalid;
			break;
		}
	}
}

