#ifndef SESSION_OUTSIDE_H_
#define SESSION_OUTSIDE_H_
#include <deque>
#include "boost/asio.hpp"
#include "message.h"
#include "DataType.h"

class CServer;
class CSessionInside;

class CSessionOutside
{
public:
	CSessionOutside(boost::asio::io_service &ios, CServer &server);
	~CSessionOutside();

	// 连接行情前置
	void connect();
	void handle_read_header(const boost::system::error_code &ec, size_t read_bytes);
	void handle_read_body(const boost::system::error_code &ec, size_t read_bytes);
	void handle_write(const boost::system::error_code &ec, size_t write_bytes);

	// 向行情客户端发送应答数据
	void send(message & msg);

	void destrory();

	void set_session_inside(CSessionInside *pSessionInside);

private:
	boost::asio::ip::tcp::socket m_socket;
	CServer &m_server;
	bool m_bConnected;// 是否连接上了交易前置
	std::deque<message>				m_dequeSend;
	T_Addr m_addrRemote;// 远程地址
	message m_recv_buf;
	CSessionInside *m_pSessionInside;
};

#endif

