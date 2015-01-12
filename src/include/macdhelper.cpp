#include "macdhelper.h"

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
			dest[i-offset] = temp;
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
int CalcMacdResult(std::queue<double>& src, int& MACD, int& MACDSignal, int& MACDHist)
{
	TA_Real    m_num_InReal[50];
	TA_Integer m_num_OutBeg;
	TA_Integer m_num_OutNbElement;
	TA_Real    m_num_OutMACD[50];
	TA_Real    m_num_OutMACDSignal[50];
	TA_Real    m_num_OutMACDHist[50];
	if (Queue2Array(src, m_num_InReal, 0, 50) == 0) //transform success
	{
		if (TA_MACD(0, 49, &m_num_InReal[0], 12, 26, 9, &m_num_OutBeg, &m_num_OutNbElement, &m_num_OutMACD[0], &m_num_OutMACDSignal[0], &m_num_OutMACDHist[0]) == TA_SUCCESS)
		{
			MACD = m_num_OutMACD[16];
			MACDSignal = m_num_OutMACDSignal[16];
			MACDHist = m_num_OutMACDHist[16];
			return 0;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
}

/*
*transform MacdData format to string in json format
*src : source
*dest : destination
*/
int MacdData2String(MacdData src, std::string& dest)
{
	rapidjson::Document m_Document;
	m_Document.SetObject();
	rapidjson::Document::AllocatorType& m_Allocator = m_Document.GetAllocator();

	rapidjson::Value m_Value_Code;
	m_Value_Code.SetString(src.Code, strlen(src.Code), m_Allocator);
	m_Document.AddMember("Code", m_Value_Code, m_Allocator);

	m_Document.AddMember("Date", src.Date, m_Allocator);
	m_Document.AddMember("Time", src.Time, m_Allocator);
	m_Document.AddMember("MACD", src.MACD, m_Allocator);
	m_Document.AddMember("MACDSignal", src.MACDSignal, m_Allocator);
	m_Document.AddMember("MACDHist", src.MACDHist, m_Allocator);
	dest = Document2String(m_Document);
	return 0;
}