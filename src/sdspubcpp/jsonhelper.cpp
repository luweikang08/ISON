#ifdef _WIN32
#define SPRINTF sprintf_s
#else
#define SPRINTF sprintf
#endif
#include <iostream> 
#include<fstream>
#include"iblog.h"
#include<vector>
#include<string>
#include<map>

#include"ibsdstool.h"
#include "document.h"
#include "isonsdsdataapi.h"
#include "jsonhelper.h"
#include"isonutil.h"

//Read the config file
int ReadConfig(const char* filename, std::vector <std::string> configkey,std::map<std::string,std::string> &configmap){
	std::ifstream is;
	is.open(filename);
	if (is){
		std::cout << "Read config file "<<filename<< " sucessfully!" << std::endl;
		std::string str((std::istreambuf_iterator<char>(is)),
			std::istreambuf_iterator<char>());
		rapidjson::Document documentprase;
		const char* ssms;
		ssms = str.c_str();
		documentprase.Parse<0>(ssms);
		if (documentprase.HasParseError())
		{
			return -1;
			printf("ParseError error:%s\n", documentprase.GetParseError());
		}
		else if (documentprase.IsObject())
		{  
			for (auto key:configkey)
			{ 
				//if (!documentprase.HasMember(key.c_str()) || documentprase[key.c_str()].IsString()) 
					//std::cout << "Parase config file fail :" << key<<"is not string or exist!"<<std::endl;
				if (!documentprase.HasMember(key.c_str()))
				{
					std::cout << "Parase config file fail :" << key << " is not  exist!" << std::endl;
					return -1;
					
				}
				if (documentprase[key.c_str()].IsString())
					configmap[key] = documentprase[key.c_str()].GetString();
				else if (documentprase[key.c_str()].IsInt())
				{  //转化成string
					int temp = documentprase[key.c_str()].GetInt();
					configmap[key]= itostring(temp);//要转化回去的话用atoi
				}
				else {
					return -1; 
					std::cout << "Parase config file fail :" << key << " type is not  string or int!" << std::endl;
				}
			}
		}
	}
	else { return -1; std::cout << "Read config file fail,so  use the default params!" << std::endl; }
	is.close();
	return 0;
}

int parase( std::string message, SubscribeMarketDataReq &maket_data_req){
	const char* json_field[7] = { "Seqno", "DateType", "BeginDate", "BeginTime", "EndDate", "EndTime", "CodeTable" };
	rapidjson::Document documentprase;
	documentprase.Parse<0>(message.c_str());
	//if (!documentprase.HasMember("Seqno") || !documentprase["Seqno"].IsInt()
	//|| !documentprase["Seqno"].GetInt() != SubscribeMarketDataResp)
	if (documentprase.HasParseError())  //解析发生错误时错误提示  
	{
		printf("GetParseError %s\n", documentprase.GetParseError());
		return -1;//解析失败
	}
	else if (documentprase.IsObject()) //把level2转化成ibserver需要的格式
	{
		if (!documentprase.HasMember("Seqno") || !documentprase["Seqno"].IsInt())
			return -1;
		for (int i = 1; i < 6; i++){
			if (!documentprase.HasMember(json_field[i]) ||! documentprase[json_field[i]].IsInt())
				return -1;
		}
		
		if (!documentprase.HasMember("CodeTable") || !documentprase["CodeTable"].IsArray())
			return -1;

		maket_data_req.seqno = documentprase["Seqno"].GetInt();
		maket_data_req.date_type = documentprase["DateType"].GetInt();
		maket_data_req.begin_date = documentprase["BeginDate"].GetInt();
		maket_data_req.begin_time = documentprase["BeginTime"].GetInt();
		maket_data_req.end_date = documentprase["EndDate"].GetInt();
		maket_data_req.end_time = documentprase["EndTime"].GetInt();
		
		rapidjson::Value& codetable_array = documentprase["CodeTable"];
		for (rapidjson::SizeType i = 0; i < codetable_array.Size(); ++i) //如果不是数组，这一行会报错  
			maket_data_req.code_table.push_back(codetable_array[i].GetString());
	}
	return 0;
}

