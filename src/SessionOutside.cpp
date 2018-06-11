#include "SessionOutside.h"
#include "SessionInside.h"
#include "Application.h"
#include "Config.h"
#include "Server.h"



CSessionOutside::CSessionOutside(boost::asio::io_service &ios, CServer &server)
	:m_socket(ios),
	m_server(server),
	m_bConnected(false),
	m_recv_buf(1024),
	m_pSessionInside(NULL)
{
}


CSessionOutside::~CSessionOutside()
{
}

void CSessionOutside::connect()
{
	T_Addr &tAddr = theApp.m_config.getQuoteGetwayAddr();

	try
	{
		boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(tAddr.szIP), tAddr.nPort);

		boost::system::error_code ec;
		m_socket.connect(ep, ec);
		if (!ec)
		{
			std::string strTest = ec.message();

			std::string strIP = m_socket.remote_endpoint().address().to_string();
			strncpy(m_addrRemote.szIP, strIP.c_str(), MAX_IP);
			m_addrRemote.nPort = m_socket.remote_endpoint().port();
			m_bConnected = true;

			BOOST_LOG_SEV(theApp.m_log.get_logger_mt(), debug) << "(req) connect quote gateway success.";

			boost::asio::async_read(m_socket,
				boost::asio::buffer(m_recv_buf.data(), message::header_length),
				boost::bind(&CSessionOutside::handle_read_header, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
		}
		else
		{
			BOOST_LOG_SEV(theApp.m_log.get_logger_mt(), warning) << "(req) connect quote gateway failed, cause:" << ec.message() << "[" << __FILE__ << ":" << __LINE__ << "]";
		}
	}
	catch (std::exception &e)
	{
		BOOST_LOG_SEV(theApp.m_log.get_logger_mt(), warning) << "(req) connect quote gateway failed." << e.what() << "[" << __FILE__ << ":" << __LINE__ << "]";
	}
}

void CSessionOutside::handle_read_header(const boost::system::error_code & ec, size_t read_bytes)
{
	if (!ec)
	{
		m_recv_buf.fill_end(read_bytes);
		int nSize = atoi(m_recv_buf.data());

		boost::asio::async_read(m_socket,
			boost::asio::buffer(m_recv_buf.body(), nSize),
			boost::bind(&CSessionOutside::handle_read_body, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	}
	else
	{
		BOOST_LOG_SEV(theApp.m_log.get_logger_mt(), debug) << "(res) recv response packet failed, cause:" << ec.message() << "[" << __FILE__ << ":" << __LINE__ << "]";
	}
}

void CSessionOutside::handle_read_body(const boost::system::error_code & ec, size_t read_bytes)
{
	if (!ec)
	{
		m_recv_buf.set_body_length(read_bytes);
		m_recv_buf.fill_end(read_bytes);

		m_pSessionInside->send(m_recv_buf);

		boost::asio::async_read(m_socket,
			boost::asio::buffer(m_recv_buf.data(), message::header_length),
			boost::bind(&CSessionOutside::handle_read_header, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	}
	else
	{
		BOOST_LOG_SEV(theApp.m_log.get_logger_mt(), debug) << "(res) send response packet failed, cause:" << ec.message() << "[" << __FILE__ << ":" << __LINE__ << "]";
	}
}

void CSessionOutside::handle_write(const boost::system::error_code & ec, size_t write_bytes)
{
	m_dequeSend.pop_front();

	if (!ec)
	{
		BOOST_LOG_SEV(theApp.m_log.get_logger_mt(), debug) << "(req) send request packet success[" << __FILE__ << ":" << __LINE__ << "]";
	}
	else
	{
		BOOST_LOG_SEV(theApp.m_log.get_logger_mt(), debug) << "(req) send request packet failed, cause:" << ec.message() << "[" << __FILE__ << ":" << __LINE__ << "]";
	}
}

void CSessionOutside::send(message & msg)
{
	try
	{
		if (!m_bConnected)
		{
			// 在这里应该通知行情客户端，消息发送失败
			BOOST_LOG_SEV(theApp.m_log.get_logger_mt(), debug) << "(req) the connection with the quote gateway has been disconnected[" << __FILE__ << ":" << __LINE__ << "]";
			return;
		}

		bool write_in_progress = !m_dequeSend.empty();
		m_dequeSend.push_back(msg);
		if (!write_in_progress)
		{
			message & msgTemp = m_dequeSend.front();

			boost::asio::async_write(m_socket,
				boost::asio::buffer(msgTemp.data(), msgTemp.length()),
				boost::bind(&CSessionOutside::handle_write, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));

			BOOST_LOG_SEV(theApp.m_log.get_logger_mt(), debug) << "(req) send data:" << msgTemp.data() << "[" << __FILE__ << ":" << __LINE__ << "]";
		}
	}
	catch (std::exception &e)
	{
		BOOST_LOG_SEV(theApp.m_log.get_logger_mt(), debug) << "(req) Send request message failure，cause:" << e.what() << "[" << __FILE__ << ":" << __LINE__ << "]";
	}
}

void CSessionOutside::destrory()
{
	m_socket.close();
	delete this;
}

void CSessionOutside::set_session_inside(CSessionInside * pSessionInside)
{
	m_pSessionInside = pSessionInside;
}
