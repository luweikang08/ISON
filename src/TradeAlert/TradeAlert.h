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

#include "MessageHeader.hpp"
#include "SDS_Transaction.hpp"
#include "SDS_Level2.hpp"
#include "SDS_Signal.hpp"
#include "isonsdsdataapi.h"
#include "data_struct.h"
#include "stringhelper.h"
#include "timehelper.h"

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
	rapidjson::Document m_Document;
	m_Document.SetObject();
	rapidjson::Document::AllocatorType& m_Allocator = m_Document.GetAllocator();

	rapidjson::Value m_Value_Code;
	m_Document.AddMember("bsflag", src.bSFlag(), m_Allocator);
	m_Document.AddMember("volum", src.volume(), m_Allocator);
	dest = Document2String(m_Document);
}

#endif