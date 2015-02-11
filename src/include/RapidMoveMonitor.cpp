#include "RapidMoveMonitor.h"

using namespace ison::signal;

RapidMoveMinitor::RapidMoveMinitor()
{

}

RapidMoveMinitor::~RapidMoveMinitor()
{
	
}

RM_STORE_RetCode RapidMoveMinitor::Store(std::string src)
{
	char srcBuf[BUFFELENGTH];
	std::memcpy(srcBuf, src.c_str(), src.size());

	baseline::MessageHeader hdr_src;
	baseline::SDS_Kline KK_src;

	hdr_src.wrap(srcBuf + sizeof(TOPICHEAD), 0, MESSAGEHEADERVERSION, BUFFELENGTH);//parse messageheader
	KK_src.wrapForDecode(srcBuf, sizeof(TOPICHEAD) + hdr_src.size(), hdr_src.blockLength(), hdr_src.version(), BUFFELENGTH);

	std::map < std::string, std::string>::iterator it = DataMap.find(KK_src.code());
	if (it == DataMap.end())
	{
		std::string m_str_Store(srcBuf + sizeof(TOPICHEAD), hdr_src.size() + KK_src.sbeBlockLength());
		DataMap.insert(std::pair<std::string, std::string>(KK_src.code(), m_str_Store));
		return RM_INSERT;
	}
	else //new data received update.
	{
		char readBuf[BUFFELENGTH];
		SendStr.clear();
		
		baseline::SDS_Kline KK_read;
		std::memcpy(readBuf, it->second.c_str(), hdr_src.size() + KK_read.sbeBlockLength());
		KK_read.wrapForDecode(readBuf, hdr_src.size(), KK_read.sbeBlockLength(), hdr_src.version(), BUFFELENGTH);
		MovePercent = volat(KK_read.preClose(), KK_src.close());

		std::memcpy(StoreBuffer, src.c_str(), src.size());

		std::string m_str_Store(srcBuf + sizeof(TOPICHEAD), hdr_src.size() + KK_src.sbeBlockLength());
		it->second = m_str_Store;
		return RM_UPDATE;
	}
}

void RapidMoveMinitor::CompStatus(double rise_limit, double fall_limit)
{
	if (MovePercent >= rise_limit || MovePercent <= (0 - fall_limit))
	{
		NeedPubFlag = true;
	}
}
std::string RapidMoveMinitor::MakeSendStr(int topic, int sn, double rise_limit, double fall_limit)
{
	char SendBuf[BUFFELENGTH];
	char Code[16];
	TOPICHEAD TopicHeadSend;
	baseline::MessageHeader HdrSend;
	baseline::SDS_Signal Signal;

	baseline::MessageHeader hdr;
	baseline::SDS_Kline KK;
	hdr.wrap(StoreBuffer, sizeof(TOPICHEAD), hdr.version(), BUFFELENGTH);
	KK.wrapForDecode(StoreBuffer, sizeof(TOPICHEAD) + hdr.size(), hdr.blockLength(), hdr.version(), BUFFELENGTH);
	TopicHeadSend.topic = topic;
	DateAndTime m_dtm = GetDateAndTime();
	TopicHeadSend.ms = (m_dtm.time % 1000);
	std::memcpy(Code, KK.code(), 16);
	TopicHeadSend.kw = atoi(Code);
	TopicHeadSend.sn = sn;
	unsigned int num_tm;
	DateTime2Second(m_dtm.date, m_dtm.time, num_tm);
	TopicHeadSend.tm = num_tm;
	memcpy(SendBuf, &TopicHeadSend, sizeof(TOPICHEAD));

	HdrSend.wrap(SendBuf, sizeof(TOPICHEAD), 0, BUFFELENGTH)                          //wrap messageheader
		.blockLength(baseline::SDS_Signal::sbeBlockLength())
		.templateId(baseline::SDS_Signal::sbeTemplateId())
		.schemaId(baseline::SDS_Signal::sbeSchemaId())
		.version(baseline::SDS_Signal::sbeSchemaVersion());

	Signal.wrapForEncode(SendBuf, HdrSend.size() + sizeof(TOPICHEAD), BUFFELENGTH);
	if (MovePercent > rise_limit) //put signalid in data
	{
		Signal.signalID(RISE_SIGNALID);
	}
	else if (MovePercent < (0 - fall_limit))
	{
		Signal.signalID(FALL_SIGNALID);
	}
	Signal.putCode(Code);
	Signal.date(KK.date());
	Signal.time(KK.time());
	std::stringstream strStm;
	strStm << MovePercent;
	Signal.putInfo(strStm.str().c_str());
	std::string m_SendStr(SendBuf, sizeof(TOPICHEAD) + HdrSend.size() + Signal.sbeBlockLength());
	return m_SendStr;
}

const char *RapidMoveMinitor::GetDataCode(void)
{
	baseline::MessageHeader hdr;
	baseline::SDS_Kline KK;
	hdr.wrap(StoreBuffer, sizeof(TOPICHEAD), hdr.version(), BUFFELENGTH);
	KK.wrapForDecode(StoreBuffer, sizeof(TOPICHEAD) + hdr.size(), hdr.blockLength(), hdr.version(), BUFFELENGTH);
	return KK.code();
}
unsigned int RapidMoveMinitor::GetDataDate()
{
	baseline::MessageHeader hdr;
	baseline::SDS_Kline KK;
	hdr.wrap(StoreBuffer, sizeof(TOPICHEAD), hdr.version(), BUFFELENGTH);
	KK.wrapForDecode(StoreBuffer, sizeof(TOPICHEAD) + hdr.size(), hdr.blockLength(), hdr.version(), BUFFELENGTH);
	return KK.date();
}
unsigned int RapidMoveMinitor::GetDataTime()
{
	baseline::MessageHeader hdr;
	baseline::SDS_Kline KK;
	hdr.wrap(StoreBuffer, sizeof(TOPICHEAD), hdr.version(), BUFFELENGTH);
	KK.wrapForDecode(StoreBuffer, sizeof(TOPICHEAD) + hdr.size(), hdr.blockLength(), hdr.version(), BUFFELENGTH);
	return KK.time();
}
unsigned int RapidMoveMinitor::GetDataVolume()
{
	baseline::MessageHeader hdr;
	baseline::SDS_Kline KK;
	hdr.wrap(StoreBuffer, sizeof(TOPICHEAD), hdr.version(), BUFFELENGTH);
	KK.wrapForDecode(StoreBuffer, sizeof(TOPICHEAD) + hdr.size(), hdr.blockLength(), hdr.version(), BUFFELENGTH);
	return KK.volume();
}
double RapidMoveMinitor::GetMovePercent()
{
	return MovePercent;
}
unsigned int RapidMoveMinitor::GetDataTurnover()
{
	baseline::MessageHeader hdr;
	baseline::SDS_Kline KK;
	hdr.wrap(StoreBuffer, sizeof(TOPICHEAD), hdr.version(), BUFFELENGTH);
	KK.wrapForDecode(StoreBuffer, sizeof(TOPICHEAD) + hdr.size(), hdr.blockLength(), hdr.version(), BUFFELENGTH);
	return KK.turnover();
}

bool RapidMoveMinitor::IsExist(const char* code)
{
	std::map<std::string, std::string>::iterator it = DataMap.find(code);
	if (it == DataMap.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}
bool RapidMoveMinitor::IsNeedPub()
{
	return NeedPubFlag;
}
void RapidMoveMinitor::ResetPubFlag()
{
	NeedPubFlag = false;
}