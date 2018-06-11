#ifndef CONFIG_H_
#define CONFIG_H_
#include "DataType.h"

class CConfig
{
public:
	CConfig();
	~CConfig();

	bool read_config();

	// 返回行情服务器地址
	T_Addr & getQuoteServerAddr();

	// 返回行情网关地址
	T_Addr & getQuoteGetwayAddr();

private:
	T_Addr m_tQuoteGetway;		// 行情前置
	T_Addr m_tQuoteServerAddr;	// 行情服务器
};

#endif