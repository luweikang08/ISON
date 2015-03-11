#include "FluctuationMonitor.h"

using namespace ison::signal;

double GetTimeDiffInSec(DateAndTime start_src, DateAndTime end_src)
{
	struct tm tm_start, tm_end;

	tm_start.tm_year = start_src.date / 10000 - 1900;
	tm_start.tm_mon = (start_src.date % 10000) / 100 - 1;
	tm_start.tm_mday = start_src.date & 100;
	tm_start.tm_hour = start_src.time / 10000000 + 8;
	tm_start.tm_min = (start_src.time % 10000000) / 100000;
	tm_start.tm_sec = ((start_src.time % 10000000) % 100000) / 1000;

	tm_end.tm_year = end_src.date / 10000 - 1900;
	tm_end.tm_mon = (end_src.date % 10000) / 100 - 1;
	tm_end.tm_mday = end_src.date & 100;
	tm_end.tm_hour = end_src.time / 10000000 + 8;
	tm_end.tm_min = (end_src.time % 10000000) / 100000;
	tm_end.tm_sec = ((end_src.time % 10000000) % 100000) / 1000;

	time_t time_start = mktime(&tm_start);
	time_t time_end = mktime(&tm_end);
	return difftime(time_end, time_start);
}

FluctuationMonitor::FluctuationMonitor()
{

}
FluctuationMonitor::~FluctuationMonitor()
{
	
}

FM_STORE_RetCode FluctuationMonitor::Store(std::string level2_src)
{
	char srcBuf[BUFFELENGTH];
	memcpy(srcBuf, level2_src.c_str(), level2_src.size());

	baseline::MessageHeader hdr_src;
	baseline::SDS_Level2 LL_src;
	hdr_src.wrap(srcBuf, TOPICHEADSIZE, MESSAGEHEADERVERSION, BUFFELENGTH);
	LL_src.wrapForDecode(srcBuf, TOPICHEADSIZE + hdr_src.size(), hdr_src.blockLength(), hdr_src.version(), BUFFELENGTH);
	
	if ((LL_src.time() < 93000000) || (LL_src.time() > 150100000) || ((LL_src.time()>113100000) && (LL_src.time()<130000000)))//9:30-11:31,13:00-15:01
	{
		return FM_NULL;
	}

	if (LL_src.price() == LL_src.highLimited())
	{
		std::vector<std::string>::iterator it_v = std::find(HoldHighVector.begin(), HoldHighVector.end(), LL_src.code());
		if (it_v == HoldHighVector.end()) //target has not been found,publish is required
		{
			std::map<std::string, std::string>::iterator it_m = HighDataMap.find(LL_src.code());
			if (it_m == HighDataMap.end())
			{
				char storeBuf[BUFFELENGTH];
				memcpy(storeBuf, srcBuf + TOPICHEADSIZE, hdr_src.size() + LL_src.sbeBlockLength());
				std::string m_str_store(storeBuf, hdr_src.size() + LL_src.sbeBlockLength());
				HighDataMap.insert(std::pair<std::string, std::string>(LL_src.code(), m_str_store));
				return FM_INSERT;
			}
			else
			{
				char readBuf[BUFFELENGTH];
				memcpy(readBuf, it_m->second.c_str(), hdr_src.size() + LL_src.sbeBlockLength());
				baseline::MessageHeader hdr_read;
				baseline::SDS_Level2 LL_read;
				hdr_read.wrap(readBuf, 0, MESSAGEHEADERVERSION, BUFFELENGTH);
				LL_read.wrapForDecode(readBuf, hdr_read.size(), hdr_read.blockLength(), hdr_read.version(), BUFFELENGTH);
				DateAndTime dt_read, dt_src;
				dt_src.date = LL_src.date();
				dt_src.time = LL_src.time();
				dt_read.date = LL_read.date();
				dt_read.time = LL_read.time();
				if (GetTimeDiffInSec(dt_read, dt_src) > SECONDDIFF)
				{
					HoldHighVector.push_back(LL_src.code());
					PutInfo(SendStr, HOLDHIGH_SIGNALID, LL_src.code(), LL_src.date(), LL_src.time(), LL_src.price());
					return FM_HOLDHIGH;
				}
				else
				{
					return FM_NULL;
				}
			}
		}
		else
		{
			return FM_NULL;
		}
	}
	else if (LL_src.price() == LL_src.lowLimited())
	{
		std::vector<std::string>::iterator it_v = std::find(HoldLowVector.begin(), HoldLowVector.end(), LL_src.code());
		if (it_v == HoldLowVector.end()) //target has not been found
		{
			std::map<std::string, std::string>::iterator it_m = LowDataMap.find(LL_src.code());
			if (it_m == LowDataMap.end())
			{
				char storeBuf[BUFFELENGTH];
				memcpy(storeBuf, srcBuf + TOPICHEADSIZE, hdr_src.size() + LL_src.sbeBlockLength());
				std::string m_str_store(storeBuf, hdr_src.size() + LL_src.sbeBlockLength());
				LowDataMap.insert(std::pair<std::string, std::string>(LL_src.code(), m_str_store));
				return FM_INSERT;
			}
			else
			{
				char readBuf[BUFFELENGTH];
				memcpy(readBuf, it_m->second.c_str(), hdr_src.size() + LL_src.sbeBlockLength());
				baseline::MessageHeader hdr_read;
				baseline::SDS_Level2 LL_read;
				hdr_read.wrap(readBuf, 0, MESSAGEHEADERVERSION, BUFFELENGTH);
				LL_read.wrapForDecode(readBuf, hdr_read.size(), hdr_read.blockLength(), hdr_read.version(), BUFFELENGTH);
				DateAndTime dt_read, dt_src;
				dt_src.date = LL_src.date();
				dt_src.time = LL_src.time();
				dt_read.date = LL_read.date();
				dt_read.time = LL_read.time();
				if (GetTimeDiffInSec(dt_read, dt_src) > SECONDDIFF)
				{
					HoldLowVector.push_back(LL_src.code());
					PutInfo(SendStr, HOLDLOW_SIGNALID, LL_src.code(), LL_src.date(), LL_src.time(), LL_src.price());
					return FM_HOLDLOW;
				}
				else
				{
					return FM_NULL;
				}
			}
		}
		else
		{
			return FM_NULL;
		}
	}
	else
	{
		std::vector<std::string>::iterator it_v = std::find(HoldHighVector.begin(), HoldHighVector.end(), LL_src.code());
		if (it_v != HoldHighVector.end()) //target has been found
		{
			HoldHighVector.erase(it_v);
			HighDataMap.erase(LL_src.code());
			PutInfo(SendStr, OPENHIGH_SIGNALID, LL_src.code(), LL_src.date(), LL_src.time(), LL_src.price());
			return FM_OPENHIGH;
		}
		it_v = std::find(HoldLowVector.begin(), HoldLowVector.end(), LL_src.code());
		if (it_v != HoldLowVector.end()) //target has been found
		{
			HoldLowVector.erase(it_v);
			LowDataMap.erase(LL_src.code());
			PutInfo(SendStr, OPENLOW_SIGNALID, LL_src.code(), LL_src.date(), LL_src.time(), LL_src.price());
			return FM_OPENHIGH;
		}
		return FM_NULL;
	}
}

