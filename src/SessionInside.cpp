#include "SessionInside.h"
#include "boost/bind.hpp"
#include "Server.h"
#include "SessionOutside.h"
#include "Application.h"



CSessionInside::CSessionInside(boost::asio::io_service &ios, CServer &server)
	:m_socket(ios),
	m_io_service(ios),
	m_server(server),
	m_recv_buf(1024),
	m_pSessionOutside(NULL)
{
}


CSessionInside::~CSessionInside()
{
}

boost::asio::ip::tcp::socket & CSessionInside::get_socket()
{
	return m_socket;
}

void CSessionInside::start()
{
	std::string strIP = m_socket.remote_endpoint().address().to_string();
	strncpy(m_addrRemote.szIP, strIP.c_str(), MAX_IP);
	m_addrRemote.nPort = m_socket.remote_endpoint().port();

	m_pSessionOutside = new CSessionOutside(m_io_service, m_server);
	m_pSessionOutside->connect();
	m_pSessionOutside->set_session_inside(this);

	m_server.join(this);

	boost::asio::async_read(m_socket,
		boost::asio::buffer(m_recv_buf.data(), message::header_length),
		boost::bind(&CSessionInside::handle_read_header, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void CSessionInside::handle_read_header(const boost::system::error_code &ec, size_t read_bytes)
{
	if (!ec)
	{
		m_recv_buf.fill_end(read_bytes);
		int nSize = atoi(m_recv_buf.data());

		boost::asio::async_read(m_socket,
			boost::asio::buffer(m_recv_buf.body(), nSize),
			boost::bind(&CSessionInside::handle_read_body, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	}
	else
	{
		m_server.leave(this);
	}
	
}

void CSessionInside::handle_read_body(const boost::system::error_code &ec, size_t read_bytes)
{
	if (!ec)
	{
		m_recv_buf.set_body_length(read_bytes);
		m_recv_buf.fill_end(read_bytes);

		BOOST_LOG_SEV(theApp.m_log.get_logger_mt(), debug) << "(req) recv data:" << m_recv_buf.body() << "[" << __FILE__ << ":" << __LINE__ << "]";

		if (m_pSessionOutside)
		{
			m_pSessionOutside->send(m_recv_buf);
		}
		else
		{
			BOOST_LOG_SEV(theApp.m_log.get_logger_mt(), warning) << "(req) request session instances do not exist" << "[" << __FILE__ << ":" << __LINE__ << "]";
		}

		boost::asio::async_read(m_socket,
			boost::asio::buffer(m_recv_buf.data(), message::header_length),
			boost::bind(&CSessionInside::handle_read_header, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	}
	else
	{
		m_server.leave(this);
	}
}

void CSessionInside::handle_write(const boost::system::error_code &ec, size_t write_bytes)
{
	m_dequeSend.pop_front();

	if (!ec)
	{
		BOOST_LOG_SEV(theApp.m_log.get_logger_mt(), warning) << "(res) send response packet success"<< "[" << __FILE__ << ":" << __LINE__ << "]";
	}
	else
	{
		BOOST_LOG_SEV(theApp.m_log.get_logger_mt(), warning) << "(res) send response packet failed, cause:" << ec.message() << "[" << __FILE__ << ":" << __LINE__ << "]";
		m_server.leave(this);
	}
}

T_Addr & CSessionInside::get_addr_remote()
{
	return m_addrRemote;
}

CSessionOutside * CSessionInside::get_session_outside()
{
	return m_pSessionOutside;
}

void CSessionInside::send(message & msg)
{
	try
	{
		bool write_in_progress = !m_dequeSend.empty();
		m_dequeSend.push_back(msg);
		if (!write_in_progress)
		{
			message & msgTemp = m_dequeSend.front();

			BOOST_LOG_SEV(theApp.m_log.get_logger_mt(), debug) << "(res) send response packet is:" << msgTemp.data() << "[" << __FILE__ << ":" << __LINE__ << "]";

			boost::asio::async_write(m_socket,
				boost::asio::buffer(msgTemp.data(), msgTemp.length()),
				boost::bind(&CSessionInside::handle_write, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
		}
	}
	catch (std::exception &e)
	{
		BOOST_LOG_SEV(theApp.m_log.get_logger_mt(), warning) << "(res) send response packet failed, cause:" << e.what() << "[" << __FILE__ << ":" << __LINE__ << "]";
	}
}
