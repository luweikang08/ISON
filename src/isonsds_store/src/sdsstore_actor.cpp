#include"sdsstore_actor.h"
#include "ibsdstool.h"
#include <map>
#include <utility>
#include "isonsdsdataapi.h"
#include "kcpolydb.h"
#include "isonutil.h"
#include"timehelper.h"
#include"memoryhelper.h"
#include"memorydata.h"
#include "memorydata.cpp"
#include"filehelper.h"
#include"jsonhelper.h"
#include"mds.h"
using namespace kyotocabinet;

extern int g_nlog_level;
extern int g_HQSource;
extern PolyDB g_memorydb;

extern MemoryData < SDS_Level2 > g_level2;
extern MemoryData < SDS_Transaction > g_transaction;
extern MemoryData <SDS_Order> g_order;
extern MemoryData <SDS_OderQueen> g_order_queen;
extern string g_chq_data_dir ;//The memory_data dir
extern string g_cpersistent_hq_dir;//Trans_store the memory_data dir
extern std::string  sstart_time;
extern std::vector<int> level2_delay;
extern std::vector<int> transaction_delay;
vector < std::pair<int ,std::pair<void *,int>> > today_all_data;//<Time<pointer,Datatype>
//extern int OpenKCAndInsertKeyValue(std::string &table, const char * key, const char* & value);
//extern int OpenKCAndInsertKeyValue(PolyDB &db, const char * table, const char * key, const char* & value);
//extern int GetKCAllKeyValue(PolyDB &db, const char * table); 
std::multimap< std::string, int> stock_signal;//用于存储topic以及对于请求的stockid
extern string filename[4];
string file_suffix = ".txt";
/*template <typename T> int StoreInMemory(std::string smss, T &data, MemoryMessage<T> &memory_message){
Parase2Object(smss, data);
vector<string> code_spit;
split(data.Code, ".", &code_spit);
memory_message.time = data.Time;
memory_message.message = data;
//data.InsertMessage(code_spit[0], memory_message);
return 0;
};*/

MemoryStoreActor::MemoryStoreActor(std::string id_):Actor(id_){}
int MemoryStoreActor::OnStart(ActorContext&) {
	vector<string > test;
	Subscribe("subfromsource", "83");//订阅
	//Subscribe("subfromsource", test);//订阅
	Subscribe("subfromsource", "85");//订阅
	Subscribe("subfromsource", "86");//订阅
	Subscribe("subfromsource", " ");//订阅
	return 0;
}

