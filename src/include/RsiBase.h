#ifndef __ISON_RSI_BASE_H__
#define __ISON_RSI_BASE_H__

#include <map>
#include "ta_common.h"
#include "ta_func.h"

#include "isonsdsdataapi.h"
#include "MessageHeader.hpp"
#include "timehelper.h"
#include "data_defs.h"
#include "SDS_Kline.hpp"
#include "SDS_Level2.hpp"
#include "SDS_Indicator.hpp"

#define DATACNT    25 //src data count in array
#define RSI1PERIOD 6
#define RSI2PERIOD 12
#define RSI3PERIOD 24

enum RSI_STORE_RetCode
{
	RSI_ERROR,
	RSI_ADD,      //new stockid store to map
	RSI_INSERT,   //queue.size() is less then DATACNT
	RSI_REPLACE   //queue.size() is equal to DATACNT,pop oldest push new
};

namespace ison{
	namespace indicator{
		class RsiBase
		{
		public:
			RsiBase();
			~RsiBase();
			RSI_STORE_RetCode Store(std::string kline_src);
			void GetResult(double Result[]);
			std::string MakeSendStr(int topic, int sn);
			const char* GetDataCode()const;
		private:
			std::map<std::string, std::vector<int>> DataMap;//store price data from level2
			std::string SendStr;    //string need to publish,messageheader+SDS_Indicator,no topichead
			double ResultDataArr[3]; //0:K,1:D,2:J
		};
	}
}

#endif
