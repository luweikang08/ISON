#include <vector>
#include<string>
#include<map>

#include "isonbase.h"
#include "iblog.h"
#include "context.h"
#include "stage.h"
#include "actor.h"
#include "event.h"
#include "ibprotocol.h"
#include "socket.h"
#include"message.h"
#include "interfacetrans.h"
#include "isonsdsdataapi.h"

#include"jsonhelper.h"
using namespace ison::base;

string g_log_dir = "./sdslog";//日志路径
int g_log_level = 1;
string g_source_sub = "tcp://127.0.0.1:7799";
string g_dest_pub = "tcp://127.0.0.1:2004";//对外的pub端口
int g_pub_topic = 1;
int g_count = 0;
void Init();

int main(int argc, char *argv[]){
	Init();
	iblog_init(argv[0], g_log_dir.c_str());
	iblog_v(0);//自定义VLOG(v)时，v值小于此处设置值的语句才有输出
	iblog_stderrthreshold(2);
	LOG(INFO) << "SDS InterfaceTrans running  ......\n";
	Context ctx;
	Stage sbe2json_stage(ctx);

	sbe2json_stage.AddSubscriber("sub", g_source_sub);
	LOG(INFO) << "Sub endpoint is :" << g_source_sub.c_str() << "\n";
	sbe2json_stage.AddPublisher("pub", g_dest_pub);
	LOG(INFO) << "Pub endpoint is :" << g_source_sub.c_str() << "\n";

	ActorPtr asbe2jsonlevel2(new Sbe2JsonLevel2("sbe2jsonlevel2"));
	ActorPtr asbe2jsontrans(new Sbe2JsonTrans("sbe2josntrans"));
	ActorPtr asbe2jsonqueue(new Sbe2JsonQueue("sbe2jsonqueue"));

	sbe2json_stage.AddActor(asbe2jsonlevel2);
	sbe2json_stage.AddActor(asbe2jsontrans);
	sbe2json_stage.AddActor(asbe2jsonqueue);
	
	sbe2json_stage.Start();
	sbe2json_stage.Join();
	LOG(INFO) << "SDS InterfaceTrans stoping!!!";
}

void Init(){
	std::vector<std::string> configkey = {"LogDir", "LogLevel", "SourceSub","DestPub","PubTopic"};
	std::map<std::string, std::string> configmap;
	ReadConfig("./sdsjson.json", configkey, configmap);//Read the config file
	g_log_dir = configmap["LogDir"];//
	g_log_level = atoi(configmap["LogLevel"].c_str());
	g_source_sub = configmap["SourceSub"];//Sub HQ from source
	g_dest_pub = configmap["DestPub"];
	g_pub_topic = atoi(configmap["PubTopic"].c_str());
	}



