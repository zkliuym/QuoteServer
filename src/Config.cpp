#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/ini_parser.hpp"
#include "Config.h"
#include "Application.h"



CConfig::CConfig()
{
}


CConfig::~CConfig()
{
}

bool CConfig::read_config()
{
	bool bRet;
	try
	{
		boost::property_tree::ptree pt;
		boost::property_tree::ini_parser::read_ini("QuoteServer.ini", pt);

		std::string ip = pt.get<std::string>("server.ip");
		int         port = pt.get<int>("server.port");
		strncpy(m_tQuoteServerAddr.szIP, ip.c_str(), MAX_IP);
		m_tQuoteServerAddr.nPort = port;

		ip = pt.get<std::string>("QuoteGetway.ip");
		port = pt.get<int>("QuoteGetway.port");
		strncpy(m_tQuoteGetway.szIP, ip.c_str(), MAX_IP);
		m_tQuoteGetway.nPort = port;

		bRet = true;
	}
	catch (std::exception &e)
	{
		bRet = false;
		
		theApp.set_last_error(e.what());
	}

	return bRet;

}

T_Addr & CConfig::getQuoteServerAddr()
{
	return m_tQuoteServerAddr;
}

T_Addr & CConfig::getQuoteGetwayAddr()
{
	return m_tQuoteGetway;
}