int  MemoryStoreActor::OnEvent(Event& e) {
	std::string smss;
	rapidjson::Document documentprase;
	/*if (e.type()==kPublish) {//
		smss = e.message();//接受组播古来的所有消息，目前推送的是上交所和深交所的股票数据
		std::cout << "MemoryStoreActor" << endl;
		const	 char* cmss = smss.c_str();
		documentprase.Parse<0>(cmss);
		if (documentprase.HasParseError())  //解析发生错误时错误提示  
		{
			printf("GetParseError %s\n", documentprase.GetParseError());
			return -1;//解析失败
		}
		else if (documentprase.IsObject()) 
		{
			string key;
			string value;
			int seqno = 0;

			if (!documentprase.HasMember("Seqno") || !documentprase["Seqno"].IsInt())
				return -1;
				seqno = documentprase["Seqno"].GetInt();	
				std::pair<void *, int> temp;
			switch (seqno)
			{ 	   
			case Level2:
			{
				SDS_Level2 sds_level2 = {0};
				Parase2Object(smss, sds_level2);
				vector<string> code_spit;
				split(sds_level2.Code, ".", &code_spit);
				MemoryMessage<SDS_Level2> memory_message;
				memory_message.time = sds_level2.Time;
				memory_message.message = sds_level2;
				g_level2.InsertMessage(code_spit[0], memory_message);//Store in Memory as Struct data
				//OpenKCAndInsertKeyValue(g_memorydb, "level2.kch", key.c_str(), cmss); //Store in MemoryDB
				//temp = {&sds_level2,Level2 };
				today_all_data.push_back({ sds_level2.Time, { &sds_level2, Level2 } });
			}
				break;
			case Transaction:
			{
				SDS_Transaction sds_transaction = { 0 };
				Parase2Object(smss, sds_transaction);
				vector<string> code_spit;
				split(sds_transaction.Code, ".", &code_spit);
				MemoryMessage<SDS_Transaction> memory_message;
				memory_message.time = sds_transaction.Time;
				memory_message.message = sds_transaction;
				g_transaction.InsertMessage(code_spit[0], memory_message);//Store in Memory as Struct data
			   //OpenKCAndInsertKeyValue(g_memorydb, "transaction.kch", key.c_str(), cmss);
				today_all_data.push_back({ sds_transaction.Time, { &sds_transaction, Transaction} });
			}
				break;
			case Order:
			{ SDS_Order sds_order = { 0 };
			 Parase2Object(smss,sds_order);
			 vector<string> code_spit;
			 split(sds_order.Code, ".", &code_spit);
			 MemoryMessage<SDS_Order> memory_message;
			 memory_message.time = sds_order.Time;
			 memory_message.message = sds_order;
			 g_order.InsertMessage(code_spit[0], memory_message);//Store in Memory as Struct data
				//OpenKCAndInsertKeyValue(g_memorydb, "oder.kch", key.c_str(), cmss);
			 today_all_data.push_back({ sds_order.Time, { &sds_order, Order } });
			}
				break;
			case OrderQueue:
			{SDS_OderQueen sds_oder_queue = { 0 };
			Parase2Object(smss, sds_oder_queue);
			vector<string> code_spit;
			split(sds_oder_queue.Code, ".", &code_spit);
			MemoryMessage<SDS_OderQueen> memory_message;
			memory_message.time = sds_oder_queue.Time;
			memory_message.message = sds_oder_queue;
			g_order_queen.InsertMessage(code_spit[0], memory_message);//Store in Memory as Struct data
			//OpenKCAndInsertKeyValue(g_memorydb, "orderqueue.kch", key.c_str(), cmss); 
			today_all_data.push_back({ sds_oder_queue.Time, { &sds_oder_queue, Order } });
			} 
				break;
			default:break;
			}
			//提供一个可查询单日行情的接口
			if (e.type() == kMessage) {
				smss = e.message();
				documentprase.Parse<0>(cmss);
				if (documentprase.HasParseError())  //解析发生错误时错误提示  
				{   
					printf("GetParseError %s\n", documentprase.GetParseError());
					return -1;//解析失败
				}
				else if (documentprase.IsObject())
				{
					const char* table;
					const char* regex;
					std::vector< std::string > *  value;
					const char* json_field[2] = { "Table", "Regex" };
					for (int i = 0; i < 2; i++){
						if (!documentprase.HasMember(json_field[i]) || !documentprase[json_field[i]].IsString())
							return -1;
					}
					table = documentprase["Table"].GetString();
					regex = documentprase["Regex"].GetString();

					    //获得相应的数据存与value中
				//	for (auto it = (*value).begin(); it != (*value).end(); it++)
						//e.Reply((*it).c_str());
				}				
			}
		}
	}*/

	return 0;
}


PersistentStoreActor::PersistentStoreActor(std::string id_) : Actor(id_)
{
}
int PersistentStoreActor::OnStart(ActorContext&) {
	Subscribe("subfromsource", "83");//订阅
	Subscribe("subfromsource", "84");//订阅
	Subscribe("subfromsource", "85");//订阅
	Subscribe("subfromsource", "86");//订阅
	Subscribe("subfromsource", "1012"); 

	return 0;
}

int  PersistentStoreActor::OnEvent(Event& e) {
	std::cout << "PersistentStoreActor" << endl;
	std::string smss;
	if (e.type() == kPublish) {//此部分实现推送功能
		smss = e.message()+"\n";//接受KMDS-Socket 推送过来的所有消息，目前推送的是上交所和深交所的股票数据
		/*const	 char* cmss = smss.c_str();
		rapidjson::Document documentprase;
		documentprase.Parse<0>(cmss);
		if (documentprase.HasParseError())  //解析发生错误时错误提示  
		{
			printf("GetParseError %s\n", documentprase.GetParseError());
			return -1;//解析失败
		}
		else if (documentprase.IsObject())
		{
			string message;
			int seqno = 0;
			if (documentprase.HasMember("Seqno")) //对数据进行存储,存储方式为key-value  key为股票code+time
			{
			   seqno = documentprase["Seqno"].GetInt();
			   //message = itostring(documentprase["Seqno"].GetInt()) + "|"
			}
			
			/*if (!documentprase.HasMember("Body") || !documentprase["Body"].IsObject())
				return -1;
			rapidjson::Value &body = documentprase["Body"];
			if (!body.HasMember("Code") || !body["Code"].IsString() || !body.HasMember("Time") || !body["Time"].IsInt())
				return -1;*/
			//message = message + body["Code"].GetString() + "|" + itostring(body["Time"].GetInt()) + " " + smss;
			/*std::cout << "PSeqno" << seqno<<endl;
			switch (seqno){
			case Level2:
			{   //解析成相应的对象。

				SDS_Level2 sds_level2;
				Parase2Object(smss, sds_level2);
				char recvBuf[1024];
				//memset(recvBuf, 0, 1024);
				//memcpy(recvBuf, (char *)&sds_level2, sizeof(SDS_Level2));
				string out2dac;
				Sring2Dac(sds_level2, out2dac);
				//WriteData2file(out2dac.c_str(), (filename[0] + file_suffix).c_str(), g_chq_data_dir); 
				Mds *mds = new Mds();
				string message(sds_level2.Code, sizeof(sds_level2.Code));
				std::vector<std::string> info_spit;
				split(message, ".", &info_spit);
				mds->Put(info_spit[1].c_str(), info_spit[0].c_str(), sds_level2.Seqno,sds_level2.Date, sds_level2.Time, &sds_level2, (size_t)sizeof(SDS_Level2));
				mds->Get(info_spit[1].c_str(), info_spit[0].c_str(), sds_level2.Seqno, sds_level2.Date, sds_level2.Time, &recvBuf, (size_t)sizeof(SDS_Level2));			
			}/*
				
			case Transaction:WriteData2file(smss.c_str(),(filename[1] + file_suffix).c_str(), g_chq_data_dir); break;
			case Order:WriteData2file(smss.c_str(), (filename[2] + file_suffix).c_str(), g_chq_data_dir); break;
			case OrderQueue:WriteData2file(smss.c_str(), (filename[3] + file_suffix).c_str(), g_chq_data_dir); break;*/

			WriteData2file(smss, (filename[0] + file_suffix).c_str(), g_chq_data_dir); //break			
			//default:break;
			//}
			
		//}
/*
		if (e.type() == kMessage){//根据过来的消息进行想
				
		
		}*/

	}

	return 0;
}

