#include "rsihelper.h"

/*
*transform queue to array,used for calculate MACD
*src : source queue
*dest : destination array
*offset : start index at queue
*length ; transform length in queue,start from offset
*/
int Queue2Array(std::queue<double>& src, double dest[], int offset, int length)
{
	double temp = 0;
	if (src.empty())
		return -1;
	for (int i = 0; i < src.size(); i++)
	{
		temp = src.front();
		src.pop();
		if ((i >= offset) && (i < (length + offset)))
		{
			dest[i - offset] = temp;
			//printf("array %d:%d\n", i - offset, dest[i - offset]);
		}
		src.push(temp);
	}
	return 0;
}

/*
*calculate MACD of queue,queue's length is 50,FastPeriod is 12,SlowPeirod is 26,SignalPeriod is 9,return the last valid data in outMACD[],outMACDSignal[],outMACDHist[]
*src : source queue
*MACD : return value,last valid data in outMACD[]
*MACDSignal : return value,last valid data in outMACDSignal[]
*MACDHist : return value,last valid data in outMACDHist[]
*/
int CalcRsiResult(std::queue<double>& src, int& RSI6, int& RSI12, int& RSI24)
{
	TA_Real    m_num_InReal[24];
	TA_Integer m_num_OutBeg;
	TA_Integer m_num_OutNbElement;
	TA_Real    m_num_OutReal[24];

	if (Queue2Array(src, m_num_InReal, 0, 24) == 0) //transform success
	{
		if (TA_RSI(0, 23, &m_num_InReal[0], 6, &m_num_OutBeg, &m_num_OutNbElement, &m_num_OutReal[0]) == TA_SUCCESS)
		{
			RSI6 = m_num_OutReal[17];
		}
		if (TA_RSI(0, 23, &m_num_InReal[0], 12, &m_num_OutBeg, &m_num_OutNbElement, &m_num_OutReal[0]) == TA_SUCCESS)
		{
			RSI12 = m_num_OutReal[11];
		}
		if (TA_RSI(0, 23, &m_num_InReal[0], 24, &m_num_OutBeg, &m_num_OutNbElement, &m_num_OutReal[0]) == TA_SUCCESS)
		{
			RSI24 = m_num_OutReal[0];
			return 0;
		}
	}
	return -1;
}

/*
*transform MacdData format to string in json format
*src : sourceS
*dest : destination
*/
int RsiData2String(RsiData src, std::string& dest)
{
	rapidjson::Document m_Document;
	m_Document.SetObject();
	rapidjson::Document::AllocatorType& m_Allocator = m_Document.GetAllocator();

	rapidjson::Value m_Value_Code;
	m_Value_Code.SetString(src.Code, strlen(src.Code), m_Allocator);
	m_Document.AddMember("Code", m_Value_Code, m_Allocator);

	m_Document.AddMember("Date", src.Date, m_Allocator);
	m_Document.AddMember("Time", src.Time, m_Allocator);
	m_Document.AddMember("RSI6", src.Rsi6, m_Allocator);
	m_Document.AddMember("RSI12", src.Rsi12, m_Allocator);
	m_Document.AddMember("RSI24", src.Rsi24, m_Allocator);
	dest = Document2String(m_Document);
	return 0;
}