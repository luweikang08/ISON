
#include <iostream>
#include <string>
#include<vector>
#include <map>
#include<fstream>


#include "isonbase.h"
#include "iblog.h"
#include "context.h"
#include "stage.h"
#include "actor.h"
#include "event.h"
#include "ibprotocol.h"
#include "socket.h"
#include"message.h"

#include "libckmdsapi/ckmdsuserapi.h"
#include"filehelper.h"
#include"ibsdstool.h"
#include "kmdsdataapidef.h"
#include "timehelper.h"
#include "sdsversion.h"
using namespace ison::base;
Socket  *g_socket;//全局Socket，用于接受KMDS接口PUSH的数据
Context ctx;
//KMDSUSERAPIHANDLE pclient;
//map<std::string, HQ_CODETABLE>  g_codetablemap;//全局变量，用于存储代码表
//extern void  OnKmdsMsg(void* pmsgdesc, void* pmsgopts, int ncmd, MSGDATAHANDLE pmsg);
//extern int  KMDSCodeTableReq(int mktcode, map<std::string, HQ_CODETABLE> &g_codetablemap);
extern int GetDataFromKMDS();
extern void GetDataFromTDF();
extern int  ReadConfig(const char* filename, std::vector<std::string> configkey, map<std::string, std::string> &configmap);
 string g_clog_dir = "./atradesdslog";//日志路径
//const char* g_chq_server = "tcp://127.0.0.1:7788";//对外的pub端口
 string g_source2sds ="inproc://end1";
int g_nlog_level = 1;
int g_nhq_source = 1;//配置行情源 1 ：表示是金证  2 ：表示是宏汇
int g_tdf_time_sn = 1;//0的话记录会加入宏汇时间以及序列号，非0的话不会加入宏汇时间及序列号
string g_cfilter_code_file = "./stockid.txt";
bool filter_code_flag = 0;
std::vector<std::string> code_filter;//过滤股票id
//序列号
int transaction_count = 0;
int level2_count = 0;
int oder_count = 0;
int oder_queue_count = 0;
std::string g_version ;

 void  SdsSourceInit();

int main(int argc, char* argv[]) {
	SdsSourceInit();
	iblog_init(argv[0], g_clog_dir.c_str());
	iblog_v(0);//自定义VLOG(v)时，v值小于此处设置值的语句才有输出
	iblog_stderrthreshold(2);
	LOG_IF(INFO,g_nlog_level>5) << "SDS source run  begin\n";
	g_version=ison::sds::version_string();
	std::cout<<g_version;
	g_socket = new Socket(ctx, SocketType::publish);//Create the Socket which transmit the publish Data from KMDS to the Actor of SDS！
	g_socket->Bind(g_source2sds.c_str());
	//zsock_set_sndhwm(g_socket->self(),10000000);

	/*                                            //暂时不需要Stage 
	Stage sds_source_stage(ctx);//
	sds_source_stage.Bind("tcp://*:5555");  //对外的监听端口
	sds_source_stage.AddSubscriber("sub1", g_source2sds);//订阅
	sds_source_stage.AddPublisher("pub1", g_chq_server);//向Client推送的端口
	ActorPtr hq_multicast_actor(new HQMulticastActor("HQMulticast"));//组播行情
	sds_source_stage.AddActor(hq_multicast_actor);
	sds_source_stage.Start();*/
	
	//GetDataFromTDF();
	
	//判断Source正常工作的方案
   //动态切换Source
	/*do
	{
	GetDataFromTDF();//先从宏汇处获得数据
	} while (0);*/
	switch (g_nhq_source){
	case 1:
		GetDataFromKMDS(); break;//从KMDS处获得数据
	case 2:
	{//从宏汇处获得数据
		
		GetDataFromTDF(); break;
		
	}
	default: GetDataFromTDF();
	}
	//sds_source_stage.Join();	
	return 0;
}

 void SdsSourceInit(){
	//Read config file!
	std::vector<std::string> configkey = { "LogDir", "LogLevel", "HQSource", "Source2SDS", "SourceTimeAndSn", "FilterFile" };
	map<std::string, std::string> configmap;
	ReadConfig("./sdssource.json", configkey, configmap);//sds配置文件
	g_clog_dir = configmap["LogDir"].c_str();
	//g_chq_server = configmap["HQServer"].c_str();
	g_nlog_level = atoi(configmap["LogLevel"].c_str());
	g_nhq_source = atoi(configmap["HQSource"].c_str());
	g_source2sds = configmap["Source2SDS"];
	g_cfilter_code_file = configmap["FilterFile"];
	g_tdf_time_sn = atoi(configmap["SourceTimeAndSn"].c_str());
	//Init the filter codetable!
	if ((std::string)g_cfilter_code_file != " ") filter_code_flag = 1;
	if (filter_code_flag) {
		int ret = ReadFilterCode(code_filter, g_cfilter_code_file);
		if (ret != 0)std::cout << "Open  filter file " << g_cfilter_code_file << " fail!" << std::endl;
		LOG(INFO) << "Open  filter file " << g_cfilter_code_file << " fail!";
		filter_code_flag = 0;
	}	
	unsigned int sn;
	SnInit(sn);//初始化sn
	transaction_count = level2_count = oder_count = oder_queue_count = sn;

}
