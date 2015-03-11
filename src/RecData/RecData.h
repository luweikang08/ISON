#ifndef  RECDATA_H_
#define  RECDATA_H_

#include <iostream>
#include "isonbase.h"
#include "iblog.h"
#include "context.h"
#include "stage.h"
#include "actor.h"
#include "event.h"
#include "ibprotocol.h"

#include <fstream>
#include <time.h>
#include "windows.h"
#include "string.h"

#include "MessageHeader.hpp"
#include "SDS_Transaction.hpp"
#include "SDS_Level2.hpp"
#include "SDS_Signal.hpp"
#include "SDS_Kline.hpp"
#include "SDS_Indicator.hpp"
#include "data_struct.h"
#include "isonsdsdataapi.h"
#include "readconfig.h"
#include "stringhelper.h"


#define SIGNALID 1

#define volum_limit_level1 10000
#define turnover_limit_level1 1000000

#define  ALERT_VERSION      "V0.00.02"
#define  ALERT_RELEASEDATE  "2015/01/10"
#define  ALERT_LOGLEVEL     "1"
#define  ALERT_SUBTOPIC     "1014"
#define  ALERT_SUBENDPOINT  "tcp://192.168.15.200:2010"
#define  ALERT_PUBTOPIC     "2011"
#define  ALERT_PUBENDPOINT  "tcp://192.166.1.41:2010"

#define CONFIG_KEY_VECTOR_DEFAULT \
{ "version", "releasedate", "loglevel", "subtopic", "subendpoint", "pubtopic", "pubendpoint", \
"volum_limit", "turnover_limit", "extra_sell_volum_limit", "extra_sell_turnover_limit", "extra_buy_volum_limit", "extra_buy_turnover_limit" }
#define JSON_FILE_DEFAULT "config.json"

#ifdef NOTOPICHEAD
#define TOPICHEADSIZE 0
#else 
#define TOPICHEADSIZE sizeof(TOPICHEAD)
#endif

int parse_sbe_signal(std::string src)
{
	char recvBuf[256];
	memcpy(recvBuf, src.c_str(), src.size());
	TOPICHEAD* m_topichead_rec;
	m_topichead_rec = (TOPICHEAD*)recvBuf;

	//std::cout <<std::endl<< "topic:" << m_topichead_rec->topic<<std::endl;

	baseline::SDS_Signal CC;
	baseline::MessageHeader hdr;
	int messageHeaderVersion = 0;
	hdr.wrap(recvBuf + sizeof(TOPICHEAD), 0, messageHeaderVersion, 256);//解析头
	CC.wrapForDecode(recvBuf + sizeof(TOPICHEAD), hdr.size(), hdr.blockLength(), hdr.version(), 256);

	char buf[50];
	std::cout << "SignalID:" << (int)CC.signalID() << " ";// std::endl;
	strcpy_s(buf, CC.code());
	std::cout << "code:" << buf << " ";// std::endl;
	std::cout << "date:" << CC.date() << " ";// std::endl;
	std::cout << "time:" << CC.time() << " ";// std::endl;
	strcpy_s(buf, CC.info());
	std::cout << "info:" << buf << std::endl;

	return 0;
}


int parse_sbe_kline(std::string src)
{
	char recvBuf[256];
	memcpy(recvBuf, src.c_str(), src.size());
	TOPICHEAD* m_topichead_rec;
	m_topichead_rec = (TOPICHEAD*)recvBuf;

	//std::cout << std::endl << "topic:" << m_topichead_rec->topic << std::endl;

	baseline::SDS_Kline KK;
	baseline::MessageHeader hdr;
	int messageHeaderVersion = 0;
	hdr.wrap(recvBuf + sizeof(TOPICHEAD), 0, messageHeaderVersion, 256);//解析头
	KK.wrapForDecode(recvBuf + sizeof(TOPICHEAD), hdr.size(), hdr.blockLength(), hdr.version(), 256);

	char buf[16];
	strcpy_s(buf, KK.code());
	string code(buf, 16);
	if (strcmp(code.c_str(), "600446.SH") == 0)
	{
		std::cout << "code:" << buf << " date:" << KK.date() << " time:" << KK.time() << " TS:" << (int)KK.timeStatus() << " preclose:" << KK.preClose()\
			<< " open:" << KK.open() << " close:" << KK.close() << " high:" << KK.high() << " low:" << KK.low() << " vol:" << KK.volume() << " turn:" << KK.turnover() << endl;
	}
	LOG(INFO) << "topic:" << m_topichead_rec->topic << " ms:" << m_topichead_rec->ms << " kw:" << m_topichead_rec->kw << " sn:" << m_topichead_rec->sn << " tm:" << m_topichead_rec->tm\
		<< " code:" << buf << " date:" << KK.date() << " time:" << KK.time() << " TS:" << (int)KK.timeStatus() << " preclose:" << KK.preClose()\
		<< " open:" << KK.open() << " close:" << KK.close() << " high:" << KK.high() << " low:" << KK.low() << " vol:" << KK.volume() << " turn:" << KK.turnover();
	return 0;
}

