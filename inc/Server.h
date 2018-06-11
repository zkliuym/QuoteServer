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

	// 开始监听客户端连接
	void start_accept();

	// 处理监听的结果
	void handle_accept(const boost::system::error_code& error, CSessionInside *pSessionInside);

	void join(CSessionInside *pSessionInside);
	void leave(CSessionInside *pSessionInside);

private:
	boost::asio::io_service &m_io_service;
	boost::asio::ip::tcp::acceptor m_acceptor;
	boost::mutex m_mutex;// 控制对m_setSession的访问
	std::set<CSessionInside*> m_setSession;// 存储所有内部会话实例
};

#endif

