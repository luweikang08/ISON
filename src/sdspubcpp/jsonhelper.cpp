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
#include"sdsdataapi.h"
#include "sdsdataapi.cpp"

//Read the config file
int ReadConfig(const char* filename, std::vector <std::string> configkey,std::map<std::string,std::string> &configmap){
	std::ifstream is;
	is.open(filename);
	if (is){
		std::string str((std::istreambuf_iterator<char>(is)),
		std::istreambuf_iterator<char>());
		rapidjson::Document documentprase;
		const char* ssms;
		ssms = str.c_str();
		documentprase.Parse<0>(ssms);
		if (documentprase.HasParseError())
		{
			return ERR_PARASE;
			printf("ParseError error:%s\n", documentprase.GetParseError());
			LOG(WARNING) << ERR_PARASE << ":" << documentprase.GetParseError();
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
					return ERR_NO_KEY;		
				}
				if (documentprase[key.c_str()].IsString())
					configmap[key] = documentprase[key.c_str()].GetString();
				else if (documentprase[key.c_str()].IsInt())
				{  //转化成string
					int temp = documentprase[key.c_str()].GetInt();
					configmap[key]= itostring(temp);//要转化回去的话用atoi
				}
				else {
					return ERR_VALUETPYE_ERROR; 
					std::cout << "Parase config file fail :" << key << " type is not  string or int!" << std::endl;
				}
			}
		}
	}
	else 

	{
		std::cout << "Read config file fail,so  use the default params!" << std::endl;
		return -1;
	
	}
	is.close();
	std::cout << "Read config file " << filename << " sucessfully!" << std::endl;
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

