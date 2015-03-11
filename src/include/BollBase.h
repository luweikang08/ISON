#ifndef __ISON_BOLL_BASE_H__
#define __ISON_BOLL_BASE_H__

#include <map>
#include <queue>
#include "ta_common.h"
#include "ta_func.h"

#include "isonsdsdataapi.h"
#include "MessageHeader.hpp"
#include "timehelper.h"
#include "data_defs.h"
#include "SDS_Kline.hpp"
#include "SDS_Level2.hpp"
#include "SDS_Indicator.hpp"

#define DATACNTBOLL 50 //data count in queue
#define TIMEPERIODBOLL 20
#define NBDEVUP    2
#define NBDEVDN    2
#define MATYPE	   TA_MAType_SMA

#ifdef NOTOPICHEAD
#define TOPICHEADSIZE 0
#else 
#define TOPICHEADSIZE sizeof(TOPICHEAD)
#endif

enum BOLL_STORE_RetCode
{
	BOLL_ERROR,
	BOLL_ADD,      //new stockid store to map
	BOLL_INSERT,   //queue.size() is less then DATACNT
	BOLL_REPLACE   //queue.size() is equal to DATACNT,pop oldest push new
};

namespace ison{
	namespace indicator{
		class BollBase
		{
		public:
			BollBase();
			~BollBase();
			BOLL_STORE_RetCode Store(std::string kline_src);
			void GetResult(double Result[]);
			std::string MakeSendStr(int topic, int sn);
			const char* GetDataCode()const;
		private:
			std::map<std::string, std::vector<int>> DataMap;//store price data from level2
			std::string SendStr;    //string need to publish,messageheader+SDS_Indicator,no topichead
			double ResultDataArr[3];
		};
	}
}

#endif
