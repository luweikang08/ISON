#include <iostream>
#include "isonbase.h"
#include "iblog.h"
#include "context.h"
#include "stage.h"
#include "actor.h"
#include "event.h"
#include "ibprotocol.h"

#include "windows.h"

#include "json_analysis.h"
//#include "data_struct.h"

using namespace ison::base;
using namespace std;

const string g_str_connid = "con1";
const string g_str_endpoint = "tcp://127.0.0.1:7799";

const char*  g_str_subendpoint = "tcp://192.168.15.200:7799";
const char*  g_str_subid = "sub1";

FILE*      g_file_outputfile;    //输出文本文件，用于测试
SYSTEMTIME g_time;               //输出时间，用于测试g_nSum
char       g_chtimenow[20];      //输出时间，用于测试

MaketDataLevel2 MaketData;

class ReplyActor : public Actor {
public:
	ReplyActor(const std::string& id) : Actor(id) {}
	int OnStart(ActorContext&)
	{
		Subscribe(g_str_subid, "83");
		//Subscribe(g_str_subid, "600446");
		cout << "test start ok" << endl;
		//GetSystemTime(&g_time);
		//sprintf_s(g_chtimenow, "%d%d%d %d%d%d %d", g_time.wYear, g_time.wMonth, g_time.wDay, g_time.wHour + 8, g_time.wMinute, g_time.wSecond, g_time.wMilliseconds);
		//fprintf(g_file_outputfile, "start time:%s\n", g_chtimenow);
		return 0;
	}
	int OnEvent(Event& e)
	{
		string smss;
		if (e.type() == kPublish)
		{
			smss = e.message();

			ParseJsonToMaketDataLevel2(smss, MaketData);
			cout << MaketData.Time << endl;
			//fprintf(g_file_outputfile, "data:%s\n", e.message());
			/*GetSystemTime(&g_time);
			sprintf_s(g_chtimenow, "%d%d%d %d%d%d %d", g_time.wYear, g_time.wMonth, g_time.wDay, g_time.wHour + 8, g_time.wMinute, g_time.wSecond, g_time.wMilliseconds);
			fprintf(g_file_outputfile, "stop time:%s\n", g_chtimenow);
			fprintf(g_file_outputfile, "data:%s\n", e.message());*/
			//printf("data:%s\n", e.message());
		}

		return 0;
	}
};

int main(int argc, char* argv[]) {
	iblog_init(argv[0]);
	iblog_v(0);//自定义VLOG(v)时，v值小于此处设置值的语句才有输出

	Context ctx;
	Stage stage(ctx);

	//string m_str_FilePath;
	//GetSystemTime(&g_time);
	//sprintf_s(g_chtimenow, "%d%d%d %d%d%d", g_time.wYear, g_time.wMonth, g_time.wDay, g_time.wHour + 8, g_time.wMinute, g_time.wSecond);
	//m_str_FilePath.append("out");
	//m_str_FilePath.append(g_chtimenow);
	//m_str_FilePath.append(".txt");
	//fopen_s(&g_file_outputfile, m_str_FilePath.c_str(), "a+");

	stage.Bind("tcp://*:5555");
	stage.AddSubscriber(g_str_subid, g_str_subendpoint);
	ActorPtr actor(new ReplyActor("aa"));
	stage.AddActor(actor);
	//stage.Connect(g_str_connid, g_str_subendpoint);
	stage.Start();
	stage.Join();
	return 0;
}