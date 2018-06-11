#include <iostream>
#include "Server.h"
#include "Application.h"

int main(int argc, char *argv[])
{
	// ∂¡»°≈‰÷√
	if (!theApp.m_config.read_config())
	{
		BOOST_LOG_SEV(theApp.m_log.get_logger_mt(), warning) << "QuoteServer.ini file read failed, cause:" << theApp.get_last_error() << "[" << __FILE__ << ":" << __LINE__ << "]";
		return -1;
	}
	
	try
	{
		T_Addr &tAddr = theApp.m_config.getQuoteServerAddr();

		boost::asio::io_service ios;
		boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(tAddr.szIP), tAddr.nPort);

		CServer *pServer(new CServer(ios, ep));
		theApp.push_server(pServer);

		BOOST_LOG_SEV(theApp.m_log.get_logger_mt(), warning) << "Quote server start success, listen to addr:[" << tAddr.szIP << ":" <<tAddr.nPort << "]" << "[" << __FILE__ << ":" << __LINE__ << "]";

		ios.run();
	}
	catch (std::exception &e)
	{
		BOOST_LOG_SEV(theApp.m_log.get_logger_mt(), warning) << "Quote server start failed, cause:" << e.what() << "[" << __FILE__ << ":" << __LINE__ << "]";
	}

	return 0;
}