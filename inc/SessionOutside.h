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

	// ��������ǰ��
	void connect();
	void handle_read_header(const boost::system::error_code &ec, size_t read_bytes);
	void handle_read_body(const boost::system::error_code &ec, size_t read_bytes);
	void handle_write(const boost::system::error_code &ec, size_t write_bytes);

	// ������ͻ��˷���Ӧ������
	void send(message & msg);

	void destrory();

	void set_session_inside(CSessionInside *pSessionInside);

private:
	boost::asio::ip::tcp::socket m_socket;
	CServer &m_server;
	bool m_bConnected;// �Ƿ��������˽���ǰ��
	std::deque<message>				m_dequeSend;
	T_Addr m_addrRemote;// Զ�̵�ַ
	message m_recv_buf;
	CSessionInside *m_pSessionInside;
};

#endif