int Parase2Object(std::string json_data, baseline::SDS_Level2 *sds_level2, char * buffer, int &bufferLength){

	rapidjson::Document documentprase;
	documentprase.Parse<0>(json_data.c_str());
	const char * IntParams[] = { "Seqno", "Date", "Time", "PreClose", "Open", "High", "Low", "Price", "NumTrades", "AvgBidPrice", "AvgAskPrice", "HighLimited", "LowLimited","SourceTime", "Sn" };//8   
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
				return -1;
		}
		if (!documentprase.HasMember("Code") || !documentprase["Code"].IsString())
			return -1;
		for (int i = 0; i <sizeof(Int64Params) / sizeof(Int64Params[0]);i++)
		{
			if (!documentprase.HasMember(Int64Params[i]) || !documentprase[Int64Params[i]].IsString())
				return -1;
		}
		for (int i = 0; i < sizeof(ArrayParams) / sizeof(ArrayParams[0]); i++)
		{
			if (!documentprase.HasMember(ArrayParams[i]) || !documentprase[ArrayParams[i]].IsArray())
				return -1;
		}
		std::vector<N4> vhq;
		for (size_t i = 0; i <sizeof(ArrayParams) / sizeof(ArrayParams[0]); ++i) {
			const rapidjson::Value &temparray = documentprase[ArrayParams[i]];
			for (rapidjson::SizeType j = 0; j < temparray.Size(); ++j)
				vhq.push_back(temparray[j].GetInt());
		}
		const char * code = documentprase["Code"].GetString();
		//char buffer[1024];
		//int bufferLength = sizeof(buffer);
		baseline::MessageHeader hdr;
		//先对头进行encode
		hdr.wrap(buffer, 0, messageHeaderVersion, bufferLength)
			.blockLength(baseline::SDS_Level2::sbeBlockLength())
			.templateId(baseline::SDS_Level2::sbeTemplateId())
			.schemaId(baseline::SDS_Level2::sbeSchemaId())
			.version(baseline::SDS_Level2::sbeSchemaVersion());
		//对data进行encode
		
		sds_level2->wrapForEncode(buffer, hdr.size(), bufferLength);
		for (int i = 0, size = 10; i < size; i++)
		{
			sds_level2->askPrice(i, vhq[i]);
			sds_level2->askVol(i, vhq[i + 10]);
			sds_level2->bidPrice(i, vhq[i + 20]);
			sds_level2->bidVol(i, vhq[i + 30]);
		}

		sds_level2->seqno(documentprase["Seqno"].GetInt());
		sds_level2->date(documentprase["Date"].GetInt());
		sds_level2->time(documentprase["Time"].GetInt());
		sds_level2->preClose(documentprase["PreClose"].GetInt());
		sds_level2->open(documentprase["Open"].GetInt());
		sds_level2->high(documentprase["High"].GetInt());
		sds_level2->low(documentprase["Low"].GetInt());
		sds_level2->price(documentprase["Price"].GetInt());
		sds_level2->volume(atoll(documentprase["Volume"].GetString()));
		sds_level2->turnover(atoll(documentprase["Turnover"].GetString()));
		sds_level2->totalBidVol(atoll(documentprase["TotalBidVol"].GetString()));
		sds_level2->totalAskVol(atoll(documentprase["TotalAskVol"].GetString()));
		sds_level2->numTrades(documentprase["NumTrades"].GetInt());
		sds_level2->avgBidPrice(documentprase["AvgBidPrice"].GetInt());
		sds_level2->avgAskPrice(documentprase["AvgAskPrice"].GetInt());
		sds_level2->lowLimited(documentprase["LowLimited"].GetInt());
		sds_level2->highLimited(documentprase["HighLimited"].GetInt());
		sds_level2->putCode(code);
		
	}		
	return 0;

}
int Parase2Object(std::string json_data, baseline::SDS_Transaction * sds_transaction,char * buffer, int &bufferLength){
	rapidjson::Document documentprase;
	documentprase.Parse<0>(json_data.c_str());
	const char * IntParams[] = { "Seqno", "Date", "Time", "Index", "Price", "Volume", "Turnover", "FunctionCode",
		"AskOrder", "BidOrder" ,"SourceTime", "Sn" };//10
	const char * CharParams[] = {"BSFlag", "OrderKind" };//2
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
				printf("no this Paramas:%s", IntParams[i]);
				return -1;
			}
		}
		if (!documentprase.HasMember("Code") || !documentprase["Code"].IsString())
			return -1;
		for (int i = 0; i < sizeof(CharParams) / sizeof(CharParams[0]); i++)
		{
			if (!documentprase.HasMember(CharParams[i]) || !documentprase[CharParams[i]].IsInt())
			{
				printf("no this Paramas:%s", CharParams[i]);
				return -1;
			}
		}
		baseline::MessageHeader hdr;
		//先对头进行encode
		hdr.wrap(buffer, 0, messageHeaderVersion, bufferLength)
			.blockLength(baseline::SDS_Transaction::sbeBlockLength())
			.templateId(baseline::SDS_Transaction::sbeTemplateId())
			.schemaId(baseline::SDS_Transaction::sbeSchemaId())
			.version(baseline::SDS_Transaction::sbeSchemaVersion());
		//对data进行encode

		sds_transaction->wrapForEncode(buffer, hdr.size(), bufferLength);
		//12.30为止15个字段
		sds_transaction->seqno(documentprase["Seqno"].GetInt());
		const char * code =documentprase["Code"].GetString();
		sds_transaction->putCode(code);
		sds_transaction->date(documentprase["Date"].GetInt());
		sds_transaction->time (documentprase["Time"].GetInt());
		sds_transaction->index (documentprase["Index"].GetInt());
		sds_transaction->price(documentprase["Price"].GetInt());
		sds_transaction->volume(documentprase["Volume"].GetInt());
		sds_transaction->turnover(documentprase["Turnover"].GetInt());
		sds_transaction->functionCode(documentprase["FunctionCode"].GetInt());
		sds_transaction->askOrder(documentprase["AskOrder"].GetInt());
		sds_transaction->bidOrder(documentprase["BidOrder"].GetInt());
		sds_transaction->bSFlag(documentprase["BSFlag"].GetInt());
		sds_transaction->orderKind(documentprase["OrderKind"].GetInt());
		sds_transaction->sDSTime(documentprase["SourceTime"].GetInt());
		sds_transaction->sn(documentprase["Sn"].GetInt());
	}
	return 0;

}

