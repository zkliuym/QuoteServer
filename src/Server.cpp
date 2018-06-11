#include "Server.h"
#include "SessionInside.h"
#include "SessionOutside.h"
#include "boost/bind.hpp"
#include "Application.h"




CServer::CServer(boost::asio::io_service &ios, const boost::asio::ip::tcp::endpoint &ep)
	:m_io_service(ios),
	m_acceptor(ios, ep)
{
	start_accept();
}


CServer::~CServer()
{
}

void CServer::start_accept()
{
	CSessionInside *pSessionInside(new CSessionInside(m_io_service, *this));

	m_acceptor.async_accept(pSessionInside->get_socket(),
		boost::bind(&CServer::handle_accept, this, boost::asio::placeholders::error, pSessionInside));

}

void CServer::handle_accept(const boost::system::error_code & error, CSessionInside *pSessionInside)
{
	if (!error)
	{
		pSessionInside->start();
	}
	else
	{

	}

	start_accept();
}

void CServer::join(CSessionInside * pSessionInside)
{
	boost::mutex::scoped_lock lock(m_mutex);
	if (pSessionInside)
	{
		m_setSession.insert(pSessionInside);

		T_Addr &tAddr = pSessionInside->get_addr_remote();

		BOOST_LOG_SEV(theApp.m_log.get_logger_mt(), warning) << "[" << tAddr.szIP << ":" << tAddr.nPort << "] join, inside online client number:" << m_setSession.size() << "[" << __FILE__ << ":" << __LINE__ << "]";
	}
}

void CServer::leave(CSessionInside * pSessionInside)
{
	boost::mutex::scoped_lock lock(m_mutex);
	if (pSessionInside)
	{
		CSessionOutside *pSessionOutside = pSessionInside->get_session_outside();
		if (pSessionOutside)
		{
			pSessionOutside->destrory();
		}
		m_setSession.erase(pSessionInside);

		T_Addr &tAddr = pSessionInside->get_addr_remote();

		BOOST_LOG_SEV(theApp.m_log.get_logger_mt(), warning) << "[" << tAddr.szIP << ":" << tAddr.nPort << "] leave, inside online client number:" << m_setSession.size() << "[" << __FILE__ << ":" << __LINE__ << "]";

		delete pSessionInside;
		pSessionInside = NULL;
	}
}
