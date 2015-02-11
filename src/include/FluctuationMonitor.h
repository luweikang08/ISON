#ifndef __ISON_FLUCTUATION_MONITOR_H__
#define __ISON_FLUCTUATION_MONITOR_H__

#include <map>
#include <sstream>
#include <string>
#include <time.h>
#include "isonsdsdataapi.h"
#include "isonbase.h"
#include "timehelper.h"
#include "MessageHeader.hpp"
#include "SDS_Transaction.hpp"
#include "SDS_Level2.hpp"
#include "SDS_Signal.hpp"
#include "data_defs.h"

#ifdef NOTOPICHEAD
#define TOPICHEADSIZE 0
#else 
#define TOPICHEADSIZE sizeof(TOPICHEAD)
#endif
#define SECONDDIFF 30

enum FM_STORE_RetCode
{
	FM_NULL,
	FM_INSERT,      //new stockid receive
	FM_REPLACE,     //new data replace old data
	FM_HOLDHIGH,    //hold on high limit
	FM_OPENHIGH,    //cann't hold on daily limit
	FM_HOLDLOW,
	FM_OPENLOW
};

double GetTimeDiffInSec(DateAndTime start_src, DateAndTime end_src);

inline void PutInfo(std::string& str,const int id,const char* code,const int date,const int time,const int price)
{
	str.clear();
	char sendBuf[BUFFELENGTH];
	baseline::MessageHeader hdr_send;
	baseline::SDS_Signal Signal;
	hdr_send.wrap(sendBuf, 0, MESSAGEHEADERVERSION, BUFFELENGTH)
		.blockLength(baseline::SDS_Signal::sbeBlockLength())
		.templateId(baseline::SDS_Signal::sbeTemplateId())
		.schemaId(baseline::SDS_Signal::sbeSchemaId())
		.version(baseline::SDS_Signal::sbeSchemaVersion());
	Signal.wrapForEncode(sendBuf, hdr_send.size(), BUFFELENGTH)
		.signalID(id)
		.putCode(code)
		.date(date)
		.time(time);
	std::stringstream strStm;
	strStm << price;
	Signal.putInfo(strStm.str().c_str());
	str.assign(sendBuf, hdr_send.size() + Signal.sbeBlockLength());
}

namespace ison{
	namespace signal{
		class FluctuationMonitor
		{
		public:
			FluctuationMonitor();
			~FluctuationMonitor();

			FM_STORE_RetCode Store(std::string level2_src);
			std::string MakeSendStr(int topic, int sn);
			const char *GetDataCode(void) const;
			unsigned int GetDataDate();
			unsigned int GetDataTime();
		private:
			DISALLOW_COPY_AND_ASSIGN(FluctuationMonitor);
			std::map<std::string, std::string> HighDataMap;//store data,<code,sbedata>,sbedata is messageheader+sds_kline,no topichead
			std::map<std::string, std::string> LowDataMap;   //store flag,to determine if the next pubblish() is necessary.
			std::vector<std::string> HoldHighVector;   //store flag,to determine if the next pubblish() is necessary.
			std::vector<std::string> HoldLowVector;    //store flag,to determine if the next pubblish() is necessary.
			std::string SendStr;    //string need to publish,messageheader+sds_signal,no topichead
		};
	}
}
#endif