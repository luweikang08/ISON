#ifndef __ISON_SDS_TRADE_ALERT_H__
#define __ISON_SDS_TRADE_ALERT_H__

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

#include "readconfig.h"
#include "stringhelper.h"
#include "data_struct.h"

#include "LargeTranMonitor.h"


#define BUY_SIGNALID      1
#define SELL_SIGNALID     2
#define AUCTION_SIGNAID   3


#define VOLUM_LIMIT               40000
#define TURNOVER_LIMIT            1000000
#define EXTRA_SELL_VOLUM_LIMIT    60000
#define EXTRA_SELL_TURNOVER_LIMIT 1500000
#define EXTRA_BUY_VOLUM_LIMIT     60000
#define EXTRA_BUY_TURNOVER_LIMIT  1500000

#define  ALERT_VERSION      "V0.00.05"
#define  ALERT_RELEASEDATE  "2015/01/22"
#define  ALERT_LOGLEVEL     "1"
#define  ALERT_SUBTOPIC     "1014,1114"
#define  ALERT_SUBENDPOINT  "tcp://192.168.15.200:2010"
#define  ALERT_PUBTOPIC     "2011,2111"
#define  ALERT_PUBENDPOINT  "tcp://192.168.15.200:2012"

#define CONFIG_KEY_VECTOR_DEFAULT \
{ "version", "releasedate", "loglevel", "subtopic", "subendpoint", "pubtopic", "pubendpoint", \
"volum_limit", "turnover_limit", "extra_sell_volum_limit", "extra_sell_turnover_limit", "extra_buy_volum_limit", "extra_buy_turnover_limit" }
#define JSON_FILE_DEFAULT "config.json"

inline void sbe2struct(baseline::SDS_Transaction& src, TranData& dest)
{
	dest.Seqno = src.seqno();
	strcpy_s(dest.Code, src.code());
	dest.Date = src.date();
	dest.Time = src.time();
	dest.SDSTime = src.sDSTime();
	dest.Sn = src.sn();
	dest.Index = src.index();
	dest.Price = src.price();
	dest.Volum = src.volume();
	dest.Turnover = src.turnover();
	dest.BSFlag = src.bSFlag();
	dest.OrderKind = src.orderKind();
	dest.FunctionCode = src.functionCode();
	dest.AskOrder = src.askOrder();
	dest.BidOrder = src.bidOrder();
}

inline void makeinfo(baseline::SDS_Transaction& src, string& dest)
{
	std::stringstream s;
	s << (int)(src.volume()/100);
	dest = s.str();
}

#endif