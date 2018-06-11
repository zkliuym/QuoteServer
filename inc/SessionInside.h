#ifndef SESSION_INSIDE_H_
#define SESSION_INSIDE_H_
#include <deque>
#include "boost/asio.hpp"
#include "message.h"
#include "DataType.h"

class CServer;
class CSessionOutside;

class CSessionInside
{
public:
	CSessionInside(boost::asio::io_service &ios, CServer &server);
	~CSessionInside();
	boost::asio::ip::tcp::socket& get_socket();
	void start();
	void handle_read_header(const boost::system::error_code &ec, size_t read_bytes);
	void handle_read_body(const boost::system::error_code &ec, size_t read_bytes);
	void handle_write(const boost::system::error_code &ec, size_t write_bytes);
	T_Addr & get_addr_remote();
	CSessionOutside* get_session_outside();
	void send(message & msg);

private:
	boost::asio::ip::tcp::socket m_socket;
	boost::asio::io_service			&m_io_service;
	CServer &m_server;// 当前会话实例所属的服务器
	message m_recv_buf;
	T_Addr m_addrRemote;// 远程客户端地址
	CSessionOutside *m_pSessionOutside;
	std::deque<message>				m_dequeSend;

};

#endif