int Parase2Object(std::string json_data, baseline::SDS_OderQueen *sds_order_queen,char * buffer, int &bufferLength){

	rapidjson::Document documentprase;
	documentprase.Parse<0>(json_data.c_str());
	const char * IntParams[] = { "Seqno", "Date", "Time", "Side", "Price", "Orders", "ABItems","SourceTime", "Sn" };//8
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

		baseline::MessageHeader hdr;
		//先对头进行encode
		hdr.wrap(buffer, 0, messageHeaderVersion, bufferLength)
			.blockLength(baseline::SDS_Transaction::sbeBlockLength())
			.templateId(baseline::SDS_Transaction::sbeTemplateId())
			.schemaId(baseline::SDS_Transaction::sbeSchemaId())
			.version(baseline::SDS_Transaction::sbeSchemaVersion());
		sds_order_queen->wrapForEncode(buffer, hdr.size(), bufferLength);
		sds_order_queen->seqno(documentprase["Seqno"].GetInt());
		const char * code = documentprase["Code"].GetString();
		sds_order_queen->putCode(code);
		sds_order_queen->date(documentprase["Date"].GetInt());
		sds_order_queen->time(documentprase["Time"].GetInt());
		sds_order_queen->side(documentprase["Side"].GetInt());
		sds_order_queen->price(documentprase["Price"].GetInt());
		sds_order_queen->orders(documentprase["Orders"].GetInt());
		sds_order_queen->aBItems(documentprase["ABItems"].GetInt());
		sds_order_queen->sDSTime(documentprase["SourceTime"].GetInt());
		sds_order_queen->sn(documentprase["Sn"].GetInt());
		
		const rapidjson::Value &volumearray = documentprase[ArrayParams[0]];
		for (int i = 0; i < sds_order_queen->aBItems(); i++)
		{
			sds_order_queen->aBVolume (i,volumearray[i].GetInt());
		}

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

int Sring2Dac(baseline::SDS_Level2 &sds_level2, string  &out2dac)
{

	char data[642];
	memset(data, ' ', sizeof(data));
	char buffer[16] = {0};
	sds_level2.getCode(buffer, 16);
	string message (buffer, sizeof(buffer));
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
	int offpointer[58] = { 0 };
	int sum = 0;
	for (int i = 1; i < 58; i++)
	{
		sum += paratemp[i - 1];
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
	for (int i = 0; i < 2; i++)
	{
		data[i + offpointer[1]] = info_spit[1][i];
	}
	//rInsertChar(info_spit[1], offpointer[1], data);//6-11
	int Date = sds_level2.date();
	StrInsertChar(itostring(Date), offpointer[2], data);//12-19
	int dac_time = sds_level2.time();
	if (dac_time<100000000)
		StrInsertChar(itostring(dac_time), offpointer[3] + 1, data);
	else
		StrInsertChar(itostring(dac_time), offpointer[3], data);//12月17日修改
	//StrInsertChar(itostring(sds_level2.Time), offpointer[3], data);//20-28
	StrInsertChar(itostring((int)sds_level2.preClose()), offpointer[4], data);//29-38
	StrInsertChar(itostring((int)sds_level2.open()), offpointer[5], data);//39-48
	StrInsertChar(itostring((int)sds_level2.high()), offpointer[6], data);//49-58
	StrInsertChar(itostring((int)sds_level2.low()), offpointer[7], data);//59-68
	StrInsertChar(itostring((int)sds_level2.price()), offpointer[8], data);//69-78
	StrInsertChar(itostring((int)sds_level2.highLimited()), offpointer[9], data);//79-88;
	StrInsertChar(itostring((int)sds_level2.lowLimited()), offpointer[10], data);//89-98

	StrInsertChar(itostring((int)sds_level2.numTrades()), offpointer[11], data);
	StrInsertChar(itostring((long long)sds_level2.volume()), offpointer[12], data);
	StrInsertChar(itostring((long long)sds_level2.turnover()), offpointer[13], data);
	StrInsertChar(itostring((long long)sds_level2.totalBidVol()), offpointer[14], data);
	StrInsertChar(itostring((long long)sds_level2.totalAskVol()), offpointer[15], data);
	StrInsertChar(itostring((int)sds_level2.avgBidPrice()), offpointer[16], data);
	StrInsertChar(itostring((int)sds_level2.avgAskPrice()), offpointer[17], data);

	//int askPrice[10] = {0};
	//const char* askPrice_ = sds_level2.askPrice();
	//std::memcpy(askPrice, askPrice_, 40);
	for (int i = 0; i < LEVLEL2LENGTH; i++)//18-37
	{
		StrInsertChar(itostring((int)sds_level2.askPrice(LEVLEL2LENGTH - (i + 1))), offpointer[18 + 2 * i], data);
		StrInsertChar(itostring((int)sds_level2.askVol(LEVLEL2LENGTH - (i + 1))), offpointer[18 + 2 * i + 1], data);
	}
	for (int i = 0; i < LEVLEL2LENGTH; i++)//38-57
	{
		StrInsertChar(itostring((int)sds_level2.bidPrice(i)), offpointer[38 + 2 * i], data);
		StrInsertChar(itostring((int)sds_level2.bidVol(i)), offpointer[38 + 2 * i + 1], data);
	}

	/*for (int i = 0; i < info_spit[1].size(); i++)
	{
	data[6] = info_spit[0][i];//6-11
	}*/
	std::string sdata(data, sizeof(data));
	out2dac = sdata;
	return 0;
}


int Sring2Dac(baseline::SDS_Transaction &sds_trans, string  &out2dac)
{

	char data[112];
	memset(data, ' ', sizeof(data));
	string message(sds_trans.code(), 16);
	std::vector<std::string> info_spit;
	split(message, ".", &info_spit);//Market|Time
	int paratemp[13] = { 10, 6, 6, 8, 9, 10, 19, 19, 1, 2, 1, 10, 10 };//18};//18
	int offpointer[13] = { 0 };
	int sum = 0;
	for (int i = 1; i < 13; i++)
	{
		sum += paratemp[i - 1];
		offpointer[i] = sum;
	}
	StrInsertChar(itostring((int)sds_trans.index()), offpointer[0], data);//12-19
	StrInsertChar(info_spit[0], offpointer[1], data);

	for (int i = 0; i < 2; i++)
	{
		data[i + offpointer[2]] = info_spit[1][i];//取出info_spit[1]的两个字符
	}

	StrInsertChar(itostring((int)sds_trans.date()), offpointer[3], data);//12-19
	int dac_time = sds_trans.time();
	if (dac_time<100000000)
		StrInsertChar(itostring(dac_time), offpointer[4] + 1, data);
	else
		StrInsertChar(itostring(dac_time), offpointer[4], data);
	StrInsertChar(itostring((int)sds_trans.price()), offpointer[5], data);//39-48
	StrInsertChar(itostring((int)sds_trans.volume()), offpointer[6], data);//49-58
	StrInsertChar(itostring((int)sds_trans.turnover()), offpointer[7], data);//59-68
	data[offpointer[8]] = sds_trans.bSFlag();
	data[offpointer[9]] = sds_trans.orderKind();
	data[offpointer[10]] = sds_trans.functionCode();

	StrInsertChar(itostring((int)sds_trans.askOrder()), offpointer[11], data);
	StrInsertChar(itostring((int)sds_trans.bidOrder()), offpointer[12], data);
	std::string sdata(data, sizeof(data));
	out2dac = sdata;
	return 0;
}


int Sring2Dac(baseline::SDS_OderQueen &sds_orderqueue, string  &out2dac)
{
	char data[615];
	memset(data, ' ', sizeof(data));
	string message(sds_orderqueue.code(), 16);
	std::vector<std::string> info_spit;
	split(message, ".", &info_spit);//Market|Time
	int paratemp[9] = { 6, 6, 8, 9, 1, 10, 12, 12, 550 };//18};//12-21
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
	StrInsertChar(itostring((int)sds_orderqueue.date()), offpointer[2], data);
	int dac_time = sds_orderqueue.time();
	if (dac_time<100000000)
	{
		StrInsertChar(itostring(dac_time), offpointer[3] + 1, data);
	}
	else{

		StrInsertChar(itostring(dac_time), offpointer[3], data);
	}
	data[offpointer[4]] = sds_orderqueue.side();
	//StrInsertChar(itostring(sds_orderqueue.Side), offpointer[4], data);
	StrInsertChar(itostring((int)sds_orderqueue.price()), offpointer[5], data);
	StrInsertChar(itostring((int)sds_orderqueue.orders()), offpointer[6], data);
	int  itemslen = sds_orderqueue.aBItems();
	StrInsertChar(itostring(itemslen), offpointer[7], data);
	std::string volume;
	//拼接成一个字符串
	for (int i = 0; i < itemslen; i++){
		volume += itostring((int)sds_orderqueue.aBVolume(i));
		if ((i + 1) != itemslen) volume += ",";
	}
	StrInsertChar(volume, offpointer[8], data);
	std::string sdata(data, sizeof(data));
	out2dac = sdata;
	return 0;
}

void SBE2Json(const std::string &sbe_message, baseline::SDS_Level2 &sds_level2, std::string &json_message){
	DecodeSBE(sbe_message, sds_level2);
	rapidjson::Document document;
	rapidjson::Document::AllocatorType &allocator = document.GetAllocator();
	document.SetObject();
	document.AddMember("SourceTime", sds_level2.time(), allocator);
	document.AddMember("Sn", sds_level2.sn(), allocator);
	document.AddMember("Seqno", (int)ISON_MSG_ID::Level2, allocator);//消息编号 1
	document.AddMember("Code", sds_level2.code(), allocator); //2
	document.AddMember("Date", sds_level2.date(), allocator);//日期  3
	document.AddMember("Time", sds_level2.time(), allocator);//时间  4
	document.AddMember("PreClose", sds_level2.preClose(), allocator);//前收盘价  5
	document.AddMember("Open", sds_level2.open(), allocator);//开盘价 6
	document.AddMember("High", sds_level2.high(), allocator);//最高价 7
	document.AddMember("Low", sds_level2.low(), allocator);//最低价 8
	document.AddMember("Price", sds_level2.price(), allocator);//最新价 9
	document.AddMember("NumTrades", sds_level2.numTrades(), allocator);//成交笔数 add in 2014-11-07 10
	AddMember64(document, allocator, "Volume", sds_level2.volume());//成交总量  string  11
	AddMember64(document, allocator, "Turnover", sds_level2.turnover());//成交总金额  string  12
	AddMember64(document, allocator, "TotalBidVol", sds_level2.totalBidVol());//add in 11-23
	AddMember64(document, allocator, "TotalAskVol", sds_level2.totalAskVol());
	document.AddMember("AvgBidPrice", sds_level2.avgBidPrice(), allocator);
	document.AddMember("AvgAskPrice", sds_level2.avgAskPrice(), allocator);
	document.AddMember("HighLimited", sds_level2.highLimited(), allocator);
	document.AddMember("LowLimited", sds_level2.lowLimited(), allocator);//add in 11-23
	rapidjson::Value aplsellprcarray(rapidjson::kArrayType);
	rapidjson::Value aplsellamtarray(rapidjson::kArrayType);
	rapidjson::Value aplbidprcarray(rapidjson::kArrayType);
	rapidjson::Value aplbidamtarray(rapidjson::kArrayType);
	for (int i = 0; i < 10; i++)
	{
		aplsellprcarray.PushBack(sds_level2.askPrice(i), allocator);
		aplsellamtarray.PushBack(sds_level2.askVol(i), allocator);
		aplbidprcarray.PushBack(sds_level2.bidPrice(i), allocator);
		aplbidamtarray.PushBack(sds_level2.bidVol(i), allocator);
	}
	document.AddMember("AskPrice", aplsellprcarray, allocator);  //13
	document.AddMember("AskVol", aplsellamtarray, allocator); //14
	document.AddMember("BidPrice", aplbidprcarray, allocator);//15
	document.AddMember("BidVol", aplbidamtarray, allocator);//16
	json_message = Document2String(document);//Ttansfer to string 

}
 void SBE2Json(const std::string &sbe_message, baseline::SDS_Transaction &sds_trans, std::string &json_message){
	DecodeSBE(sbe_message, sds_trans);
	rapidjson::Document document;
	rapidjson::Document::AllocatorType &allocator = document.GetAllocator();
	document.SetObject();
	document.AddMember("SourceTime", sds_trans.sDSTime(), allocator);
	document.AddMember("Sn",sds_trans.sn(), allocator);
	document.AddMember("Seqno", (int)ISON_MSG_ID::Transaction, allocator);//消息编号 84
	document.AddMember("Code", sds_trans.code(), allocator);
	document.AddMember("Date",sds_trans.date(), allocator); //自然日
	document.AddMember("Time", sds_trans.time(), allocator);//时间(HHMMSSmmm)
	document.AddMember("Index", sds_trans.index(), allocator);//成交编号
	document.AddMember("Price", sds_trans.index(), allocator);//成交价格
	document.AddMember("Volume", sds_trans.volume(), allocator);//成交数量
	document.AddMember("Turnover", sds_trans.turnover(), allocator);//成交金额
	document.AddMember("BSFlag", sds_trans.bSFlag(), allocator);//买卖方向
	document.AddMember("OrderKind", sds_trans.orderKind(), allocator);//成交类别
	document.AddMember("FunctionCode", sds_trans.functionCode(), allocator);//成交代码
	document.AddMember("AskOrder", sds_trans.askOrder(), allocator);//叫卖方委托序号
	document.AddMember("BidOrder", sds_trans.bidOrder(), allocator);//叫买方委托序号
	json_message= Document2String(document);	
}

void SBE2Json(const std::string &sbe_message, baseline::SDS_OderQueen &sds_queue, std::string &json_message){
	DecodeSBE(sbe_message, sds_queue);
	rapidjson::Document document;
	rapidjson::Document::AllocatorType &allocator = document.GetAllocator();
	document.SetObject();
	document.AddMember("SourceTime", sds_queue.sDSTime(), allocator);
	document.AddMember("Sn", sds_queue.sn(), allocator);
	document.AddMember("Seqno", (int)ISON_MSG_ID::OrderQueue, allocator);//消息编号 86
	document.AddMember("Code", sds_queue.code(), allocator);
	document.AddMember("Date", sds_queue.date(), allocator); //自然日
	document.AddMember("Time", sds_queue.time(), allocator);//时间(HHMMSSmmm)
	document.AddMember("Side", sds_queue.side(), allocator);//买卖方向
	document.AddMember("Price", sds_queue.price(), allocator);//委托价格
	document.AddMember("Orders",sds_queue.orders(), allocator);//订单数量
	int nabitems = sds_queue.aBItems();
	document.AddMember("ABItems", nabitems, allocator);//委托价格
	rapidjson::Value nabitemsarray(rapidjson::kArrayType);
	for (int i = 0; i < nabitems; i++)
		nabitemsarray.PushBack(nabitems,allocator);
	document.AddMember("ABVolume", nabitemsarray, allocator);//订单明细
	json_message = Document2String(document);//Ttansfer to string 
	//int res = MessageSend(itostring(OrderQueue).c_str(), source_order_queue.c_str(), g_socket);//Send to Socket
	}
