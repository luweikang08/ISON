#ifndef __ISON_KDJ_BASE_H__
#define __ISON_KDJ_BASE_H__

#include <map>
#include <queue>
#include "kdj_func.h"

#include "isonsdsdataapi.h"
#include "MessageHeader.hpp"
#include "timehelper.h"
#include "data_defs.h"
#include "SDS_Kline.hpp"
#include "SDS_Level2.hpp"
#include "SDS_Indicator.hpp"

#define DATACNTKDJ    10 //src data count in array
#define TIMEPERIODKDJ 9
#define KPERIOD    3
#define DPERIOD    3

#ifdef NOTOPICHEAD
#define TOPICHEADSIZE 0
#else 
#define TOPICHEADSIZE sizeof(TOPICHEAD)
#endif

enum KDJ_STORE_RetCode
{
	KDJ_ERROR,
	KDJ_ADD,      //new stockid store to map
	KDJ_INSERT,   //queue.size() is less then DATACNT
	KDJ_REPLACE   //queue.size() is equal to DATACNT,pop oldest push new
};

namespace ison{
	namespace indicator{
		class KdjBase
		{
		public:
			KdjBase();
			~KdjBase();
			KDJ_STORE_RetCode Store(std::string kline_src);
			void GetResult(double Result[]);
			std::string MakeSendStr(int topic, int sn);
			const char* GetDataCode()const;
		private:
			std::map<std::string, std::vector<std::vector<int>>> DataMap;//store price data from level2
			std::string SendStr;    //string need to publish,messageheader+SDS_Indicator,no topichead
			double ResultDataArr[3]; //0:K,1:D,2:J
		};
	}
}

#endif
