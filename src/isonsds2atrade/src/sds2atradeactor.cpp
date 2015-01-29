
#include "ibsdstool.h"
#include <map>
#include<mutex>
#include "isonutil.h"
#include"sds2atradeactor.h"
#include"timehelper.h"
extern int g_nlog_level;
extern int gHQSource;
std::mutex mutex_read_code;
std::mutex mutex_close_price;
std::map< std::string, std::string> mcode_table;//stockid an stock name
std::map< std::string, std::string> mlevel2;//stock the last level2
/*int counthq=0;
int level2 = 0;
int sellintime = 0;
int sdspubfailcount=0;
int pubfaillevel2=0;
int pubfailsell=0;*/
//Level22Atrade::Level22Atrade(const std::string id_):Actor(id_){};
const char * askpriceparam[] = { "askprice1", "askprice2", "askprice3", "askprice4"
, "askprice5", "askprice6", "askprice7", "askprice8", "askprice9", "askprice10" };
const char * askvolparam[] = { "askvol1", "askvol2", "askvol3", "askvol4"
, "askvol5", "askvol6", "askvol7", "askvol8", "askvol9", "askvol10" };
const char * bidpriceparam[] = { "bidprice1", "bidprice2", "bidprice3", "bidprice4"
, "bidprice5", "bidprice6", "bidprice7", "bidprice8", "bidprice9", "bidprice10" };
const char * bidvolparam[] = { "bidvol1", "bidvol2", "bidvol3", "bidvol4"
, "bidvol5", "bidvol6", "bidvol7", "bidvol8", "bidvol9", "bidvol10" };
int Level22Atrade::OnStart(ActorContext&){
	const char * leve2 = itostring(Level2).c_str();
	Subscribe("sub1", "83");
	return 0;
};
int  Level22Atrade::OnEvent(Event& e){
	std::string smss;
	if (e.type() == kPublish) {//此部分实现推送功能

		//转化成json接口


		//记录进来的时间
		DateAndTime recv4tdftimein = GetDateAndTime();
		smss = e.message();
		LOG_IF(INFO, g_nlog_level>5) << "SDS@Atrede recv:" << smss.c_str();
		////std::cout << "SDS@Atrede recv:" << smss.c_str() << endl;
		SDS_Level2  sds_level2;
		Parase2Object(smss,sds_level2);//Parase Level2
		//Package to atrade
		rapidjson::Document document;
		document.SetObject();
		rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
		document.AddMember("head", "1009", allocator);
		rapidjson::Value body;
		body.SetObject();
		vector<string> code_spit;
		split(sds_level2.Code, ".", &code_spit);
		body.AddMember("stockid", code_spit[0].c_str(), allocator); //2
		string stock_name = mcode_table[sds_level2.Code];
		body.AddMember("stockname", stock_name.c_str(), allocator);
		body.AddMember("time", sds_level2.Time, allocator);//时间  4
		body.AddMember("yesterdayclose", sds_level2.PreClose, allocator);//前收盘价  5
		body.AddMember("todayopen",sds_level2.Open, allocator);//开盘价 6
		body.AddMember("todayhigh",sds_level2.High, allocator);//最高价 7
		body.AddMember("todaylow", sds_level2.Low, allocator);//最低价 8
		body.AddMember("lastprice",sds_level2.Price, allocator);//最新价 9
		body.AddMember("topprice", 1.1*sds_level2.PreClose,allocator);
		body.AddMember("lowestprice", 0.9*sds_level2.PreClose, allocator);
		body.AddMember("executevolume",sds_level2.NumTrades, allocator);//成交笔数 add in 2014-11-07 10
		
		//body.AddMember("totalvolume", sds_level2.Volume, allocator);
		//body.AddMember("totalamount", sds_level2.Turnover, allocator);
		AddMember64(body, allocator, "totalvolume", sds_level2.Volume);//成交总量  string  11
		AddMember64(body, allocator, "totalamount", sds_level2.Turnover);//成交总金额  string  12
		for (int i = 0; i < 10; i++)//Ten HQ
		{	/*
			std::string  askpriceparam = concatenate("askprice", i + 1);
			std::string  askvolparam = concatenate("askvol", i + 1);
			std::string bidkpriceparam = concatenate("bidprice", i + 1);
			std::string  bidvolparam = concatenate("bidvol", i + 1);*/
		body.AddMember(askpriceparam[i], sds_level2.AskPrice[i], allocator);
		body.AddMember(askvolparam[i], sds_level2.AskVol[i], allocator);
		body.AddMember(bidpriceparam[i], sds_level2.BidPrice[i], allocator);
		body.AddMember(bidvolparam[i], sds_level2.BidVol[i], allocator);
	    }

		//2012-12-23
		body.AddMember("sdstime", sds_level2.SDSTime, allocator);//sds时间
		body.AddMember("atradeintime", recv4tdftimein.time, allocator);//进入atrade的时间
		body.AddMember("sn", sds_level2.Sn, allocator);
		document.AddMember("body", body, allocator);
		//pub  
		std::string message=Document2String(document);	
	////	std::cout << "SDS@Atrede pub level2:" << message.c_str() << endl;
		//int ret = Publish("pub1","1009", message.c_str());
		int ret = Publish("pub1",code_spit[0].c_str(), message.c_str());
		LOG_IF(INFO, g_nlog_level>5) << "SDS@Atrede pub:" << message.c_str();
		mlevel2[code_spit[0].c_str()] = message.c_str();
		//记录出去的时间
	}
	
	return 0;

}

