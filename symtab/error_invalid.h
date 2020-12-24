
enum Error_ {
	ERR_NONE,
	ERR_NULL_NAME,			//Name: chua nhap
	ERR_LENGTH_NAME,		//Name:  length < 30
	ERR_INVAL_NAME,			//Name: khong hop le
	ERR_NULL_PASSWORD,		//Password: chua nhap
	ERR_PASSWORD,			//password khong hop le
	ERR_MIN_PASSWORD,		//password ngan
	ERR_MAX_PASSWORD,		//PASSWORD dai
	ERR_CAN_NOT_PASSWORD,	//password khong dung
	ERR_NULL_USERNAME,		//Username chua nhap
	ERR_USERNAME,			//Username khong hop le
	ERR_NOT_USERNAME,		//Username khong ton tai
	ERR_HAS_USERNAME,		//Username da ton tai
	ERR_USERNAME_PASSWORD,	//Username hoac password khong dung
	ERR_NULL_RE_PASSWORD,	//re-password chua nhap
	ERR_NOT_RE_PASSWORD,		//re-password khong giong
	ERR_USERNAME_LOGIN		//username dang dang nhap
};

typedef enum Error_ Error;

enum Invalid_ {
	OK_NONE,
	OK_LOGIN,				//dang nhap thanh cong
	OK_SIGNUP,				//dang ky thanh cong
	OK_CHANGE_PASSWORD		//doi mat khau thanh cong
};

typedef enum Invalid_ Invalid;
typedef enum {
	true,
	false
}bool;
//convect Error:  enum  -> message
void error_to_string(Error error, char *message);
//convect Error:  message -> enum
Error error_to_enum(char *message);
// convect Invalid: enum -> message
void invalid_to_string(Invalid invalid, char *message);
//convect Invalid: message -> enum 
Invalid invalid_to_enum(char *message);
////////check error and invalid----------
//check error or invalid
//string co khoang trang: co -> 0, khong -> 1
int check_space(char *str);
//kiem tra loi cua name
Error check_signup_name(char *name);
//kiem tra loi cua username
Error check_signup_username(char *account);
//kiem tra loi cua password
Error check_signup_password(char *password);
//kiem tra loi cua confirm password
Error check_signup_re_password(char *re_password, char *password);
//kiemtra loi cua cur_password
Error check_current_password(char *cur_pass, char *login_password);
