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
//#include"jsonhelper.h"
#include"testactor.h"

#define g_str_subid "sub1";

using namespace ison::base;
const char* g_clog_dir = "./atradesdslog";//��־·��
const char* g_chq_server = "tcp://127.0.0.1:7789";//�����pub�˿�
const char* g_sub4sds_source = "tcp://192.168.15.200:7799";
int  g_nlog_level = 1;
int windowmin = 5;

int main(int argc, char *argv[]){
	iblog_init(argv[0], g_clog_dir);
	iblog_v(0);//�Զ���VLOG(v)ʱ��vֵС�ڴ˴�����ֵ�����������
	iblog_stderrthreshold(2);
	LOG_IF(INFO, g_nlog_level > 5) << "Test run  begin\n";
	Context ctx;
	Stage sourcetest_stage(ctx);
	sourcetest_stage.Bind("tcp://*:5555");  //����ļ����˿�
	sourcetest_stage.AddSubscriber("sub1", g_sub4sds_source);//����

	ActorPtr level2delay(new Level2TDFDealay ("level2"));
	sourcetest_stage.AddActor(level2delay);

	ActorPtr transdelay(new TransTDFDealay("transdelay"));
	sourcetest_stage.AddActor(transdelay);

	ActorPtr orderqueuedelay(new  OrderQueueTDFDealay("orderqueuedelay"));
	sourcetest_stage.AddActor(orderqueuedelay);


	ActorPtr caculatedelay(new CaculateTDFDealay("caculatedelay"));//������ʱ�Ͷ�ʧ��������
	sourcetest_stage.AddActor(caculatedelay);
	sourcetest_stage.Start();
	sourcetest_stage.Join();
	return 0;
}

//����һЩ����
