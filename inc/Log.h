/*
@brief  ʹ��boost��log��ʵ����־��ӡ

ʹ��ʾ����
CLog log;
log.init("test");

example 1: ��ָ����־�ȼ���ӡ��־�����е���־����0�ȼ���ӡ��
BOOST_LOG(log.get_logger_mt()) << "Hello world.";

example 2: ָ����־�ȼ���ӡ��־
BOOST_LOG_SEV(theApp.m_log.get_logger_mt(), warning) << "����һ�����м�ֵ���飬ֵ��ȥ�Ķ�";

��ʽ����־��ϢΪָ����ʽ��
1��ʱ��+��Ϣ
sink->set_formatter(expr::format("[%1%]: %2%")
% boost::log::expressions::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")
% boost::log::expressions::smessage);

2��ʱ��+��־�ȼ�+��Ϣ
sink->set_formatter(expr::format("[%1%]<%2%>: %3%")
% expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")
% boost::log::expressions::attr<sign_severity_level>("Severity")
% boost::log::expressions::smessage);

3��ʱ��+��־�ȼ�+�߳�id+��Ϣ
sink->set_formatter(expr::format("[%1%]<%2%>(%3%): %4%")
% expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")
% boost::log::expressions::attr<sign_severity_level>("Severity")
% boost::log::expressions::attr<boost::log::attributes::current_thread_id::value_type >("ThreadID")
% boost::log::expressions::smessage);

*/
#ifndef LOG_H_
#define LOG_H_
#include <fstream>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/thread/thread.hpp>
#include <boost/core/null_deleter.hpp>                // empty_deleter
#include <boost/log/core.hpp>
#include <boost/log/common.hpp>                       // BOOST_LOG
#include <boost/log/trivial.hpp>                      // BOOST_LOG_TRIVIAL/BOOST_LOG_SEV
#include <boost/log/sinks.hpp>                        // synchronous_sink/asynchronous_sink
#include <boost/log/sources/logger.hpp>               // logger
#include <boost/log/expressions.hpp>                  // expressions
#include <boost/log/attributes.hpp>					  // attributes
#include <boost/log/utility/setup/console.hpp>        // add_console_log
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/formatter_parser.hpp>



// ��־�ȼ�
enum sign_severity_level
{
	trace,
	debug,
	info,
	warning,
	error,
	fatal,
	report
};

typedef boost::log::sinks::synchronous_sink<boost::log::sinks::text_ostream_backend> text_sink;
typedef boost::log::sources::severity_logger_mt<sign_severity_level> logger_mt;

/*
  * @name operator<<
  *
  * @brief ����־������ļ���ʱ������Ӧ�ĵȼ���ֵת��Ϊ��Ӧ���ַ���
*/ 
template< typename CharT, typename TraitsT >
inline std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& strm, sign_severity_level lvl)
{
	static const char* const str[] =
	{
		"trace",
		"debug",
		"info",
		"warning",
		"error",
		"fatal",
		"report"
	};
	if (static_cast<std::size_t>(lvl) < (sizeof(str) / sizeof(*str)))
		strm << str[lvl];
	else
		strm << static_cast<int>(lvl);
	return strm;
}

class CLog
{
public:
	CLog();
	~CLog();

	void init(const char *szFileName, bool bOuputScreen = false);

	logger_mt & get_logger_mt();


private:
	logger_mt m_lg_mt;

};



#endif
