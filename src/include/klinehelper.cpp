#include "klinehelper.h"
#ifdef _WIN32
#define SPRINTF sprintf_s
#else
#define SPRINTF sprintf
#endif
/*
*update maketdata from current receive data array
*StartIdx : start index,start+1 price will be dest's open price
*EndIdx   : end index,end-1 price will be dest's close price
*src[]    : source array
*dest     : destination
*/
int KlineFromMaketData(int StartIdx, int EndIdx, SDS_Level2 src[], KLineData& dest) //����1���ӵ�K�����ݣ���ǰһ�������һ�����ݿ�ʼ������һ���ӵĵ�һ�����ݽ���������ʱ��������� ������
{
	strcpy_s(dest.Code, src[EndIdx].Code);
	dest.Date = src[EndIdx].Date;
	dest.Time = src[EndIdx - 1].Time;    //�������ڽ��յ����һ�����ݵ�ʱ��
	dest.TimeStatus = OneMinute;
	dest.PreClose = src[StartIdx].Price;
	dest.Open = src[StartIdx + 1].Price;
	dest.Close = src[EndIdx - 1].Price;

	if (src[StartIdx].Volume == 0)   //��ʼ�����ݵ�ʱ����93000000֮ǰ
	{
		dest.Volume = src[EndIdx - 1].Volume
			+ (src[EndIdx].Volume - src[EndIdx - 1].Volume)*(60000 - (src[EndIdx - 1].Time % 100000)) / (src[EndIdx].Time - src[EndIdx - 1].Time);//�����������һ����������һ�����ݵĲ�ֵ�ڱ����ӵ�ռ��
	}
	else
	{
		dest.Volume =
			(src[StartIdx + 1].Volume - src[StartIdx].Volume)*(src[StartIdx + 1].Time % 100000) / (src[StartIdx + 1].Time - src[StartIdx].Time) //�������ڵ�һ����������һ�����ݵĲ�ֵ�ڱ����ӵ�ռ��
			+ (src[EndIdx - 1].Volume - src[StartIdx + 1].Volume)                                                                           //�����������ݲ�ֵ
			+ (src[EndIdx].Volume - src[EndIdx - 1].Volume)*(60000 - (src[EndIdx - 1].Time % 100000)) / (src[EndIdx].Time - src[EndIdx - 1].Time); //�����������һ����������һ�����ݵĲ�ֵ�ڱ����ӵ�ռ��
	}

	if (src[StartIdx].Turnover == 0)   //��ʼ�����ݵ�ʱ����93000000֮ǰ
	{
		dest.Turnover = src[EndIdx - 1].Turnover 
			+ (src[EndIdx].Turnover - src[EndIdx - 1].Turnover)*(60000 - (src[EndIdx - 1].Time%100000)) / (src[EndIdx].Time - src[EndIdx - 1].Time);//�����������һ����������һ�����ݵĲ�ֵ�ڱ����ӵ�ռ��
	}
	else
	{
		dest.Turnover =
			(src[StartIdx + 1].Turnover - src[StartIdx].Turnover)*(src[StartIdx + 1].Time % 100000) / (src[StartIdx + 1].Time - src[StartIdx].Time) //�������ڵ�һ����������һ�����ݵĲ�ֵ�ڱ����ӵ�ռ��
			+ (src[EndIdx - 1].Turnover - src[StartIdx + 1].Turnover)                                                                           //�����������ݲ�ֵ
			+ (src[EndIdx].Turnover - src[EndIdx - 1].Turnover)*(60000 - (src[EndIdx - 1].Time % 100000)) / (src[EndIdx].Time - src[EndIdx - 1].Time); //�����������һ����������һ�����ݵĲ�ֵ�ڱ����ӵ�ռ��
	}

	//printf("Turnover:%d,%d\n", src[EndIdx].Turnover, src[StartIdx].Turnover);

	dest.High = src[StartIdx + 1].Price;
	dest.Low = src[StartIdx + 1].Price;

	for (int m_Cnt = StartIdx + 2; m_Cnt <= EndIdx - 1; m_Cnt++)
	{
		//printf("%d %d src:%d,low:%d,high:%d\n", m_Cnt, EndIdx, src[m_Cnt].Price, dest.Low, dest.High);
		if (src[m_Cnt].Price > dest.High)
			dest.High = src[m_Cnt].Price;
		if (src[m_Cnt].Price < dest.Low)
			dest.Low = src[m_Cnt].Price;
		//printf("%d %d src:%d,low:%d,high:%d\n", m_Cnt, EndIdx, src[m_Cnt].Price, dest.Low, dest.High);
	}
	return 0;
}