int Parase2Object(std::string json_data, SDS_Level2 &sds_level2){

	rapidjson::Document documentprase;
	documentprase.Parse<0>(json_data.c_str());
	const char * IntParams[] = { "Seqno", "Date", "Time", "PreClose", "Open", "High", "Low", "Price", "NumTrades", "AvgBidPrice", "AvgAskPrice", "HighLimited", "LowLimited" };//8,    2014/12/29,  , "TDFTime", "Sn" 
	const char * Int64Params[] = { "Volume", "Turnover","TotalBidVol", "TotalAskVol" };//2
	const char * ArrayParams[] = { "AskPrice","AskVol","BidPrice","BidVol"};//4*10

	if (documentprase.HasParseError())  //解析发生错误时错误提示  
	{
		printf("GetParseError %s\n", documentprase.GetParseError());
		return -1;//解析失败
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
		for (int i = 0; i <sizeof(Int64Params) / sizeof(Int64Params[0]);i++)
		{
			if (!documentprase.HasMember(Int64Params[i]) || !documentprase[Int64Params[i]].IsString())
			{
				printf("lose:%s\n", Int64Params[i]);
				return -1;
			}
		}
		for (int i = 0; i < sizeof(ArrayParams) / sizeof(ArrayParams[0]); i++)
		{
			if (!documentprase.HasMember(ArrayParams[i]) || !documentprase[ArrayParams[i]].IsArray())
			{
				printf("lose:%s\n", ArrayParams[i]);
				return -1;
			}
		}
		sds_level2.Seqno =documentprase["Seqno"].GetInt();
		sds_level2.Date = documentprase["Date"].GetInt();
		sds_level2.Time = documentprase["Time"].GetInt();
		sds_level2.PreClose =documentprase["PreClose"].GetInt();
		sds_level2.Open = documentprase["Open"].GetInt();//在这出错  
		sds_level2.High = documentprase["High"].GetInt();
		sds_level2.Low = documentprase["Low"].GetInt();
		sds_level2.Price = documentprase["Price"].GetInt();
		sds_level2.Volume = atoll(documentprase["Volume"].GetString());
		sds_level2.Turnover = atoll(documentprase["Turnover"].GetString());
		sds_level2.TotalBidVol = atoll(documentprase["TotalBidVol"].GetString());
		sds_level2.TotalAskVol = atoll(documentprase["TotalAskVol"].GetString());
		sds_level2.NumTrades = documentprase["NumTrades"].GetInt();
		sds_level2.AvgBidPrice = documentprase["AvgBidPrice"].GetInt();
		sds_level2.AvgAskPrice = documentprase["AvgAskPrice"].GetInt();
		sds_level2.LowLimited = documentprase["LowLimited"].GetInt();
		sds_level2.HighLimited = documentprase["HighLimited"].GetInt();
		//sds_level2.Volume = documentprase["Volume"].GetInt64();
	//	sds_level2.Turnover = documentprase["Turnover"].GetInt64();
	    const char * code = documentprase["Code"].GetString();
		memset(sds_level2.Code, 0, sizeof(sds_level2.Code));
		SPRINTF(sds_level2.Code, "%s", code);
		std::vector<N4> vhq;
		
		for (size_t i = 0; i <sizeof(ArrayParams) / sizeof(ArrayParams[0]); ++i) {
			const rapidjson::Value &temparray = documentprase[ArrayParams[i]];
			for (rapidjson::SizeType j = 0; j < temparray.Size(); ++j)
					vhq.push_back(temparray[j].GetInt());
		}
		for(int i = 0; i < 10;i++ )
		{
			sds_level2.AskPrice[i] = vhq[i];
			sds_level2.AskVol[i] = vhq[10+i];
			sds_level2.BidPrice[i] = vhq[2*10+i];
			sds_level2.BidVol[i] = vhq[3*10+i];
		}/*
			const rapidjson::Value &temparray1 = documentprase[ArrayParams[0]];
			for (rapidjson::SizeType j = 0; j < temparray1.Size(); ++j)
				sds_level2.AskPrice[j] = temparray1[j].GetInt();

			const rapidjson::Value &temparray2 = documentprase[ArrayParams[1]];
			for (rapidjson::SizeType j = 0; j < temparray2.Size(); ++j)
				sds_level2.AskVol[j] = temparray2[j].GetInt();

			const rapidjson::Value &temparray3 = documentprase[ArrayParams[2]];
			for (rapidjson::SizeType j = 0; j < temparray3.Size(); ++j)
				sds_level2.BidPrice[j] = temparray3[j].GetInt();

			const rapidjson::Value &temparray4 = documentprase[ArrayParams[3]];
			for (rapidjson::SizeType j = 0; j < temparray4.Size(); ++j)
				sds_level2.BidVol[j] = temparray4[j].GetInt();*/
		//12.23
		sds_level2.SDSTime = documentprase["TDFTime"].GetInt();
		sds_level2.Sn = documentprase["Sn"].GetInt();

		
	}
		

	return 0;

}
int Parase2Object(std::string json_data, SDS_Transaction &sds_transaction){

	rapidjson::Document documentprase;
	documentprase.Parse<0>(json_data.c_str());
	const char * IntParams[] = { "Seqno", "Date", "Time", "Index", "Price", "Volume", "Turnover", "FunctionCode",
		"AskOrder", "BidOrder" ,"TDFTime", "Sn" };//10
	const char * CharParams[] = { "BSFlag", "OrderKind" };//2
	if (documentprase.HasParseError())  //解析发生错误时错误提示  
	{
		printf("GetParseError %s\n", documentprase.GetParseError());
		return -1;//解析失败
	}
	else if (documentprase.IsObject())
	{
		for (int i = 0; i <sizeof(IntParams) / sizeof(IntParams[0]); i++)
		{
			if (!documentprase.HasMember(IntParams[i]) || !documentprase[IntParams[i]].IsInt())
				return -1;
		}
		if (!documentprase.HasMember("Code") || !documentprase["Code"].IsString())
			return -1;
		for (int i = 0; i < sizeof(CharParams) / sizeof(CharParams[0]); i++)
		{
			if (!documentprase.HasMember(CharParams[i]) || !documentprase[CharParams[i]].IsInt())
				return -1;
		}
		sds_transaction.Seqno = documentprase["Seqno"].GetInt();

		const char * code = documentprase["Code"].GetString();
		memset(sds_transaction.Code, 0, sizeof(sds_transaction.Code));
		SPRINTF(sds_transaction.Code, "%s", code);

		sds_transaction.Date = documentprase["Date"].GetInt();
		sds_transaction.Time = documentprase["Time"].GetInt();
		sds_transaction.Index = documentprase["Index"].GetInt();
		sds_transaction.Price = documentprase["Price"].GetInt();
		sds_transaction.Volume = documentprase["Volume"].GetInt();
		sds_transaction.Turnover = documentprase["Turnover"].GetInt();
		sds_transaction.FunctionCode = documentprase["FunctionCode"].GetInt();
		sds_transaction.AskOrder = documentprase["AskOrder"].GetInt();
		sds_transaction.BidOrder = documentprase["BidOrder"].GetInt();
		sds_transaction.BSFlag = documentprase["BSFlag"].GetInt();
		sds_transaction.OrderKind =documentprase["OrderKind"].GetInt();
		//12.23
		sds_transaction.SDSTime = documentprase["TDFTime"].GetInt();
		sds_transaction.Sn = documentprase["Sn"].GetInt();



	}


	return 0;

}
int Parase2Object(std::string json_data, SDS_Order &sds_order){

	rapidjson::Document documentprase;
	documentprase.Parse<0>(json_data.c_str());
	const char * IntParams[] = { "Seqno", "Date", "Time", "Order", "Price", "Volume","TDFTime", "Sn" };//6
	const char * CharParams[] = { "FunctionCode", "OrderKind" };//2
	if (documentprase.HasParseError())  //解析发生错误时错误提示  
	{
		printf("GetParseError %s\n", documentprase.GetParseError());
		return -1;//解析失败
	}
	else if (documentprase.IsObject())
	{
		for (int i = 0; i <sizeof(IntParams) / sizeof(IntParams[0]); i++)
		{
			if (!documentprase.HasMember(IntParams[i]) || !documentprase[IntParams[i]].IsInt())
				return -1;
		}
		if (!documentprase.HasMember("Code") || !documentprase["Code"].IsInt())
			return -1;
		for (int i = 0; i < sizeof(CharParams) / sizeof(CharParams[0]); i++)
		{
			if (!documentprase.HasMember(CharParams[i]) || !documentprase[CharParams[i]].IsInt())
				return -1;
		}

		sds_order.Seqno = documentprase["Seqno"].GetInt();
		const char * code = documentprase["Code"].GetString();
		memset(sds_order.Code, 0, sizeof(sds_order.Code));
		SPRINTF(sds_order.Code, "%s", code);
		sds_order.Date = documentprase["Date"].GetInt();
		sds_order.Time = documentprase["Time"].GetInt();
		sds_order.Order = documentprase["Order"].GetInt();
		sds_order.Price = documentprase["Price"].GetInt();
		sds_order.Volume = documentprase["Volume"].GetInt();
		
		sds_order.FunctionCode = documentprase["FunctionCode"].GetInt();
		sds_order.OrderKind = documentprase["AskOrder"].GetInt();
		//12.23
		sds_order.SDSTime = documentprase["TDFTime"].GetInt();
		sds_order.Sn = documentprase["Sn"].GetInt();
	}


	return 0;

}
int Parase2Object(std::string json_data, SDS_OderQueen &sds_order_queen){

	rapidjson::Document documentprase;
	documentprase.Parse<0>(json_data.c_str());
	const char * IntParams[] = { "Seqno", "Date", "Time", "Side", "Price","Orders","ABItems" };//8
	const char * ArrayParams[] = { "ABVolume" };
	if (documentprase.HasParseError())  //解析发生错误时错误提示  
	{
		printf("GetParseError %s\n", documentprase.GetParseError());
		return -1;//解析失败
	}
	else if (documentprase.IsObject())
	{
		for (int i = 0; i <sizeof(IntParams) / sizeof(IntParams[0]); i++)
		{
			if (!documentprase.HasMember(IntParams[i]) || !documentprase[IntParams[i]].IsInt())
				return -1;
		}
		if (!documentprase.HasMember("Code") || !documentprase["Code"].IsString())
			return -1;
		
		for (int i = 0; i < sizeof(ArrayParams) / sizeof(ArrayParams[0]); i++)
		{
			if (!documentprase.HasMember(ArrayParams[i]) || !documentprase[ArrayParams[i]].IsArray())
				return -1;
		}

		sds_order_queen.Seqno = documentprase["Seqno"].GetInt();
		const char * code = documentprase["Code"].GetString();
		memset(sds_order_queen.Code, 0, sizeof(sds_order_queen.Code));
		SPRINTF(sds_order_queen.Code, "%s", code);

		sds_order_queen.Date = documentprase["Date"].GetInt();
		sds_order_queen.Time = documentprase["Time"].GetInt();
		sds_order_queen.Side = documentprase["Side"].GetInt();
		sds_order_queen.Price = documentprase["Price"].GetInt();
		sds_order_queen.Orders = documentprase["Orders"].GetInt();
		sds_order_queen.ABItems = documentprase["ABItems"].GetInt();
		/*
		std::vector<N4> vhq;
		for (size_t i = 0; i <sizeof(ArrayParams) / sizeof(ArrayParams[0]); ++i) {
			const rapidjson::Value &temparray = documentprase[ArrayParams[i]];
			for (rapidjson::SizeType j = 0; j < temparray.Size(); ++j)
				vhq.push_back(temparray[j].GetInt());
		}*/
		const rapidjson::Value &temparray = documentprase[ArrayParams[0]];
		for (int i = 0; i < sds_order_queen.ABItems; i++)
		{
			sds_order_queen.ABVolume[i] = temparray[i].GetInt();
		}

	}
return 0;
}
int Parase2Object(std::string json_data, SDS_Code &sds_code){

	rapidjson::Document documentprase;
	documentprase.Parse<0>(json_data.c_str());
	const char * StringParams[] = { "Code", "ENName", "CNName" };//8
	if (documentprase.HasParseError())  //解析发生错误时错误提示  
	{
		printf("GetParseError %s\n", documentprase.GetParseError());
		return -1;//解析失败
	}
	else if (documentprase.IsObject())
	{
		for (int i = 0; i <sizeof(StringParams) / sizeof(StringParams[0]); i++)
		{
			if (!documentprase.HasMember(StringParams[i]) || !documentprase[StringParams[i]].IsString())
				return -1;
		}
		
		const char * code = documentprase["Code"].GetString();
		memset(sds_code.Code, 0, sizeof(sds_code.Code));
		SPRINTF(sds_code.Code, "%s", code);
		const char * en_name = documentprase["ENName"].GetString();
		memset(sds_code.ENName, 0, sizeof(sds_code.ENName));
		SPRINTF(sds_code.ENName, "%s", en_name);
		const char * cn_name = documentprase["CNName"].GetString();
		memset(sds_code.CNName, 0, sizeof(sds_code.CNName));
		SPRINTF(sds_code.CNName, "%s", cn_name);
	
	}
	return 0;
}

