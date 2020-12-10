

enum Error_ {
	ERR_NONE,
	ERR_PASSWORD,			//password khong hop le
	ERR_MIN_PASSWORD,		//password ngan
	ERR_CAN_NOT_PASSWORD,	//password khong dung
	ERR_ACCOUNT,			//account khong hop le
	ERR_NOT_ACCOUNT,		//account khong ton tai
	ERR_HAS_ACCOUNT,		//account da ton tai
	ERR_CAN_NOT_ACCOUNT,	//account khong dung
	ERR_ACCOUNT_PASSWORD	//account hoac password khong dung
};

typedef enum Error_ Error;

enum Invalid_ {
	OK_NONE,
	OK_LOGIN,				//dang nhap thanh cong
	OK_SIGNUP,				//dang ky thanh cong
	OK_CHANGE_PASSWORD		//doi mat khau thanh cong
};

typedef enum Invalid_ Invalid;

//convect Error:  enum  -> message
void error_to_string(Error error, char *message);
//convect Error:  message -> enum
void error_to_enum(char *message, Error error);
// convect Invalid: enum -> message
void invalid_to_string(Invalid invalid, char *message);
//convect Invalid: message -> enum 
void invalid_to_enum(char *message, Invalid invalid);