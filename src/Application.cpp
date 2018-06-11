#include "Application.h"

CApplication theApp;

CApplication::CApplication()
{
	m_log.init("QuoteServer");
}


CApplication::~CApplication()
{
}

void CApplication::push_server(CServer * pServer)
{
	boost::mutex::scoped_lock lock(m_mutex);
	if (pServer)
	{
		m_deq_server.push_back(pServer);
	}
}

void CApplication::pop_server(CServer * pServer)
{
	boost::mutex::scoped_lock lock(m_mutex);
	if (pServer)
	{
		std::deque<CServer*>::iterator it = find(m_deq_server.begin(), m_deq_server.end(), pServer);
		if (it != m_deq_server.end())
		{
			m_deq_server.erase(it);
		}
	}
}

void CApplication::set_last_error(std::string strErrorMsg)
{
	m_strLastError = strErrorMsg;
}

std::string CApplication::get_last_error()
{
	return m_strLastError;
}
