#include "LargeTranMonitor.h"

using namespace ison::signal;

LargeTranMonitor::LargeTranMonitor()
{

}
LargeTranMonitor::LargeTranMonitor(std::string src)
{
	memcpy(StoreBuffer, src.c_str(), src.size());
}
LargeTranMonitor::~LargeTranMonitor()
{
	
}

int LargeTranMonitor::Store(std::string src)
{
	memcpy(StoreBuffer, src.c_str(), src.size());
	TOPICHEAD *topichead_temp;
	topichead_temp = (TOPICHEAD*)StoreBuffer;
	MessHdr.wrap(StoreBuffer, TOPICHEADSIZE, MESSAGEHEADERVERSION, BUFFELENGTH);
	Tran.wrapForDecode(StoreBuffer, TOPICHEADSIZE + MessHdr.size(), MessHdr.blockLength(), MessHdr.version(), BUFFELENGTH);
	return 0;
}

EqualFlag LargeTranMonitor::CompStatus(unsigned int volumelimit, unsigned int turnoverlimit)
{
	if (volumelimit <= 0)
	{
		return EF_Error;
	}
	if (Tran.volume() >= volumelimit)
	{
		return EF_Large;
	}
	else if (turnoverlimit > 0)
	{
		if (Tran.turnover() >= turnoverlimit)
		{
			return EF_Large;
		}
		else
			return EF_Small;
	}
	else
	{
		return EF_Small;
	}
}
//TOPICHEAD+MessageHeader+SDS_Signal
std::string LargeTranMonitor::MakeSendStr(int topic, int sn)
{
	char SendBuf[BUFFELENGTH];
	char Code[16];
	TOPICHEAD TopicHeadSend;
	baseline::MessageHeader HdrSend;
	baseline::SDS_Signal Signal;

	TopicHeadSend.topic = topic;
	DateAndTime m_dtm = GetDateAndTime();
	TopicHeadSend.ms = (m_dtm.time % 1000);
	memcpy(Code, Tran.code(), 16);
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
	if (Tran.time() < 93000000)
	{
		Signal.signalID(AUCTION_SIGNAID);
	}
	else if (Tran.bSFlag() == 'B' || Tran.bSFlag() == 'b') //put signalid in data
	{
		Signal.signalID(BUY_SIGNALID);
	}
	else if (Tran.bSFlag() == 'S' || Tran.bSFlag() == 's')
	{
		Signal.signalID(SELL_SIGNALID);
	}
	else
	{
		Signal.signalID(LTMERROR_SIGNALID);
	}
	Signal.putCode(Code);
	Signal.date(Tran.date());
	Signal.time(Tran.time());
	std::stringstream strStm;
	strStm << (int)(Tran.volume() / 100);
	Signal.putInfo(strStm.str().c_str());
	std::string SendStr(SendBuf, sizeof(TOPICHEAD) + HdrSend.size() + Signal.sbeBlockLength());
	return SendStr;
}

const char *LargeTranMonitor::GetDataCode(void) const
{
	return Tran.code();
}
unsigned int LargeTranMonitor::GetDataDate()
{
	return Tran.date();
}
unsigned int LargeTranMonitor::GetDataTime()
{
	return Tran.time();
}
unsigned int LargeTranMonitor::GetDataVolume()
{
	return Tran.volume();
}
unsigned int LargeTranMonitor::GetDataTurnover()
{
	return Tran.turnover();
}
char LargeTranMonitor::GetDatabSFlag()
{
	return Tran.bSFlag();
}