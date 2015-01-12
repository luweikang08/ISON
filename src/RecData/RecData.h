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


int parsesbe(std::string src)
{
	char recvBuf[256];
	memcpy(recvBuf, src.c_str(), src.size());
	baseline::SDS_Signal CC;
	baseline::MessageHeader hdr;
	int messageHeaderVersion = 0;
	hdr.wrap(recvBuf + sizeof(TOPICHEAD), 0, messageHeaderVersion, 256);//Ω‚ŒˆÕ∑
	CC.wrapForDecode(recvBuf + sizeof(TOPICHEAD), hdr.size(), hdr.blockLength(), hdr.version(), 256);

	char buf[50];
	std::cout << "SignalID:" << CC.signalID() << std::endl;
	strcpy_s(buf, CC.code());
	std::cout << "code:" << buf << std::endl;
	std::cout << "date:" << CC.date() << std::endl;
	std::cout << "time:" << CC.time() << std::endl;
	strcpy_s(buf, CC.info());
	std::cout << "info:" << buf << std::endl;

	return 0;
}

#endif 