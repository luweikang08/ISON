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

#include"jsonhelper.h"
#include"sds2atradeactor.h"
using namespace ison::base;

string g_clog_dir = "./atradesdslog";//日志路径
string g_chq_server = "tcp://127.0.0.1:7789";//对外的pub端口
string g_sub4sds_source = "tcp://127.0.0.1:7799";

string g_chq_reqpub = "tcp://127.0.0.1:7788";//对外的pub端口
string sds2atrade_bind = "tcp://*:5555";
string sdsreq_bind = "tcp://*:5566";

int  g_nlog_level = 1;
int  g_nisfilter = 0;

void Sds2Atrade();
//extern map<std::string, TDF_CODE>  g_codetablepdfmap;//存储

int main(int argc, char *argv[]){
	Sds2Atrade();
	iblog_init(argv[0], g_clog_dir.c_str());
	iblog_v(0);//自定义VLOG(v)时，v值小于此处设置值的语句才有输出
	iblog_stderrthreshold(2);
	std::cout << "123" << endl;
	LOG_IF(INFO, g_nlog_level>5) << "SDS run  begin\n";
	//Get the code table !
	Context ctx;
	Stage sds2atrade_stage(ctx);//ISON SDS HQ PUB 2 Atrade
	Stage sdsreq_stage(ctx);

	sds2atrade_stage.Bind(sds2atrade_bind);  //对外的监听端口
	sdsreq_stage.Bind(sdsreq_bind);

	sds2atrade_stage.AddSubscriber("sub1", g_sub4sds_source);//订阅
	sds2atrade_stage.AddPublisher("pub1", g_chq_server);//向Client推送的端口

	sdsreq_stage.AddPublisher("pub2", g_chq_reqpub);//向Client推送的端口
	

	ActorPtr acode_table(new SubCodeTable("CodeTable"));//组播行情
	ActorPtr alevel2_multicast(new Level22Atrade("Level2ToAtrade"));//组播行情
	ActorPtr asellintime_multicast(new SellInTime2Atrade("SellInTime2Atrade"));//组播行情
	ActorPtr alevel2_req(new Level2Req("Level2Req"));//
	ActorPtr aclose_price_req(new ClosePriceReq("ClosePriceReq"));

	sds2atrade_stage.AddActor(acode_table);
	sds2atrade_stage.AddActor(alevel2_multicast);
	sds2atrade_stage.AddActor(asellintime_multicast);
	sdsreq_stage.AddActor(alevel2_req);
	sdsreq_stage.AddActor(aclose_price_req);
	sds2atrade_stage.Start();
	sdsreq_stage.Start();
	sds2atrade_stage.Join();
	sdsreq_stage.Join();
}

void Sds2Atrade(){
	std::vector<std::string> configkey = { "LogDir", "HQServer", "LogLevel", "Source2Atrade","Level2ReqPub",
		"Sds2AtradeBind","Level2ReqBind"};
	std::map<std::string, std::string> configmap;
	ReadConfig("./sds2atrade.json", configkey, configmap);//Read the config file
	g_clog_dir = configmap["LogDir"];//
	g_nlog_level = atoi(configmap["LogLevel"].c_str());
	g_sub4sds_source = configmap["Source2Atrade"];//Sub HQ from source
	g_chq_server = configmap["HQServer"];//Pub HQ 2 atrade 

	g_chq_reqpub = configmap["Level2ReqPub"];
	sds2atrade_bind = configmap["Sds2AtradeBind"];
	sdsreq_bind = configmap["Level2ReqBind"];
	}