/*
*update maketdata from current receive data array
*StartIdx : start index,its open price will be dest's open price
*EndIdx : end index,its close price will be dest's close price
*src[] : source array
*dest : destination
*/
int KlineFromKline(int StartIdx, int EndIdx, KLineData src[], KLineData& dest)
{
	if (StartIdx < 0)
		StartIdx = 0;
	if (StartIdx >= EndIdx)
	{
		dest = src[StartIdx];
		return 0;
	}
	strcpy_s(dest.Code, src[EndIdx].Code);
	dest.Date = src[EndIdx].Date;
	dest.Time = src[EndIdx].Time;      //��ʱ����ڣ����һ���ӵ��е����һ�����ݵ�ʱ��
	dest.TimeStatus = (TimeStatus)(EndIdx - StartIdx + 1);
	dest.PreClose = src[StartIdx].PreClose;
	dest.Open = src[StartIdx].Open;
	dest.Close = src[EndIdx].Close;

	dest.High = src[StartIdx].High;
	dest.Low = src[StartIdx].Low;
	dest.Turnover = src[StartIdx].Turnover;

	for (int m_Cnt = StartIdx + 1; m_Cnt <= EndIdx; m_Cnt++)
	{
		if (src[m_Cnt].High > dest.High)
			dest.High = src[m_Cnt].High;
		if (src[m_Cnt].Low < dest.Low)
			dest.Low = src[m_Cnt].Low;
		dest.Turnover += src[m_Cnt].Turnover;
	}
	return 0;
}

/*
*trans KLineData format to string in json format
*src : source
*dest : destination
*/
int KLineData2String(KLineData src, std::string& dest)
{
	rapidjson::Document m_Document;
	m_Document.SetObject();
	rapidjson::Document::AllocatorType& m_Allocator = m_Document.GetAllocator();

	rapidjson::Value m_Value_Code;
	m_Value_Code.SetString(src.Code, strlen(src.Code), m_Allocator);
	m_Document.AddMember("Code", m_Value_Code, m_Allocator);

	m_Document.AddMember("Date", src.Date, m_Allocator);
	m_Document.AddMember("Time", src.Date, m_Allocator);
	m_Document.AddMember("TimeStatus", (int)src.TimeStatus, m_Allocator);
	m_Document.AddMember("PreClose", src.PreClose, m_Allocator);
	m_Document.AddMember("Open", src.Open, m_Allocator);
	m_Document.AddMember("Close", src.Close, m_Allocator);
	m_Document.AddMember("High", src.High, m_Allocator);
	m_Document.AddMember("Low", src.Low, m_Allocator);
	m_Document.AddMember("Volume", src.Volume, m_Allocator);
	m_Document.AddMember("Turnover", src.Turnover, m_Allocator);
	dest = Document2String(m_Document);
	return 0;
}


int Parase2Object(std::string json_data, KLineData &dest){

	rapidjson::Document documentprase;
	documentprase.Parse<0>(json_data.c_str());
	const char * IntParams[] = { "Date", "Time", "PreClose", "Open", "Close", "High", "Low" };//8,    2014/12/29,  , "TDFTime", "Sn" , "Price", "NumTrades", "AvgBidPrice", "AvgAskPrice", "HighLimited", "LowLimited"
	const char * Int64Params[] = { "Volume", "Turnover" };//2, "TotalBidVol", "TotalAskVol"

	if (documentprase.HasParseError())  //������������ʱ������ʾ  
	{
		printf("GetParseError %s\n", documentprase.GetParseError());
		return -1;//����ʧ��
	}
	else if (documentprase.IsObject())
	{
		for (int i = 0; i <sizeof(IntParams) / sizeof(IntParams[0]); i++)
		{
			if (!documentprase.HasMember(IntParams[i]) || !documentprase[IntParams[i]].IsInt())
			{
				printf("lose:%s\n", IntParams[i]);
				return -1;
			}
		}
		if (!documentprase.HasMember("Code") || !documentprase["Code"].IsString())
		{
			printf("lose:code\n");
			return -1;
		}
		if (!documentprase.HasMember("TimeStatus") || !documentprase["Code"].IsInt())
		{
			printf("lose:code\n");
			return -1;
		}
		for (int i = 0; i <sizeof(Int64Params) / sizeof(Int64Params[0]); i++)
		{
			if (!documentprase.HasMember(Int64Params[i]) || !documentprase[Int64Params[i]].IsString())
			{
				printf("lose:%s\n", Int64Params[i]);
				return -1;
			}
		}
		dest.Date = documentprase["Date"].GetInt();
		dest.Time = documentprase["Time"].GetInt();
		dest.PreClose = documentprase["PreClose"].GetInt();
		dest.Open = documentprase["Open"].GetInt();//�������  
		dest.High = documentprase["High"].GetInt();
		dest.Low = documentprase["Low"].GetInt();
		dest.Close = documentprase["Close"].GetInt();
		dest.Volume = atoll(documentprase["Volume"].GetString());
		dest.Turnover = atoll(documentprase["Turnover"].GetString());
		
	}
	return 0;

}