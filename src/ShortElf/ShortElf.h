#ifndef __ISON_SDS_SHORT_ELF_H__
#define __ISON_SDS_SHORT_ELF_H__

#include <iostream>
#include "isonbase.h"
#include "iblog.h"
#include "context.h"
#include "stage.h"
#include "actor.h"
#include "event.h"
#include "ibprotocol.h"

#include "isonsdsdataapi.h"
#include "readconfig.h"
#include "stringhelper.h"
#include "data_defs.h"

#include "LargeTranMonitor.h"
#include "RapidMoveMonitor.h"
#include "FluctuationMonitor.h"

const vector<std::string> VECTOR_TOPIC = { PreTopicLevel2, PreTopicTrans, "", PreTopicQueue, PreTopicCode, "", "", "", PreTopicKline };

const int LARGE_BUY_SIGNALID = 1;
const int LARGE_SELL_SIGNALID = 2;
const int LARGE_AUCTION_SIGNAID = 3;

const char* SUBID_OF_SUB1 = "sub_subactor1";
const char* SUBID_OF_SUB2 = "sub_subactor2";
const char* PUBID_OF_SUB = "pub_subactor";
const char* SUBID_OF_PROC = "sub_procactor";
const char* PUBID_OF_PROC = "pub_procactor";
const char* SUBID_OF_PUB = "sub_pubactor";
const char* PUBID_OF_PUB = "pub_pubactor";
const char* STR_EP_SUB = "inproc://subdata";  //tansform data from stage_sub to stage_proc
const char* STR_EP_PUB = "inproc://pubindicator";  //tansform data from stage_sub to stage_proc

const char* CLOG_DIR = "SELog";
const char* JSON_FILE_DEFAULT = "config.json";

const vector<string> VECTOR_CONFIGKEY = { "version", "releasedate", "loglevel", "subendpoint_mkt", "subendpoint_kline", "pubendpoint", \
"large_volum_limit", "large_turnover_limit", "extra_sell_volum_limit", "extra_sell_turnover_limit", "extra_buy_volum_limit", "extra_buy_turnover_limit", "rise_limit", "fall_limit" };
const vector<string> VECTOR_CONFIGVALUE_DEFAULT = { "V1.00.00", "2015/02/12", "1", "tcp://192.168.15.200:2003", "tcp://192.168.15.200:2031", "tcp://192.168.15.200:2030", \
"40000",             "1000000",              "60000",                  "1500000",                   "60000",                 "1500000",                  "1.8",        "1.8"        };

inline void makeinfo(const float & src, string& dest)
{
	std::stringstream s;
	s << src;
	dest = s.str();
}
inline void makeinfo(const unsigned int & src, string& dest)
{
	std::stringstream s;
	s << src;
	dest = s.str();
}

#endif