int Level2Req::OnStart(ActorContext&){
	const char * leve2 = itostring(Level2).c_str();
	Subscribe("sub1", "83");
	return 0;
};
int  Level2Req::OnEvent(Event& e){	
	std::string smss;
	if (e.type() == kMessage)
	{

		string message = e.message();
		rapidjson::Document documentprase;
		documentprase.Parse<0>(message.c_str());
		if (documentprase.HasParseError())  //解析发生错误时错误提示  
		{
			printf("GetParseError %s\n", documentprase.GetParseError());
			return -1;
		}
		else if (documentprase.IsObject())
		{
			if (!documentprase.HasMember("Seqno") || !documentprase["Seqno"].IsInt() || documentprase["Seqno"].GetInt() != 1008)
				return -1;
			if (!documentprase.HasMember("Code") || !documentprase["Code"].IsArray())
				return -1;
			const rapidjson::Value &codearray = documentprase["Code"];

			for (rapidjson::SizeType i = 0; i < codearray.Size(); ++i)
			{
				//读的时候加锁
				mutex_read_code.lock();
				std::string temp_code = codearray[i].GetString();
				mutex_read_code.unlock();
				try {
				
					Publish("pub2", temp_code.c_str(), mlevel2.at(temp_code).c_str());
					std::cout << "Req level2:" << mlevel2[temp_code].c_str() << endl;
				}
				catch (std::exception){
					std::cout << temp_code.c_str() << "is not find" << endl;
					return -1;

				}

			}
		}
	}



}

//SellInTime2Atrade::SellInTime2Atrade(std::string id_) : Actor(id_){};
int SellInTime2Atrade::OnStart(ActorContext&){
	const char * leve2 = itostring(Transaction).c_str();
	Subscribe("sub1", "84");
	return 0;
};
int  SellInTime2Atrade::OnEvent(Event &e){
	std::string smss;
	if (e.type() == kPublish) {//此部分实现推送功能
		DateAndTime recv4tdftimetransin = GetDateAndTime();
		smss = e.message();
		LOG_IF(INFO, g_nlog_level>5) << "SDS@Atrede recv:" << smss.c_str();
		////std::cout << "SDS@Atrede recv:" << smss.c_str() << endl;
		SDS_Transaction sds_sellintime;
		Parase2Object(smss, sds_sellintime);//Parase Level2
		//Package to atrade
		rapidjson::Document document;
		document.SetObject();
		rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
		document.AddMember("head", "2001", allocator);
		rapidjson::Value body;
		body.SetObject();
		vector<string> code_spit;
		split(sds_sellintime.Code, ".", &code_spit);
		body.AddMember("stockid", code_spit[0].c_str(), allocator);
		body.AddMember("price", sds_sellintime.Price, allocator);
		body.AddMember("volume", sds_sellintime.Volume, allocator);
		body.AddMember("txntime", sds_sellintime.Time, allocator);
		body.AddMember("sdstime", sds_sellintime.SDSTime, allocator);//sds时间
		body.AddMember("atradeintime", recv4tdftimetransin.time, allocator);//进入atrade的时间
		body.AddMember("sn", sds_sellintime.Sn, allocator);
		document.AddMember("body",body,allocator);
		std::string message = Document2String(document);	
		////std::cout << "SDS@Atrede pub sellintime:" << message.c_str() << endl;
		//int ret = Publish("pub1","2001", message.c_str());
		int ret = Publish("pub1", code_spit[0].c_str(), message.c_str());
		LOG_IF(INFO, g_nlog_level>5) << "SDS@Atrede pub:" << message.c_str();
	}
	return 0;
}
int SubCodeTable::OnStart(ActorContext&){
	Subscribe("sub1", "10");
	return 0;
};

