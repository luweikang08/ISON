#include "KLineBase.h"

using namespace ison::indicator;

int getmm(int time) //get minute
{
	return ((time / 100000) % 100);
}

int getmm(std::string sbe_kline)
{
	char recvBuf[BUFFELENGTH];
	memcpy(recvBuf, sbe_kline.c_str(), sbe_kline.size());
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
	char recvBuf[BUFFELENGTH];
	memcpy(recvBuf, sbe_trans.c_str(), sbe_trans.size());
	TOPICHEAD  * m_topichead_rec;
	m_topichead_rec = (TOPICHEAD*)recvBuf;

	baseline::SDS_Transaction  CC;
	baseline::MessageHeader hdr;
	hdr.wrap(recvBuf, 0, MESSAGEHEADERVERSION, BUFFELENGTH);//parse messageheader
	CC.wrapForDecode(recvBuf, hdr.size(), hdr.blockLength(), hdr.version(), BUFFELENGTH);
	return gethhmm(CC.time());
}

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


KLineBase::KLineBase()
{
}

KLineBase::~KLineBase()
{
}
KL_STORE_RetCode KLineBase::Store(std::string tran_src)
{
	char srcBuf[BUFFELENGTH];
	memcpy(srcBuf, tran_src.c_str(), tran_src.size());

	baseline::MessageHeader hdr_src;
	baseline::SDS_Transaction TT_src;

	hdr_src.wrap(srcBuf + TOPICHEADSIZE, 0, MESSAGEHEADERVERSION, BUFFELENGTH);//parse messageheader
	TT_src.wrapForDecode(srcBuf, TOPICHEADSIZE + hdr_src.size(), hdr_src.blockLength(), hdr_src.version(), BUFFELENGTH);
	LastTime = TT_src.time();
	Sn = TT_src.sn();

	if ((TT_src.time() < 93000000) || ((TT_src.time() > 113000000) && (TT_src.time() < 130000000)) || (TT_src.time() > 150100000))
	{
		return KL_FAIL;
	}
	std::map < std::string, std::string>::iterator it = DataMap.find(TT_src.code());
	if (it == DataMap.end())
	{
		char StoreBuf[BUFFELENGTH];
		baseline::MessageHeader hdr_Store;
		baseline::SDS_Kline KK_Store;
		hdr_Store.wrap(StoreBuf, 0, MESSAGEHEADERVERSION, BUFFELENGTH)
			.blockLength(baseline::SDS_Kline::sbeBlockLength())
			.templateId(baseline::SDS_Kline::sbeTemplateId())
			.schemaId(baseline::SDS_Kline::sbeSchemaId())
			.version(baseline::SDS_Kline::sbeSchemaVersion());
		KK_Store.wrapForEncode(StoreBuf, hdr_Store.size(), BUFFELENGTH)
			.putCode(TT_src.code())
			.date(TT_src.date())
			.time(TT_src.time())
			.timeStatus(OneMinute)
			.preClose(TT_src.price())
			.open(TT_src.price())
			.close(TT_src.price())
			.high(TT_src.price())
			.low(TT_src.price())
			.volume(TT_src.volume())
			.turnover(TT_src.turnover()); KK_Store
			.avgPrice((TT_src.volume() == 0 ? 0 : TT_src.turnover() / TT_src.volume()))
			.trndNum(1);
		std::string m_str_StoreMess(StoreBuf, hdr_Store.size() + KK_Store.sbeBlockLength());
		DataMap.insert(std::pair<std::string, std::string>(TT_src.code(), m_str_StoreMess));
		return KL_INSERT;
	}
	else
	{
		baseline::MessageHeader hdr_read;
		baseline::SDS_Kline KK_read;

		char readBuf[BUFFELENGTH];
		memcpy(readBuf, it->second.c_str(), hdr_read.size() + KK_read.sbeBlockLength());

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
			unsigned long long temp_vol,temp_turn;
			temp_vol = KK_read.volume();
			temp_vol += TT_src.volume();
			KK_read.volume(temp_vol);
			temp_turn = KK_read.turnover();
			temp_turn += TT_src.turnover();
			KK_read.turnover(temp_turn);
			KK_read.avgPrice(temp_vol == 0 ? 0 : temp_turn / temp_vol);
			unsigned int temp_num;
			temp_num = KK_read.trndNum();
			KK_read.trndNum(temp_num + 1);
			std::string m_str_Store(readBuf, hdr_read.size() + KK_read.sbeBlockLength());
			it->second = m_str_Store;
			return KL_UPDATE;
		}
		else //the data received is in the next minute,publish is required
		{
			SendStr.clear();
			SendStr.assign(readBuf, hdr_read.size() + KK_read.sbeBlockLength());
			NeedPubFlag = true;

			char StoreBuf[BUFFELENGTH];
			baseline::MessageHeader hdr_Store;
			baseline::SDS_Kline KK_Store;
			hdr_Store.wrap(StoreBuf, 0, MESSAGEHEADERVERSION, BUFFELENGTH)
				.blockLength(baseline::SDS_Kline::sbeBlockLength())
				.templateId(baseline::SDS_Kline::sbeTemplateId())
				.schemaId(baseline::SDS_Kline::sbeSchemaId())
				.version(baseline::SDS_Kline::sbeSchemaVersion());
			KK_Store.wrapForEncode(StoreBuf, hdr_Store.size(), BUFFELENGTH)
				.putCode(TT_src.code())
				.date(TT_src.date())
				.time(TT_src.time())
				.timeStatus(OneMinute)
				.preClose(TT_src.price())
				.open(TT_src.price())
				.close(TT_src.price())
				.high(TT_src.price())
				.low(TT_src.price())
				.volume(TT_src.volume())
				.turnover(TT_src.turnover())
				.avgPrice((TT_src.volume() == 0 ? 0 : TT_src.turnover() / TT_src.volume()))
				.trndNum(1);
			std::string m_str_StoreMess(StoreBuf, hdr_Store.size() + KK_Store.sbeBlockLength());
			it->second = m_str_StoreMess;
			return KL_REPLACE;
		}
	}
}
std::string KLineBase::GetSendStr()
{
	return SendStr;
}

