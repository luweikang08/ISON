#include "json_analysis.h"

int ParseJsonToMaketDataLevel2(std::string Signal, SDS_Level2& dest)
{
	rapidjson::Document m_Document;
	m_Document.Parse<0>(Signal.c_str());
	const char * IntParams[] = { "Seqno", "Date", "Time", "PreClose", "Open", "High", "Low", "Price", "NumTrades", "AvgBidPrice", "AvgAskPrice", "HighLimited", "LowLimited", "TDFTime", "Sn" };//8
	const char * Int64Params[] = { "Volume", "Turnover", "TotalBidVol", "TotalAskVol" };//2
	const char * ArrayParams[] = { "AskPrice", "AskVol", "BidPrice", "BidVol" };//4*10
	if (m_Document.HasParseError())
	{
		printf("GetParseError %s\n", m_Document.GetParseError());
		return 0;
	}
	else if (m_Document.IsObject())
	{
		for (int i = 0; i < sizeof(IntParams) / sizeof(IntParams[0]); i++)
		{
			if (!m_Document.HasMember(IntParams[i]) || !m_Document[IntParams[i]].IsInt())
				return 0;
		}
		if (!m_Document.HasMember("Code") || !m_Document["Code"].IsString())
			return false;
		for (int i = 0; i < sizeof(Int64Params) / sizeof(Int64Params[0]); i++)
		{
			if (!m_Document.HasMember(Int64Params[i]) || !m_Document[Int64Params[i]].IsString())
				return 0;
		}
		for (int i = 0; i < sizeof(ArrayParams) / sizeof(ArrayParams[0]); i++)
		{
			if (!m_Document.HasMember(ArrayParams[i]) || !m_Document[ArrayParams[i]].IsArray())
				return 0;
		}

		dest.Seqno = m_Document["Seqno"].GetInt();
		dest.Date = m_Document["Date"].GetInt();
		dest.Time = m_Document["Time"].GetInt();
		dest.PreClose = m_Document["PreClose"].GetInt();
		dest.Open = m_Document["Open"].GetInt();//在这出错  
		dest.High = m_Document["High"].GetInt();
		dest.Low = m_Document["Low"].GetInt();
		dest.Price = m_Document["Price"].GetInt();
		dest.Volume = atoll(m_Document["Volume"].GetString());
		dest.Turnover = atoll(m_Document["Turnover"].GetString());
		dest.TotalBidVol = atoll(m_Document["TotalBidVol"].GetString());
		dest.TotalAskVol = atoll(m_Document["TotalAskVol"].GetString());
		dest.NumTrades = m_Document["NumTrades"].GetInt();
		dest.AvgBidPrice = m_Document["AvgBidPrice"].GetInt();
		dest.AvgAskPrice = m_Document["AvgAskPrice"].GetInt();
		dest.LowLimited = m_Document["LowLimited"].GetInt();
		dest.HighLimited = m_Document["HighLimited"].GetInt();
		const char * Code = m_Document["Code"].GetString();
		strcpy_s(dest.Code, Code);
		std::vector<N4> vhq;

		for (size_t i = 0; i <sizeof(ArrayParams) / sizeof(ArrayParams[0]); ++i) {
			const rapidjson::Value &temparray = m_Document[ArrayParams[i]];
			for (rapidjson::SizeType j = 0; j < temparray.Size(); ++j)
				vhq.push_back(temparray[j].GetInt());
		}
		for (int i = 0; i < 10; i++)
		{
			dest.AskPrice[i] = vhq[i];
			dest.AskVol[i] = vhq[10 + i];
			dest.BidPrice[i] = vhq[2 * 10 + i];
			dest.BidVol[i] = vhq[3 * 10 + i];
		}
		return 1;
	}
	return 0;
}

//
//bool ParseJsonWithCode(string Code, const string Signal, MaketDataLevel2& m_uMaketData)
//{
//	Document m_Document;
//	string tmp;
//	m_Document.Parse<0>(Signal.c_str());
//	if (m_Document.HasParseError())
//	{
//		printf("GetParseError %s\n", m_Document.GetParseError());
//		return false;
//	}
//	if (m_Document["Code"].GetString() == Code) //判断股票代码是否正确
//	{
//		//strcpy(m_uMaketData.Detail.Code, root["Code"].asString());
//		//root["Code"].asString().copy(m_uMaketData.Code, 16, 0);
//		//Code.copy(m_uMaketData.Code, 16, 0);
//		//sprintf_s(m_uMaketData.Code, "%s", Code);
//		strcpy_s(m_uMaketData.Code, Code.c_str());
//		m_uMaketData.Date = m_Document["Date"].GetInt();
//		m_uMaketData.Time = m_Document["Time"].GetInt();
//		m_uMaketData.Price = m_Document["Price"].GetInt();
//		tmp = m_Document["Volume"].GetString();
//		m_uMaketData.Volume = atoll(tmp.c_str());
//		tmp = m_Document["Turnover"].GetString();
//		m_uMaketData.Turnover = atoll(tmp.c_str());
//		return true;
//	}
//	return false;
//}
//
//bool ParseJsonWithHead(string head, const string Signal, MaketDataLevel2& m_uMaketData)
//{
//	Document m_Document, m_m_Document;
//	m_Document.Parse<0>(Signal.c_str());
//	if (m_Document.HasParseError())
//	{
//		printf("GetParseError %s\n", m_Document.GetParseError());
//		return false;
//	}
//	if (m_Document["head"].GetString() == head)
//	{
//		if (m_Document.HasMember("body"))//这个时候要保证m_Document是IsObject类型 最好是 if(d.Isobject() && d.HasMember("a"))  
//		{
//			rapidjson::Value &body = m_Document["body"];
//			if (body.IsObject())
//			{
//				/*sdstime = body["sdstime"].GetInt();
//				atradeintime = body["atradeintime"].GetInt();*/
//				strcpy_s(m_uMaketData.Code, body["stockid"].GetString());
//				//m_uMaketData.Date = body["Date"].GetInt();
//				m_uMaketData.Time = body["time"].GetInt();
//				m_uMaketData.Price = body["lastprice"].GetInt();
//				m_uMaketData.Volume = body["Volume"].GetInt();
//				m_uMaketData.Turnover = body["Turnover"].GetInt();
//				return true;
//			}
//		}
//	}
//	return false;
//}