int  SubCodeTable::OnEvent(Event &e){
	std::string smss;
	if (e.type() == kPublish) {//此部分实现推送功能
		smss = e.message();
		SDS_Code sds_code;//
		Parase2Object(smss, sds_code);//Parase Level2
		//Package to atrade
		/*
		string codeid = sds_code.Code;
		string codename = sds_code.CNName;
		string codenamegb = sds_code.CNName;
		//string codename=" " ;
		iconv_code("utf-8", "gb18030", codenamegb, &codename);
		mcode_table[codeid] = codename;*/
	}
	return 0;
}

//返回收盘价格
int ClosePriceReq::OnEvent(Event &e){
	//接收闭市消息
	//把内存中的
	mutex_close_price.lock();
	int i = 0;
	rapidjson::Document docpackage;
	docpackage.SetObject();
	rapidjson::Document::AllocatorType& allocator = docpackage.GetAllocator();
	rapidjson::Value body;
	body.SetArray();
	std::string code[1000] ;
	int price[1000] ;
	for (auto iter = mlevel2.begin();iter!= mlevel2.end();iter++) {
		i++;
		int j = i - 1;
		std::string json_leve2 = iter->second;
        //开始解析出
		rapidjson::Document documentprase;
		documentprase.Parse<0>(json_leve2.c_str());
		
		if (documentprase.HasParseError())  //解析发生错误时错误提示  
		{
			printf("GetParseError %s\n", documentprase.GetParseError());
			return -1;//解析失败
		}
		else if (documentprase.IsObject())
		{
			if (!documentprase.HasMember("body") || !documentprase["body"].IsObject())
				return -1;
			rapidjson::Value &body_parase = documentprase["body"];
			if (!body_parase.IsObject())
				return -1;
			std::string stockid = body_parase["stockid"].GetString();
			int lastprice = body_parase["lastprice"].GetInt();
			//code.push_back(stockid);
			//price.push_back(lastprice);
			//开始打包	
			if (i == 1)
			{
				docpackage.AddMember("head", "4444", allocator);
			}
			rapidjson::Value close_price;
			close_price.SetObject();
		    close_price.AddMember("Code", code[j].c_str(), allocator);
			close_price.AddMember("Price", price[j], allocator);
			//close_price.AddMember("Code", stockid.c_str(), allocator);
			//close_price.AddMember("Price", lastprice, allocator);
			if (i < 3)
			{
				body.PushBack(close_price, allocator);

			}
			if (i == 3)
			{
				docpackage.AddMember("body", body, allocator);
				std::string closeprice2ib = Document2String(docpackage);
				e.Reply(closeprice2ib.c_str());
				i = 0;
				docpackage.RemoveMember("body");
				body.SetArray();
				//code.clear();
				//price.clear();
			}
		}

		
		
	}
	//最后一次不足1000条数据的包返回
	std::string closeprice2ib = Document2String(docpackage);
	e.Reply(closeprice2ib);
	//docpackage.Clear();
	mutex_close_price.unlock();
	return 0;
	
}







/*
ConfigModifyActor::ConfigModifyActor(std::string id_) :Actor(id_)
{
}
int ConfigModifyActor::OnStart(ActorContext&) {
	
	return 0;
}
int ConfigModifyActor::OnEvent(Event& e){
	std::string smss;
	if (e.type() == kMessage) {//此部分实现推送功能
		smss = e.message();
		const	 char* cmss = smss.c_str();
 rapidjson::Document documentprase;
		documentprase.Parse<0>(cmss);
		if (documentprase.HasParseError())  //解析发生错误时错误提示  
		{
			printf("GetParseError %s\n", documentprase.GetParseError());
			return -1;//解析失败
		}
		else if (documentprase.IsObject()) 
		{	
			if (documentprase.HasMember("LogLevel")) //对数据进行存储,存储方式为key-value  key为股票code+time
			{
				gLogLevel = documentprase["LogLevel"].GetInt();
			
			}
		}
	}
return 0;
}*/
