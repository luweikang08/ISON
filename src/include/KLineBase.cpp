#include "KLineBase.h"

using namespace ison::indicator;

int getmm(int time) //get minute
{
	return ((time / 100000) % 100);
}

int getmm(std::string sbe_kline)
{
	char recvBuf[BUFFELENGTH];
	std::memcpy(recvBuf, sbe_kline.c_str(), sbe_kline.size());
	TOPICHEAD  * m_topichead_rec;
	m_topichead_rec = (TOPICHEAD*)recvBuf;

	baseline::SDS_Kline  KK;
	baseline::MessageHeader hdr;
	hdr.wrap(recvBuf, 0, MESSAGEHEADERVERSION, BUFFELENGTH);//parse messageheader
	KK.wrapForDecode(recvBuf, hdr.size(), hdr.blockLength(), hdr.version(), BUFFELENGTH);
	return getmm(KK.time());
}

int gethhmm(int time) //get hour and minute
{
	return (time / 100000);
}

int gethhmm(std::string sbe_trans)
{
	char recvBuf[1024];
	std::memcpy(recvBuf, sbe_trans.c_str(), sbe_trans.size());
	TOPICHEAD  * m_topichead_rec;
	m_topichead_rec = (TOPICHEAD*)recvBuf;

	baseline::SDS_Transaction  CC;
	baseline::MessageHeader hdr;
	hdr.wrap(recvBuf, 0, MESSAGEHEADERVERSION, 1024);//parse messageheader
	CC.wrapForDecode(recvBuf, hdr.size(), hdr.blockLength(), hdr.version(), 1024);
	return gethhmm(CC.time());
}

KLineBase::KLineBase()
{
}

KLineBase::~KLineBase()
{
}
KL_STORE_RetCode KLineBase::Store(std::string src)
{
	char srcBuf[BUFFELENGTH];
	std::memcpy(srcBuf, src.c_str(), src.size());

	baseline::MessageHeader hdr_src;
	baseline::SDS_Transaction TT_src;

	hdr_src.wrap(srcBuf + sizeof(TOPICHEAD), 0, MESSAGEHEADERVERSION, BUFFELENGTH);//parse messageheader
	TT_src.wrapForDecode(srcBuf, sizeof(TOPICHEAD) + hdr_src.size(), hdr_src.blockLength(), hdr_src.version(), BUFFELENGTH);

	std::map < std::string, std::string>::iterator it = DataMap.find(TT_src.code());
	if (it == DataMap.end())
	{
		std::string m_str_Store(srcBuf + sizeof(TOPICHEAD), hdr_src.size() + TT_src.sbeBlockLength());
		DataMap.insert(std::pair<std::string, std::string>(TT_src.code(), m_str_Store));
		return KL_INSERT;
	}
	else
	{
		baseline::MessageHeader hdr_read;
		baseline::SDS_Kline KK_read;

		char readBuf[BUFFELENGTH];
		std::memcpy(readBuf, it->second.c_str(), hdr_read.size() + KK_read.sbeBlockLength());

		hdr_read.wrap(readBuf, 0, MESSAGEHEADERVERSION, BUFFELENGTH);
		KK_read.wrapForDecode(readBuf, hdr_read.size(), hdr_read.blockLength(), hdr_read.version(), BUFFELENGTH);
		if (getmm(KK_read.time()) == getmm(TT_src.time())) //the data in map is not the last data in this minute,update it
		{
			KK_read.time(TT_src.time());
			KK_read.close(TT_src.price());
			if (TT_src.price() > KK_read.high())
			{
				KK_read.high(TT_src.price());
			}
			else if (TT_src.price()<KK_read.low())
			{
				KK_read.low(TT_src.price());
			}
			unsigned long long temp;
			temp = KK_read.volume();
			temp += TT_src.volume();
			KK_read.volume(temp);
			temp = KK_read.turnover();
			temp += TT_src.turnover();
			KK_read.turnover(temp);
			std::string m_str_Store(readBuf, hdr_read.size() + KK_read.sbeBlockLength());
			it->second = m_str_Store;
			return KL_UPDATE;
		}
		else //the data received is in the next minute,publish is required
		{
			SendStr.clear();
			SendStr.assign(readBuf, hdr_read.size() + KK_read.sbeBlockLength());
			NeedPubFlag = true;
			std::string m_str_Store(srcBuf + sizeof(TOPICHEAD), hdr_src.size() + TT_src.sbeBlockLength());
			it->second = m_str_Store;
			return KL_REPLACE;
		}
	}
}
std::string KLineBase::GetSendStr()
{
	return SendStr;
}
std::string KLineBase::MakeSendStr(int topic, int sn)
{
	char SendBuf[BUFFELENGTH];
	char Code[16];
	TOPICHEAD TopicHeadSend;
	baseline::MessageHeader HdrSend;
	baseline::SDS_Kline KK;

	std::memcpy(SendBuf, SendStr.c_str(), SendStr.size());
	HdrSend.wrap(SendBuf, 0, MESSAGEHEADERVERSION, BUFFELENGTH);
	KK.wrapForDecode(SendBuf, HdrSend.size(), HdrSend.blockLength(), HdrSend.version(), BUFFELENGTH);

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

	std::string str_temp;
	str_temp.assign(SendBuf, sizeof(TOPICHEAD));
	str_temp.append(SendStr);
	return str_temp;
}

const char* KLineBase::GetDataCode()
{
	if (SendStr.size() > 0)
	{
		char Buffer[BUFFELENGTH];
		baseline::MessageHeader HdrSend;
		baseline::SDS_Kline KK;

		std::memcpy(Buffer, SendStr.c_str(), SendStr.size());
		HdrSend.wrap(Buffer, 0, MESSAGEHEADERVERSION, BUFFELENGTH);
		KK.wrapForDecode(Buffer, HdrSend.size(), HdrSend.blockLength(), HdrSend.version(), BUFFELENGTH);

		return KK.code();
	}
}
bool KLineBase::isExist(const char* code)
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
bool KLineBase::isNeedPub()
{
	return NeedPubFlag;
}
void KLineBase::resetPubFlag()
{
	NeedPubFlag = false;
}