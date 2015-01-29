#ifndef __ISON_RAPID_MOVE_MONITOR_H__
#define __ISON_RAPID_MOVE_MONITOR_H__

#include <map>
#include <sstream>
#include <string>
#include "isonsdsdataapi.h"
#include "isonbase.h"
#include "timehelper.h"
#include "MessageHeader.hpp"
#include "SDS_Kline.hpp"
#include "SDS_Signal.hpp"
#include "data_defs.h"

#define RISE_SIGNALID     4
#define FALL_SIGNALID     5

enum RM_STORE_RetCode
{
	RM_INSERT, //new stockid receive
	RM_UPDATE, //update volume tunrover.....
	RM_REPLACE //data time is next minute
};

inline float volat(int base, int value)
{
	return ((float)((value - base) * 100)) / base;
}

namespace ison{
	namespace signal{
		class RapidMoveMinitor
		{
		public:
			RapidMoveMinitor();
			~RapidMoveMinitor();

			RM_STORE_RetCode Store(std::string src);
			void CompStatus(double rise_limit, double fall_limit);
			std::string MakeSendStr(int topic, int sn, double rise_limit, double fall_limit);
			const char *GetDataCode(void);
			unsigned int GetDataDate();
			unsigned int GetDataTime();
			unsigned int GetDataVolume();
			unsigned int GetDataTurnover();
			double GetMovePercent();
			bool isExist(const char* code);
			bool isNeedPub();
			void resetPubFlag();
		private:
			DISALLOW_COPY_AND_ASSIGN(RapidMoveMinitor);
			std::map<std::string, std::string> DataMap;//store data,<code,sbedata>,sbedata is messageheader+sds_kline,no topichead
			std::string SendStr;    //string need to publish,messageheader+sds_signal,no topichead
			double MovePercent;
			char StoreBuffer[BUFFELENGTH];
			bool NeedPubFlag = false;
		};
	}
}
#endif