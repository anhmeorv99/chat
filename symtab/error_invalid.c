#include "error_invalid.h"
#include <string.h>
#define NUM_ERROR 21
#define NUM_INVALID 4
#define INVALID_USERNAME "ABC"
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
	{ERR_NULL_NAME, "Ban chua nhap Name!"},
	{ERR_LENGTH_NAME, "Name phai it hon 30 ky tu!"},
	{ERR_INVAL_NAME, "Name khong hop le!"},
	{ERR_NULL_PASSWORD, "Ban chua nhap Password!"},
	{ERR_PASSWORD , "Password khong hop le!"},
	{ERR_MIN_PASSWORD , "Password phai 6 ky tu tro len!"},
	{ERR_MAX_PASSWORD, "Password phai nhieu nhat 29 ky tu!"},
	{ERR_CAN_NOT_PASSWORD , "Password khong dung!"},
	{ERR_NULL_USERNAME, "Ban chua nhap Username!"},
	{ERR_USERNAME , "Username khong hop le!"},
	{ERR_NOT_USERNAME , "Username khong ton tai!"},
	{ERR_HAS_USERNAME , "Username da ton tai!"},
	{ERR_USERNAME_PASSWORD , "Username hoac password khong dung!"},
	{ERR_NULL_RE_PASSWORD,"Ban chua nhap Confirm Password!"},
	{ERR_NOT_RE_PASSWORD,"Confirm Password khong dung!"},
	{ERR_USERNAME_LOGIN,"Username dang dang nhap!"},
	{ERR_FULL_MEMBER,"Thanh vien day!"},
	{ERR_CAN_NOT_ID_ROOM,"Id room khong ton tai!"},
	{ERR_DANG_XAC_NHAN,"Tai Khoan dang cho xac nhan!"},
	{ERR_LA_BAN_BE,"Tai khoan da la ban be!"}
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
Error error_to_enum(char *message){
	int i;
	Error err = ERR_NONE;
	for(i = 0; i < NUM_ERROR ; i++){
		if(strcmp(message,error_message[i].message) == 0){
			err = error_message[i].error;
			break;
		}
	}
	return err;
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
Invalid invalid_to_enum(char *message){
	int i;
	Invalid invalid = OK_NONE;
	for(i = 0; i < NUM_INVALID; i++){
		if(strcmp(message,invalid_message[i].message) == 0){
			invalid = invalid_message[i].invalid;
			break;
		}
	}
	return invalid;
}
////////check error and invalid----------
//check error or invalid
//string co khoang trang: co -> 0, khong -> 1
int check_space(char *str){
	int i, len;
	len = strlen(str); //gia thiet len >0
	for(i = 0; i < len ; i++){
		if(str[i] == ' '){
			return 0;
		}
	}
	return 1;
}
//kiem tra loi cua name
Error check_signup_name(char *name){
	Error err;
	if(strlen(name) == 0){
		err = ERR_NULL_NAME;
	}
	else if(check_space(name) == 0){
		err = ERR_INVAL_NAME;
	}
	else if(strlen(name) > 29){
		err = ERR_LENGTH_NAME;
	}
	else{
		err = ERR_NONE;
	}
	return err;
}
//kiem tra loi cua username
Error check_signup_username(char *account){
    Error err;
    if(strlen(account) == 0){
    	err = ERR_NULL_USERNAME;
    }
    else if(check_space(account) == 0){
    	err = ERR_USERNAME;
    }
    else if(strlen(account) < 6 || strlen(account) > 30){
    	err = ERR_USERNAME;
    }
    else{
        err = ERR_NONE;
    }
    return err;
}
//kiem tra loi cua password
Error check_signup_password(char *password){
    Error err;
    if(strlen(password) == 0){
    	err = ERR_NULL_PASSWORD;
    }
    else if(strlen(password) < 6){
        err = ERR_MIN_PASSWORD;
    }
    else if(strlen(password) > 29){
    	err = ERR_MAX_PASSWORD;
    }
    else if(check_space(password) == 0){
    	err = ERR_PASSWORD;
    }
    else{
        err = ERR_NONE;
    }
    return err;
}
//kiem tra loi cua confirm password
Error check_signup_re_password(char *re_password, char *password){
	Error err;
	if(strlen(re_password) == 0){
		err = ERR_NULL_RE_PASSWORD;
	}
	else if(strcmp(re_password,password) != 0){
		err = ERR_NOT_RE_PASSWORD;
	}else{
		err = ERR_NONE;
	}
	return err;
}
//kiemtra loi cua cur_password
Error check_current_password(char *cur_pass, char *login_password){
	Error err;
	if(strlen(cur_pass) == 0){
		err = ERR_NULL_RE_PASSWORD;
	}
	else if(strcmp(cur_pass,login_password) != 0){
		err = ERR_CAN_NOT_PASSWORD;
	}
	else{
		err = ERR_NONE;
	}
	return err;
}