int Parase2Object(std::string json_data, SDS_CloseMarket &sds_close_market){

	rapidjson::Document documentprase;
	documentprase.Parse<0>(json_data.c_str());
	const char * IntParams[] = { "Seqno","Time" };//8
	const char * StringParams[] = { "Market"};//8
	if (documentprase.HasParseError())  //解析发生错误时错误提示  
	{
		printf("GetParseError %s\n", documentprase.GetParseError());
		return -1;//解析失败
	}
	else if (documentprase.IsObject())
	{
		for (int i = 0; i < sizeof(StringParams) / sizeof(StringParams[0]); i++)
		{
			if (!documentprase.HasMember(IntParams[i]) || !documentprase[IntParams[i]].IsInt())
				return -1;
			if (!documentprase.HasMember(StringParams[i]) || !documentprase[StringParams[i]].IsString())
				return -1;
			sds_close_market.Seqno = documentprase["Seqno"].GetInt();
			sds_close_market.Time = documentprase["Time"].GetInt();
			const char * market = documentprase["Market"].GetString();
			memset(sds_close_market.Market, 0, sizeof(sds_close_market.Market));
			SPRINTF(sds_close_market.Market, "%s", market);




		}

	}
	return 0;
}
/*
int StrInsertChar(string in, int offset, char *out)
{
	for (int i = 0; i < in.length();i++)
	{
		out[i + offset] = in[i];
	}
	return 0;
}*/
int Sring2Dac(SDS_Level2 &sds_level2, string  &out2dac)
{

	char data[642];
	memset(data, ' ', sizeof(data));
	string message(sds_level2.Code, sizeof(sds_level2.Code));
	std::vector<std::string> info_spit;
	split(message, ".", &info_spit);//Market|Time
	/*for (int i = 0; i < info_spit[0].size(); i++)
	{
		data[i] = info_spit[0][i];//0-5 
	}*/
		int paratemp[58] = { 6, 6, 8, 9, 10, 10, 10, 10, 10, 10, 10,
		9, 19, 19, 19, 19, 10, 10,
		10, 12, 10, 12, 10, 12, 10, 12, 10, 12, 10, 12, 10, 12, 10, 12, 10, 12, 10, 12,
		10, 12, 10, 12, 10, 12, 10, 12, 10, 12, 10, 12, 10, 12, 10, 12, 10, 12, 10, 12
	};//18};//18
	//int para10temp[2] = {10,12};//40个字段
	//计算偏移距离
	int offpointer[58] = {0};
	int sum = 0;
	for (int i = 1; i < 58; i++)
	{
		sum += paratemp[i-1];
		offpointer[i] = sum;
	}
	/*
	for (int i = 18; i < 58; i=i+2)
	{
		sum += para10temp[0];
		offpointer[i] = sum;
		sum += para10temp[1];
		offpointer[i+1] = sum;
	}*/
	StrInsertChar(info_spit[0], offpointer[0], data);//0-5
	for (int i = 0; i < 2;i++)
	{
		data[i + offpointer[1]] = info_spit[1][i];
	}
	//rInsertChar(info_spit[1], offpointer[1], data);//6-11
	StrInsertChar(itostring(sds_level2.Date), offpointer[2], data);//12-19
	int dac_time = sds_level2.Time;
	if (dac_time<100000000)
		StrInsertChar(itostring(dac_time), offpointer[3] + 1, data);
	else
		StrInsertChar(itostring(dac_time), offpointer[3], data);//12月17日修改
	//StrInsertChar(itostring(sds_level2.Time), offpointer[3], data);//20-28
	StrInsertChar(itostring(sds_level2.PreClose), offpointer[4], data);//29-38
	StrInsertChar(itostring(sds_level2.Open), offpointer[5], data);//39-48
	StrInsertChar(itostring(sds_level2.High), offpointer[6], data);//49-58
	StrInsertChar(itostring(sds_level2.Low), offpointer[7], data);//59-68
	StrInsertChar(itostring(sds_level2.Price), offpointer[8], data);//69-78
	StrInsertChar(itostring(sds_level2.HighLimited), offpointer[9], data);//79-88;
	StrInsertChar(itostring(sds_level2.LowLimited), offpointer[10], data);//89-98

	StrInsertChar(itostring(sds_level2.NumTrades), offpointer[11], data);
	StrInsertChar(itostring(sds_level2.Volume), offpointer[12], data);
	StrInsertChar(itostring(sds_level2.Turnover), offpointer[13], data);
	StrInsertChar(itostring(sds_level2.TotalBidVol), offpointer[14], data);
	StrInsertChar(itostring(sds_level2.TotalAskVol), offpointer[15], data);
	StrInsertChar(itostring(sds_level2.AvgBidPrice), offpointer[16], data);
	StrInsertChar(itostring(sds_level2.AvgAskPrice), offpointer[17], data);

	for (int i = 0; i < LEVLEL2LENGTH; i++)//18-37
	{
		StrInsertChar(itostring(sds_level2.AskPrice[LEVLEL2LENGTH - (i + 1)]),offpointer[18 +2*i], data);
		StrInsertChar(itostring(sds_level2.AskVol[LEVLEL2LENGTH - (i + 1)]), offpointer[18+2*i+1], data);
	}
	for (int i = 0; i < LEVLEL2LENGTH; i++)//38-57
	{
		StrInsertChar(itostring(sds_level2.BidPrice[i]), offpointer[38+2*i], data);
		StrInsertChar(itostring(sds_level2.BidVol[i]), offpointer[38+2*i+1], data);
		std::cout << "i:" << i << std::endl;
	}

	/*for (int i = 0; i < info_spit[1].size(); i++)
	{
		data[6] = info_spit[0][i];//6-11
	}*/
	std::string sdata(data, sizeof(data));
	out2dac = sdata;
	return 0;
}

