#ifndef DATA_TYPE_H_
#define DATA_TYPE_H_
#include <memory.h>
#include "macro.h"

typedef struct _Addr
{
	char szIP[MAX_IP];
	int nPort;
	_Addr()
	{
		memset(this, 0, sizeof(_Addr));
	}
}T_Addr;

typedef struct _XtpQuoteLogin
{
	T_Addr tAddr;
	char szUserName[MAX_USERNAME];
	char szPwd[MAX_PWD];
	int  nClientID;
	int  nUdpPush;
	_XtpQuoteLogin()
	{
		memset(this, 0, sizeof(_XtpQuoteLogin));
	}
}T_XtpQuoteLogin;

#endif