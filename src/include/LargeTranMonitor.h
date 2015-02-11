#ifndef __ISON_LARGE_TRAN_MONITOR_H__
#define __ISON_LARGE_TRAN_MONITOR_H__

#include <sstream>
#include <string>
#include "isonsdsdataapi.h"
#include "isonbase.h"
#include "timehelper.h"
#include "MessageHeader.hpp"
#include "SDS_Transaction.hpp"
#include "SDS_Signal.hpp"
#include "data_defs.h"

enum EqualFlag
{
	EF_Error,
	EF_Large = 1,
	EF_Equal,
	EF_Small
};

namespace ison{
	namespace signal{
		class LargeTranMonitor
		{
		public:
			LargeTranMonitor();
			explicit LargeTranMonitor(std::string src);
			~LargeTranMonitor();

			int Store(std::string src);
			EqualFlag CompStatus(unsigned int volume,unsigned int turnover = 0);
			std::string MakeSendStr(int topic, int sn);
			const char *GetDataCode(void) const;
			unsigned int GetDataDate();
			unsigned int GetDataTime();
			unsigned int GetDataVolume();
			unsigned int GetDataTurnover();
			char GetDatabSFlag();
		private:
			DISALLOW_COPY_AND_ASSIGN(LargeTranMonitor);
			char StoreBuffer[BUFFELENGTH];
			baseline::MessageHeader MessHdr;
			baseline::SDS_Transaction Tran;
		};
	}
}
#endif