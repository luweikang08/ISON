#ifndef __ISON_KLINE_BASE_H__
#define __ISON_KLINE_BASE_H__

#include <map>
#include "isonbase.h"
#include "isonsdsdataapi.h"
#include "data_struct.h"
#include "data_defs.h"
#include "timehelper.h"
#include "MessageHeader.hpp"
#include "SDS_Transaction.hpp"
#include "SDS_Kline.hpp"

enum KL_STORE_RetCode
{
	KL_INSERT, //new stockid receive
	KL_UPDATE, //update volume tunrover.....
	KL_REPLACE //data time is next minute
};

int getmm(int time); //get minute
int getmm(std::string sbe_kline);
int gethhmm(int time); //get hour and minute
int gethhmm(std::string sbe_trans);

namespace ison
{
	namespace indicator
	{
		class KLineBase
		{
		public:
			KLineBase();
			~KLineBase();
			KL_STORE_RetCode Store(std::string src);  //store data to map
			std::string GetSendStr();
			std::string MakeSendStr(int topic, int sn);
			const char* GetDataCode();
			bool isExist(const char* code);
			bool isNeedPub();
			void resetPubFlag();
		private:
			DISALLOW_COPY_AND_ASSIGN(KLineBase);
			std::map<std::string, std::string> DataMap;//store data,<code,sbedata>,sbedata is messageheader+sds_kline,no topichead
			std::string SendStr;    //string need to publish,messageheader+sds_kline,no topichead
			bool NeedPubFlag = false;
			TS TimeStatus = OneMinute;
		};
	}
}

#endif