//TOPICHEAD+MessageHeader+SDS_Signal
std::string FluctuationMonitor::MakeSendStr(int topic, int sn)
{
	char SendBuf[BUFFELENGTH];
	char Code[16];
	TOPICHEAD TopicHeadSend;
	baseline::MessageHeader HdrSend;
	baseline::SDS_Signal Signal;

	memcpy(SendBuf, SendStr.c_str(), SendStr.size());
	HdrSend.wrap(SendBuf, 0, MESSAGEHEADERVERSION, BUFFELENGTH);
	Signal.wrapForDecode(SendBuf, HdrSend.size(), HdrSend.blockLength(), HdrSend.version(), BUFFELENGTH);

	TopicHeadSend.topic = topic;
	DateAndTime m_dtm = GetDateAndTime();
	TopicHeadSend.ms = (m_dtm.time % 1000);
	memcpy(Code, Signal.code(), 16);
	TopicHeadSend.kw = atoi(Code);
	TopicHeadSend.sn = sn;
	unsigned int num_tm;
	DateTime2Second(m_dtm.date, m_dtm.time, num_tm);
	TopicHeadSend.tm = num_tm;
	memcpy(SendBuf, &TopicHeadSend, sizeof(TOPICHEAD));

	std::string str_temp;
	str_temp.assign(SendBuf, sizeof(TOPICHEAD));
	str_temp.append(SendStr);
	return str_temp;
}

const char *FluctuationMonitor::GetDataCode(void) const
{
	if (SendStr.size() > 0)
	{
		char Buffer[BUFFELENGTH];
		baseline::MessageHeader HdrSend;
		baseline::SDS_Signal Signal;

		memcpy(Buffer, SendStr.c_str(), SendStr.size());
		HdrSend.wrap(Buffer, 0, MESSAGEHEADERVERSION, BUFFELENGTH);
		Signal.wrapForDecode(Buffer, HdrSend.size(), HdrSend.blockLength(), HdrSend.version(), BUFFELENGTH);

		return Signal.code();
	}
	return 0;
}
unsigned int FluctuationMonitor::GetDataDate()
{
	if (SendStr.size() > 0)
	{
		char Buffer[BUFFELENGTH];
		baseline::MessageHeader HdrSend;
		baseline::SDS_Signal Signal;

		memcpy(Buffer, SendStr.c_str(), SendStr.size());
		HdrSend.wrap(Buffer, 0, MESSAGEHEADERVERSION, BUFFELENGTH);
		Signal.wrapForDecode(Buffer, HdrSend.size(), HdrSend.blockLength(), HdrSend.version(), BUFFELENGTH);

		return Signal.date();
	}
}
unsigned int FluctuationMonitor::GetDataTime()
{
	if (SendStr.size() > 0)
	{
		char Buffer[BUFFELENGTH];
		baseline::MessageHeader HdrSend;
		baseline::SDS_Signal Signal;

		memcpy(Buffer, SendStr.c_str(), SendStr.size());
		HdrSend.wrap(Buffer, 0, MESSAGEHEADERVERSION, BUFFELENGTH);
		Signal.wrapForDecode(Buffer, HdrSend.size(), HdrSend.blockLength(), HdrSend.version(), BUFFELENGTH);

		return Signal.time();
	}
	return 0;
}
