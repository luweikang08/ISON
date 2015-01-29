
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
Socket  *g_socket;//ȫ��Socket�����ڽ���KMDS�ӿ�PUSH������
Context ctx;
//KMDSUSERAPIHANDLE pclient;
//map<std::string, HQ_CODETABLE>  g_codetablemap;//ȫ�ֱ��������ڴ洢�����
//extern void  OnKmdsMsg(void* pmsgdesc, void* pmsgopts, int ncmd, MSGDATAHANDLE pmsg);
//extern int  KMDSCodeTableReq(int mktcode, map<std::string, HQ_CODETABLE> &g_codetablemap);
extern int GetDataFromKMDS();
extern void GetDataFromTDF();
extern int  ReadConfig(const char* filename, std::vector<std::string> configkey, map<std::string, std::string> &configmap);
 string g_clog_dir = "./atradesdslog";//��־·��
//const char* g_chq_server = "tcp://127.0.0.1:7788";//�����pub�˿�
 string g_source2sds ="inproc://end1";
int g_nlog_level = 1;
int g_nhq_source = 1;//��������Դ 1 ����ʾ�ǽ�֤  2 ����ʾ�Ǻ��
int g_tdf_time_sn = 1;//0�Ļ���¼�������ʱ���Լ����кţ���0�Ļ����������ʱ�估���к�
string g_cfilter_code_file = "./stockid.txt";
bool filter_code_flag = 0;
std::vector<std::string> code_filter;//���˹�Ʊid
//���к�
int transaction_count = 0;
int level2_count = 0;
int oder_count = 0;
int oder_queue_count = 0;
std::string g_version ;

 void  SdsSourceInit();

int main(int argc, char* argv[]) {
	SdsSourceInit();
	iblog_init(argv[0], g_clog_dir.c_str());
	iblog_v(0);//�Զ���VLOG(v)ʱ��vֵС�ڴ˴�����ֵ�����������
	iblog_stderrthreshold(2);
	LOG_IF(INFO,g_nlog_level>5) << "SDS source run  begin\n";
	g_version=ison::sds::version_string();
	std::cout<<g_version;
	g_socket = new Socket(ctx, SocketType::publish);//Create the Socket which transmit the publish Data from KMDS to the Actor of SDS��
	g_socket->Bind(g_source2sds.c_str());
	//zsock_set_sndhwm(g_socket->self(),10000000);

	/*                                            //��ʱ����ҪStage 
	Stage sds_source_stage(ctx);//
	sds_source_stage.Bind("tcp://*:5555");  //����ļ����˿�
	sds_source_stage.AddSubscriber("sub1", g_source2sds);//����
	sds_source_stage.AddPublisher("pub1", g_chq_server);//��Client���͵Ķ˿�
	ActorPtr hq_multicast_actor(new HQMulticastActor("HQMulticast"));//�鲥����
	sds_source_stage.AddActor(hq_multicast_actor);
	sds_source_stage.Start();*/
	
	//GetDataFromTDF();
	
	//�ж�Source���������ķ���
   //��̬�л�Source
	/*do
	{
	GetDataFromTDF();//�ȴӺ�㴦�������
	} while (0);*/
	switch (g_nhq_source){
	case 1:
		GetDataFromKMDS(); break;//��KMDS���������
	case 2:
	{//�Ӻ�㴦�������
		
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
	ReadConfig("./sdssource.json", configkey, configmap);//sds�����ļ�
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
	SnInit(sn);//��ʼ��sn
	transaction_count = level2_count = oder_count = oder_queue_count = sn;

}
