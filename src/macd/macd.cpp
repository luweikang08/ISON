#include <iostream>
#include "isonbase.h"
#include "iblog.h"
#include "context.h"
#include "stage.h"
#include "actor.h"
#include "event.h"
#include "ibprotocol.h"

#include "ta_common.h"
#include "ta_func.h"
#include "math.h"
#include <fstream>
#include <time.h>
#include "windows.h"

using namespace ison::base;
using namespace std;

const string g_str_connid = "con1";
const string g_str_endpoint = "tcp://127.0.0.1:7799";

//const char * g_str_filepath = "result.txt";
const char* g_str_subendpoint = "tcp://192.168.15.200:7799";
const char* g_str_subid = "sub1";

FILE* g_outputfile;
time_t g_timenow;
int g_count;
SYSTEMTIME m_time;
char g_chtimenow[16];

TA_Real GetPrice(string* Signal)
{

}

class ReplyActor : public Actor {
public:
	ReplyActor() : Actor("test") {}
	int OnStart(ActorContext&) {
		Subscribe(g_str_subid, "83");
		cout << "test start ok" << endl;
		//time(&g_timenow);
		GetSystemTime(&m_time);
		sprintf(g_chtimenow, "%d-%d-%d %d:%d:%d.%d", m_time.wYear, m_time.wMonth, m_time.wDay, m_time.wHour + 8, m_time.wMinute, m_time.wSecond, m_time.wMilliseconds);
		fprintf(g_outputfile, "start time:%s\n", g_chtimenow);
		return 0;
	}
	int OnEvent(Event& e) {
		g_count++;
		cout << e.message() << endl;
		fprintf(g_outputfile, "data:%s\n", e.message());
		cout << "test OnEvent ok!" << endl;
		//e.Reply("ok!");
		if (g_count >= 1000)
		{
			time(&g_timenow);
			fprintf(g_outputfile,"stop time:%s\n", ctime(&g_timenow));
			fclose(g_outputfile);
			g_count = 0;
			system("pause");

		}
		return 0;
	}
};

int main(int argc, char* argv[]) {
	iblog_init(argv[0]);
	iblog_v(0);//自定义VLOG(v)时，v值小于此处设置值的语句才有输出

	Context ctx;
	Stage stage(ctx);

	TA_Real    closePrice[360];
	TA_Real    out[51];
	TA_Integer outBeg;
	TA_Integer outNbElement;
	TA_Real    outMACD[51];
	TA_Real    outMACDSignal[51];
	TA_Real    outMACDHist[51];

	g_outputfile = fopen("out.txt", "a+");
	g_count = 0;

	stage.Bind("tcp://*:7799");
	stage.AddSubscriber(g_str_subid, g_str_subendpoint);
	ActorPtr actor(new ReplyActor());
	stage.AddActor(actor);
	//stage.Connect(g_str_connid, g_str_subendpoint);
	stage.Start();
	stage.Join();
	return 0;
}