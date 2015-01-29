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

const int LARGE_BUY_SIGNALID = 1;
const int LARGE_SELL_SIGNALID = 2;
const int LARGE_AUCTION_SIGNAID = 3;

const char* SUBID = "sub1";
const char* PUBID = "pub1";
const string SUBTOPIC_SZ = itostring(ISON_SDS2TGW::TSZ_TRANSACTION);
const string SUBTOPIC_SH = itostring(ISON_SDS2TGW::TSH_TRANSACTION);

const char* CLOG_DIR = "SELog";
const char* JSON_FILE_DEFAULT = "config.json";

const vector<string> VECTOR_CONFIGKEY = { "version", "releasedate", "loglevel", "subendpoint_large", "pubendpoint_large", \
"large_volum_limit", "large_turnover_limit", "extra_sell_volum_limit", "extra_sell_turnover_limit", "extra_buy_volum_limit", "extra_buy_turnover_limit" };
const vector<string> VECTOR_CONFIGVALUE_DEFAULT = { "V0.00.01", "2015/01/26", "1", "tcp://192.168.15.200:2010", "tcp://192.168.15.200:2030" ,\
"40000",             "1000000",              "60000",                  "1500000",                   "60000",                 "1500000"                  };

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