int parse_sbe_kline_no(std::string src)
{
	char recvBuf[256];
	memcpy(recvBuf, src.c_str(), src.size());
	TOPICHEAD* m_topichead_rec;
	m_topichead_rec = (TOPICHEAD*)recvBuf;

	//std::cout << std::endl << "topic:" << m_topichead_rec->topic << std::endl;

	baseline::SDS_Kline KK;
	baseline::MessageHeader hdr;
	int messageHeaderVersion = 0;
	hdr.wrap(recvBuf + TOPICHEADSIZE , 0, messageHeaderVersion, 256);//解析头
	KK.wrapForDecode(recvBuf + TOPICHEADSIZE, hdr.size(), hdr.blockLength(), hdr.version(), 256);

	char buf[16];
	strcpy_s(buf, KK.code());
	string code(buf, 16);
	if (strcmp(code.c_str(), "600446.SH") == 0)
	{
		std::cout << "code:" << buf << " date:" << KK.date() << " time:" << KK.time() << " TS:" << (int)KK.timeStatus() << " preclose:" << KK.preClose()\
			<< " open:" << KK.open() << " close:" << KK.close() << " high:" << KK.high() << " low:" << KK.low() << " vol:" << KK.volume() << " turn:" << KK.turnover() << endl;
	}
	LOG(INFO) /*<< "topic:" << m_topichead_rec->topic << " ms:" << m_topichead_rec->ms << " kw:" << m_topichead_rec->kw << " sn:" << m_topichead_rec->sn << " tm:" << m_topichead_rec->tm*/\
		<< " code:" << buf << " date:" << KK.date() << " time:" << KK.time() << " TS:" << (int)KK.timeStatus() << " preclose:" << KK.preClose()\
		<< " open:" << KK.open() << " close:" << KK.close() << " high:" << KK.high() << " low:" << KK.low() << " vol:" << KK.volume() << " turn:" << KK.turnover();
	return 0;
}

int parse_sbe_trans(std::string src)
{
	char recvBuf[256];
	memcpy(recvBuf, src.c_str(), src.size());

	//std::cout << std::endl << "topic:" << m_topichead_rec->topic << std::endl;

	baseline::SDS_Transaction TT;
	baseline::MessageHeader hdr;
	int messageHeaderVersion = 0;
	hdr.wrap(recvBuf + TOPICHEADSIZE, 0, messageHeaderVersion, 256);//解析头
	TT.wrapForDecode(recvBuf + TOPICHEADSIZE, hdr.size(), hdr.blockLength(), hdr.version(), 256);

	char buf[16];
	strcpy_s(buf, TT.code());
	string code(buf, 16);
	if (strcmp(code.c_str(), "600446.SH") == 0)
	{
		std::cout << "code:" << buf << " seqno:" << TT.seqno() << " date:" << TT.date() << " time:" << TT.time() << " SDSTime:" << TT.sDSTime() << " sn:" << TT.sn()\
			<< " index:" << TT.index() << " price:" << TT.price() << " volume:" << TT.volume() << " turnover:" << TT.turnover() << " bSFlag:" << TT.bSFlag() << " orderKind:" << TT.orderKind() \
			<< " askOrder:" << TT.askOrder() << " bidOrder:" << TT.bidOrder() << endl;
	}
	LOG(INFO) /*<< "topic:" << m_topichead_rec->topic << " ms:" << m_topichead_rec->ms << " kw:" << m_topichead_rec->kw << " sn:" << m_topichead_rec->sn << " tm:" << m_topichead_rec->tm*/\
		<< "code:" << buf << " seqno:" << TT.seqno() << " date:" << TT.date() << " time:" << TT.time() << " SDSTime:" << TT.sDSTime() << " sn:" << TT.sn()\
		<< " index:" << TT.index() << " price:" << TT.price() << " volume:" << TT.volume() << " turnover:" << TT.turnover() << " bSFlag:" << TT.bSFlag() << " orderKind:" << TT.orderKind() \
		<< " askOrder:" << TT.askOrder() << " bidOrder:" << TT.bidOrder();
	return TT.sn();
}


