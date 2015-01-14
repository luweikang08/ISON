#ifndef __ISON_SDS_RAPID_MOVE_H__
#define __ISON_SDS_RAPID_MOVE_H__

#include <iostream>
#include "isonbase.h"
#include "iblog.h"
#include "context.h"
#include "stage.h"
#include "actor.h"
#include "event.h"
#include "ibprotocol.h"

#include "timehelper.h"
#include "stringhelper.h"
#include "readconfig.h"
#include "MessageHeader.hpp"
#include "isonsdsdataapi.h"
#include "SDS_Transaction.hpp"
#include "data_struct.h"

#define LightCnt 3

#define SIGNALID 2

#define  MOVE_VERSION      "V0.00.01"
#define  MOVE_RELEASEDATE  "2015/01/13"
#define  MOVE_LOGLEVEL     "1"
#define  MOVE_SUBTOPIC     "1014,1114"
#define  MOVE_SUBENDPOINT  "tcp://192.168.15.200:2010"
#define  MOVE_PUBTOPIC     "2012,2112"
#define  MOVE_PUBENDPOINT  "tcp://192.168.15.200:2012"
#define  MOVE_RISELIMIT    1.8
#define  MOVE_FALLLIMIT    1.8

#define CONFIG_KEY_VECTOR_DEFAULT \
{ "version", "releasedate", "loglevel", "subtopic", "subendpoint", "pubtopic", "pubendpoint", "riselimit", "falllimit" }
#define JSON_FILE_DEFAULT "config.json"

struct LightData
{
	int time;
	int price;
};

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

#endif