#ifndef __ISON_MACD_BASE_H__
#define __ISON_MACD_BASE_H__

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

#define DATACNT 50 //data count in queue
#define FASTPERIOD 12
#define SLOWPERIOD 26
#define SIGNALPERIOD 9

enum MACD_STORE_RetCode
{
	MACD_ERROR,
	MACD_ADD,      //new stockid store to map
	MACD_INSERT,   //queue.size() is less then DATACNT
	MACD_REPLACE   //queue.size() is equal to DATACNT,pop oldest push new
};

namespace ison{
	namespace indicator{
		class MacdBase
		{
		public:
			MacdBase();
			~MacdBase();
			MACD_STORE_RetCode Store(std::string kline_src);
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