int Sring2Dac(SDS_Transaction &sds_trans, string  &out2dac)
{

	char data[112];
	memset(data, ' ', sizeof(data));
	string message(sds_trans.Code, sizeof(sds_trans.Code));
	std::vector<std::string> info_spit;
	split(message, ".", &info_spit);//Market|Time
	int paratemp[13] = { 10, 6, 6,8, 9, 10, 19, 19, 1,2,1,10,10};//18};//18
	int offpointer[13] = { 0 };
	int sum = 0;
	for (int i = 1; i < 13; i++)
	{
		sum += paratemp[i - 1];
		offpointer[i] = sum;
	}
	StrInsertChar(itostring(sds_trans.Index), offpointer[0], data);//12-19
	StrInsertChar(info_spit[0], offpointer[1], data);

	for (int i = 0; i < 2; i++)
	{
		data[i + offpointer[2]] = info_spit[1][i];//取出info_spit[1]的两个字符
	}

	StrInsertChar(itostring(sds_trans.Date), offpointer[3], data);//12-19
	int dac_time = sds_trans.Time;
	if (dac_time<100000000)
		StrInsertChar(itostring(dac_time), offpointer[4] + 1, data);
	else
		StrInsertChar(itostring(dac_time), offpointer[4], data);
	StrInsertChar(itostring(sds_trans.Price), offpointer[5], data);//39-48
	StrInsertChar(itostring(sds_trans.Volume), offpointer[6], data);//49-58
	StrInsertChar(itostring(sds_trans.Turnover), offpointer[7], data);//59-68
	data[offpointer[8]] = sds_trans.BSFlag;
	data[offpointer[9]] = sds_trans.OrderKind;
	data[offpointer[10]] = sds_trans.FunctionCode;
   
	StrInsertChar(itostring(sds_trans.AskOrder), offpointer[11], data);
	StrInsertChar(itostring(sds_trans.BidOrder), offpointer[12], data);
	std::string sdata(data, sizeof(data));
	out2dac = sdata;
	return 0;
}