int parse_sbe_level2(std::string src)
{
	char recvBuf[1024];
	memcpy(recvBuf, src.c_str(), src.size());
	TOPICHEAD* m_topichead_rec;
	m_topichead_rec = (TOPICHEAD*)recvBuf;

	//std::cout << std::endl << "topic:" << m_topichead_rec->topic << std::endl;

	baseline::SDS_Level2 LL;
	baseline::MessageHeader hdr;
	int messageHeaderVersion = 0;
	hdr.wrap(recvBuf + sizeof(TOPICHEAD), 0, messageHeaderVersion, 1024);//解析头
	LL.wrapForDecode(recvBuf + sizeof(TOPICHEAD), hdr.size(), hdr.blockLength(), hdr.version(), 1024);

	return LL.sn();
	//cout << LL.sn() << endl;

	/*char buf[50];
	strcpy_s(buf, LL.code());
	string code(buf, 10);
	if (strcmp(code.c_str(), "600446.SH") == 0)
	{
		std::cout << "code:" << buf << " date:" << KK.date() << " time:" << KK.time() << " TS:" << KK.timeStatus() << " preclose:" << KK.preClose()\
			<< " open:" << KK.open() << " close:" << KK.close() << " high:" << KK.high() << " vol:" << KK.volume() << " turn:" << KK.turnover() << endl;
	}
	LOG(INFO) << "code:" << buf << " date:" << KK.date() << " time:" << KK.time() << " TS:" << KK.timeStatus() << " preclose:" << KK.preClose()\
		<< " open:" << KK.open() << " close:" << KK.close() << " high:" << KK.high() << " vol:" << KK.volume() << " turn:" << KK.turnover();*/
	//return 0;
}

int parse_sbe_indicator(std::string src)
{
	char recvBuf[256];
	memcpy(recvBuf, src.c_str(), src.size());
	TOPICHEAD* m_topichead_rec;
	m_topichead_rec = (TOPICHEAD*)recvBuf;

	//std::cout <<std::endl<< "topic:" << m_topichead_rec->topic<<std::endl;

	baseline::SDS_Indicator II;
	baseline::MessageHeader hdr;
	int messageHeaderVersion = 0;
	hdr.wrap(recvBuf + sizeof(TOPICHEAD), 0, messageHeaderVersion, 256);//解析头
	II.wrapForDecode(recvBuf + sizeof(TOPICHEAD), hdr.size(), hdr.blockLength(), hdr.version(), 256);

	char buf[50];
	std::cout << "SignalID:" << (int)II.signalID() << std::endl;
	strcpy_s(buf, II.code());
	std::cout << "code:" << buf << std::endl;
	std::cout << "date:" << II.date() << std::endl;
	std::cout << "time:" << II.time() << std::endl;
	std::cout << "data:" << II.dataArr(0) << "," << II.dataArr(1) << "," << II.dataArr(2) << "," << II.dataArr(3) << "," << II.dataArr(4) << std::endl;

	LOG(INFO) << "SignalID:" << (int)II.signalID() << "code:" << buf << "date:" << II.date() << "time:" << II.time() \
		<< "data:" << II.dataArr(0) << "," << II.dataArr(1) << "," << II.dataArr(2) << "," << II.dataArr(3) << "," << II.dataArr(4);

	return 0;
}

#endif 