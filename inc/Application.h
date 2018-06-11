#ifndef APPLICATION_H_
#define APPLICATION_H_
#include <deque>
#include "boost/thread.hpp"
#include "Config.h"
#include "Log.h"

class CServer;

class CApplication
{
public:
	CApplication();
	~CApplication();

	void push_server(CServer* pServer);
	void pop_server(CServer* pServer);

	void set_last_error(std::string strErrorMsg);
	std::string get_last_error();



private:
	boost::mutex m_mutex;// ���ƶ�m_deq_server�ķ���
	std::deque<CServer*> m_deq_server;// �������з�����ʵ��

public:
	std::string m_strLastError;
	CConfig m_config;
	CLog m_log;


};

extern CApplication theApp;

#endif

