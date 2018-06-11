#ifndef SERVER_H_
#define SERVER_H_
#include <set>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

class CSessionInside;

class CServer
{
public:
	CServer(boost::asio::io_service &ios, const boost::asio::ip::tcp::endpoint &ep);
	~CServer();

	// ��ʼ�����ͻ�������
	void start_accept();

	// ��������Ľ��
	void handle_accept(const boost::system::error_code& error, CSessionInside *pSessionInside);

	void join(CSessionInside *pSessionInside);
	void leave(CSessionInside *pSessionInside);

private:
	boost::asio::io_service &m_io_service;
	boost::asio::ip::tcp::acceptor m_acceptor;
	boost::mutex m_mutex;// ���ƶ�m_setSession�ķ���
	std::set<CSessionInside*> m_setSession;// �洢�����ڲ��Ựʵ��
};

#endif