QueryServiceActor::QueryServiceActor(std::string id_) : Actor(id_)
{
}
int QueryServiceActor::OnStart(ActorContext&) {
	return 0;
}

int  QueryServiceActor::OnEvent(Event& e) {
	std::string smss;
	if (e.type() == kMessage) {
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
			
			if (!documentprase.HasMember("Seqno") || !documentprase["Seqno"].IsInt())
				return -1;
			int Seqno = documentprase["Seqno"].GetInt();
			//根据头，去调用不同的actor进行相应的处理
			/*switch (Seqno){
			case SubscribeMarketData:	
				e.Forward("con1", "sub_market_data_resp", cmss); break;
            //Query interface
			case GetMarketInfo: e.Forward("con1", "market_info_resp", "mseeage"); break;
			case GetStockInfo:e.Forward("con1", "stock_info_resp", "mseeage"); break;
			case GetMarketData:e.Forward("con1", "market_data_resp", "mseeage"); break;
			case GetIndicator:
			case GetMarketRank:				
			defaut:break;			
			}*/
			
			
		}

	}

	return 0;
}
 TransStoreActor::TransStoreActor(std::string id_) : Actor(id_)
{
}
 int TransStoreActor::OnStart(ActorContext&){
	 Subscribe("subfromsource","10");
 
	AddTimer(4*60*60*1000);//定时每4
	 //AddTimer( 60*1000);//定时每4个小时触发一次
	 return 0;
 }
 int TransStoreActor::OnEvent(Event&e){
	 string  message;
	 if (e.type() == kPublish)
	 {//收到闭市场消息
		 message = e.message();
		 SDS_CloseMarket close_market;
		 Parase2Object(message, close_market);
		 if (close_market.Time >= 143000000 || close_market.Time==0)///闭市  转移文件  上海16点闭市
		 {
			 //string goal_path = g_cpersistent_hq_dir + date_time.date;
			 for (auto file : filename)
			 {
				 string source = file + ".txt";
				 TransferFile(file.c_str(), file.c_str(), g_cpersistent_hq_dir, g_chq_data_dir);
			 }

			 //转移完成后要重置文件名
			 DateAndTime start_time = GetDateAndTime();//初始化系统时间
			 sstart_time = itostring(start_time.date) + itostring(start_time.time);
			 filename[0] = "level2"; filename[1] = "transaction"; filename[2] = "order"; filename[3] = "orderqueue";
			 for (auto &file : filename)//初始化存储文件名
			 { 
				 file = sstart_time + file;
			 }




		 }


	 }
 
	 if (e.type() == kTimer)
	 {
		//获取当前时间与设置的数据库存储时间相比较把，相应记录存储到数据库

		 //1、设置转存时间  
		// 时间到的话
		
		 DateAndTime date_time = { 0, 0 };
		 date_time = GetDateAndTime();
		 if (150000000< date_time.time)//闭市后
		 {
   //1、把数据导入数据库



   //2、文件转到其他目录  读文件 再写到其他地方 
			 string goal_path = g_cpersistent_hq_dir+itostring(date_time.date);
			 for (auto file:filename)
			 { 
				 string source = file + ".txt";

				 TransferFile(file.c_str(), file.c_str(), goal_path, g_chq_data_dir);
			 }
   
   //3、删除内存数据库
			 bool a = g_memorydb.clear();
			// GetKCAllKeyValue(g_memorydb, "leve2.kch");
		
   

		 }
		 
		 		 				 

	 }
 
	 return 0;
 
 }
