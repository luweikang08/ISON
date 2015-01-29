#include  <iostream>
#include  <string>
#include  <vector>
#include  <map>
#include  <fstream>
#include  "isonbase.h"
#include  "iblog.h"
#include  "context.h"
#include  "stage.h"
#include  "actor.h"
#include  "event.h"
#include  "ibprotocol.h"
#include  "socket.h"
#include  "message.h"
#include  "kcpolydb.h"
#include  "sdsstore_actor.h"
#include  "timehelper.h"
#include  "reqresp_actor.h"
#include  "memorydata.h"
#include  "filehelper.h"
#include "ibsdstool.h"
#include  "bpt.h"
//#include "memorydata.cpp"

using namespace ison::base;

//using namespace kyotocabinet;
string g_clog_dir = "./sdsstore.log";//the log path

string g_chq_data_dir = "./sdsmemorystore";//The memory_data dir
string g_cpersistent_hq_dir = "./sdsfilestore";//Trans_store the memory_data dir

int g_nlog_level = 1;
string g_cfile_store = "./filestore";
string g_cmemory_store_bind = "tcp://*:5566";
string g_cmemory_store_conid = "con1";
//string g_cmemory_store_conip = "tcp://127.0.0.1:5566";
string g_cmemory_store_conip = "192.168.15.200:2010";
string g_cpersistent_bind = "tcp://*:5577";
//string g_csource2store = "tcp://127.0.0.1:7799";
string g_csource2store = "192.168.15.200:2010";


//define 4 global object for Level2 、Transaction 、 Order 、OrderQueen  add 2  Level2Basic Level2Price
 MemoryData< SDS_Level2 > g_level2;
 MemoryData< SDS_Transaction > g_transaction;
 MemoryData<SDS_Order> g_order;
 MemoryData<SDS_OderQueen> g_order_queen;
 std::vector<int> level2_delay;
 std::vector<int> transaction_delay;
 std::string  sstart_time = "";
 std::string filename[4] = { "level2", "transaction", "order", "orderqueue" };
kyotocabinet::PolyDB g_memorydb;
extern int  ReadConfig(const char* filename, std::vector<std::string> configkey, std::map<std::string, std::string> &configmap);
extern int ReadFileKeyValue2KC(kyotocabinet::PolyDB &db, const char * table, const char* fliename);
//extern int CreatDir(const char *pszDir);
void SdsStoreInit();

int main(int argc, char* argv[]) {
	SdsStoreInit();
    Context ctx;
    iblog_init(argv[0], g_clog_dir.c_str());
	iblog_v(0);//自定义VLOG(v)时，v值小于此处设置值的语句才有输出
	iblog_stderrthreshold(2);
	LOG_IF(INFO, g_nlog_level>5) << "SDS Store run  begin\n";
	Stage memory_store_stage(ctx);//数据接入Stage,写内存数据库
	Stage persistent_stage(ctx);//持久化到文件中
	/*
	//Test B+tree
	bpt::bplus_tree database("test.txt", true);
	for (int i = 1; i <= 10; i=i+2) {
		bpt::bplus_tree database("test.txt");
		// if (i % 1000 == 0)
		int   data = 1211130 + i * 100 + i;
		std::string sdate = itostring(data);
		const char * temp = sdate.c_str();
		
		printf("%d\n", i);
		char key[16] = { 0 };
		sprintf(key, "%s", temp);


		//sprintf(key, "%d", i);
		database.insert(key, i);
		int  value;
		if (database.search(key, &value) != 0)
			printf("Key %s not found\n", value);
		else printf("value%d\n", value);
	}*/

	memory_store_stage.Bind(g_cmemory_store_bind);  //对外的监听端口
	memory_store_stage.Connect(g_cmemory_store_conid, g_cmemory_store_conip);

	memory_store_stage.AddSubscriber("subfromsource", g_csource2store);//订阅发布出来的行情
	ActorPtr amemory_store(new MemoryStoreActor("MemoryStore"));//把行情存内存数据库（四个文件）
	ActorPtr amarket_data(new SubMarketDataResp("sub_market_data_resp"));

	
	persistent_stage.Bind(g_cpersistent_bind);  //对外的监听端口
	persistent_stage.AddSubscriber("subfromsource", g_csource2store);//订阅发布出来的行情

	ActorPtr apersistent_store(new PersistentStoreActor("PersistentStore"));//把行情持久化到文件（四个文件）
	//ActorPtr aquery_service(new QueryServiceActor("query"));
	//ActorPtr atrans_store(new TransStoreActor("Timer"));//进行一些定时操作，如每天清除内存数据库中的记录，转移文件等
	

	memory_store_stage.AddActor(amemory_store);
	//memory_store_stage.AddActor(aquery_service);
	memory_store_stage.AddActor(amarket_data);


	persistent_stage.AddActor(apersistent_store);
//	persistent_stage.AddActor(atrans_store);

	memory_store_stage.Start();
	persistent_stage.Start();
	memory_store_stage.Join();
	persistent_stage.Join();	
	return 0;
}

void SdsStoreInit()
{
	
//Read configfile!
std::vector<std::string> configkey = { "LogDir", "HqDataDir", "TransHqDir", "LogLevel", "Source2Store", "MemoryStoreBind"
, "MemoryStoreConId", "MemoryStoreConIP", "PersistentBind" };
std::map<std::string, std::string> configmap;
ReadConfig("./sdsstore.json", configkey, configmap);//sds配置文件
g_clog_dir = configmap["LogDir"];
g_chq_data_dir = configmap["HqDataDir"];
g_cpersistent_hq_dir = configmap["TransHqDir"];
g_nlog_level = atoi(configmap["LogLevel"].c_str());
g_csource2store = configmap["Source2Store"].c_str();
g_cmemory_store_bind = configmap["MemoryStoreBind"];
g_cmemory_store_conid = configmap["MemoryStoreConId"];
g_cmemory_store_conip = configmap["MemoryStoreConIP"];
g_cpersistent_bind = configmap["PersistentBind"];

//系统启动后记录一个时间，用于生成文件，一天启动多次的话形成的是多个文件
DateAndTime start_time = GetDateAndTime();//初始化系统时间
sstart_time = itostring(start_time.date) + itostring(start_time.time);
 for (auto &file : filename)//初始化存储文件名
  {
	file = sstart_time + file;
  }
}