int Sring2Dac(SDS_OderQueen &sds_orderqueue, string  &out2dac)
{
	char data[615];
	memset(data, ' ', sizeof(data));
	string message(sds_orderqueue.Code, sizeof(sds_orderqueue.Code));
	std::vector<std::string> info_spit;
	split(message, ".", &info_spit);//Market|Time
	int paratemp[9] = { 6, 6, 8, 9, 1, 10, 12, 12, 550};//18};//12-21
	//计算偏移距离
	int offpointer[9] = { 0 };
	int sum = 0;
	for (int i = 1; i < 9; i++)
	{
		sum += paratemp[i - 1];
		offpointer[i] = sum;
	}
	StrInsertChar(info_spit[0], offpointer[0], data);
	for (int i = 0; i < 2; i++)
	{
		data[i + offpointer[1]] = info_spit[1][i];//data 0 1
	}//
	StrInsertChar(itostring(sds_orderqueue.Date), offpointer[2], data);
	int dac_time = sds_orderqueue.Time;
	if (dac_time<100000000)
	{ 
		StrInsertChar(itostring(dac_time), offpointer[3]+1, data);
	}
	else{
	
		StrInsertChar(itostring(dac_time), offpointer[3], data);
	}
	data[offpointer[4]] = sds_orderqueue.Side;
	//StrInsertChar(itostring(sds_orderqueue.Side), offpointer[4], data);
	StrInsertChar(itostring(sds_orderqueue.Price), offpointer[5], data);
	StrInsertChar(itostring(sds_orderqueue.Orders), offpointer[6], data);
	int  itemslen = sds_orderqueue.ABItems;
		StrInsertChar(itostring(itemslen), offpointer[7], data);
		std::string volume;
	//拼接成一个字符串
		for (int i = 0; i < itemslen; i++){
			volume += itostring(sds_orderqueue.ABVolume[i]);
			if ((i +1)!= itemslen) volume += ",";
		}
	StrInsertChar(volume, offpointer[8], data);
	std::string sdata(data, sizeof(data));
	out2dac = sdata;
	return 0;
}