#ifdef NOTOPICHEAD
std::string KLineBase::MakeSendStr()
{
	return SendStr;
}
#else
std::string KLineBase::MakeSendStr(int topic, int sn)
{
	char SendBuf[BUFFELENGTH];
	char Code[16];
	TOPICHEAD TopicHeadSend;
	baseline::MessageHeader HdrSend;
	baseline::SDS_Kline KK;

	memcpy(SendBuf, SendStr.c_str(), SendStr.size());
	HdrSend.wrap(SendBuf, 0, MESSAGEHEADERVERSION, BUFFELENGTH);
	KK.wrapForDecode(SendBuf, HdrSend.size(), HdrSend.blockLength(), HdrSend.version(), BUFFELENGTH);

	TopicHeadSend.topic = topic;
	DateAndTime m_dtm = GetDateAndTime();
	TopicHeadSend.ms = (m_dtm.time % 1000);
	memcpy(Code, KK.code(), 16);
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
#endif

const std::string KLineBase::GetDataCode()const
{
	if (SendStr.size() > 0)
	{
		char Buffer[BUFFELENGTH];
		baseline::MessageHeader HdrSend;
		baseline::SDS_Kline KK;

		memcpy(Buffer, SendStr.c_str(), SendStr.size());
		HdrSend.wrap(Buffer, 0, MESSAGEHEADERVERSION, BUFFELENGTH);
		KK.wrapForDecode(Buffer, HdrSend.size(), HdrSend.blockLength(), HdrSend.version(), BUFFELENGTH);

		std::string m_str_temp(KK.code(),16);
		return m_str_temp;
	}
	return NULL;
}

bool KLineBase::IsExist(const char* code)
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
bool KLineBase::IsNeedPub()
{
	if (NeedPubFlag)
	{
		NeedPubFlag = false; //reset NeedPubFlag
		return true;
	}
	else
	{
		return false;
	}
}
void KLineBase::ResetPubFlag()
{
	NeedPubFlag = false;
}

int KLineBase::GetDataMapSize()const
{
	return DataMap.size();
}
void KLineBase::InitIt()
{
	if (DataMap.size()>0)
		Itera = DataMap.begin();
}
void KLineBase::IncreIt()
{
	if (Itera!=DataMap.end())
	{
		Itera++;
	}
}

bool KLineBase::ChkItNeedPub(bool playbackmode)
{
	char readBuf[BUFFELENGTH];
	std::string RetStr;
	memcpy(readBuf, Itera->second.c_str(), Itera->second.size());

	baseline::MessageHeader hdr_Read;
	baseline::SDS_Kline KK_Read;
	hdr_Read.wrap(readBuf, 0, MESSAGEHEADERVERSION, BUFFELENGTH);
	KK_Read.wrapForDecode(readBuf, hdr_Read.size(), hdr_Read.blockLength(), hdr_Read.version(), BUFFELENGTH);
	
	DateAndTime m_DateAndTime_Now;
	if (playbackmode)
	{
		m_DateAndTime_Now.date = KK_Read.date();
		m_DateAndTime_Now.time = LastTime;
	}
	else
	{
		m_DateAndTime_Now = GetDateAndTime();
	}
	DateAndTime m_DateAndTime_Read;
	m_DateAndTime_Read.date = KK_Read.date();
	m_DateAndTime_Read.time = KK_Read.time();

	if (GetTimeDiffInSec(m_DateAndTime_Read, m_DateAndTime_Now) > 60) //last record is stored more than one minute ago from now,must pub and update it
	{
		SendStr.clear();
		SendStr.assign(readBuf, hdr_Read.size() + KK_Read.sbeBlockLength());

		char storeBuf[BUFFELENGTH];
		baseline::SDS_Kline KK_store;
		memcpy(storeBuf, readBuf, hdr_Read.size());
		KK_store.wrapForEncode(storeBuf, hdr_Read.size(), hdr_Read.size() + KK_store.sbeBlockLength());
		KK_store.putCode(KK_Read.code());
		KK_store.date(KK_Read.date());
		//time increase one minute
		//KK_store.time((KK_Read.time() / 100000) % 100 >= 59 ? (KK_Read.time() / 10000000 + 1) * 10000000 + KK_Read.time() % 100000 : (KK_Read.time() / 10000000) * 10000000 + ((KK_Read.time() / 100000) % 100 + 1) * 100000 + KK_Read.time() % 100000);
		if (KK_Read.time() > 235999999)
		{
			KK_store.time(0);
		}
		else
		{
			KK_store.time((KK_Read.time() / 100000) % 100 >= 59 ? (KK_Read.time() / 10000000 + 1) * 10000000 + KK_Read.time() % 100000 : (KK_Read.time() + 100000));
		}
		KK_store.timeStatus(KK_Read.timeStatus());
		KK_store.preClose(KK_Read.close());
		KK_store.open(KK_Read.close());
		KK_store.close(KK_Read.close());
		KK_store.high(KK_Read.close());
		KK_store.low(KK_Read.close());
		KK_store.volume(0);
		KK_store.turnover(0);
		KK_store.avgPrice(0);
		KK_store.trndNum(0);
		std::string storeStr(storeBuf, hdr_Read.size() + KK_Read.sbeBlockLength());
		DataMap[Itera->first] = storeStr;
		if ((KK_Read.time() < 93000000) || ((KK_Read.time() > 113100000) && (KK_Read.time() < 130000000)) || (KK_Read.time() > 150200000)) //do not pub info beyond market time
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	return false;
}

const char* KLineBase::GetCodeInIt()const
{
	char readBuf[BUFFELENGTH];
	memcpy(readBuf, Itera->second.c_str(), Itera->second.size());

	baseline::MessageHeader hdr_Read;
	baseline::SDS_Kline KK_Read;
	hdr_Read.wrap(readBuf, 0, MESSAGEHEADERVERSION, BUFFELENGTH);
	KK_Read.wrapForDecode(readBuf, hdr_Read.size(), hdr_Read.blockLength(), hdr_Read.version(), BUFFELENGTH);

	return KK_Read.code();
}
const int KLineBase::GetSn()const
{
	return Sn;
}