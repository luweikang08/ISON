#include <iostream>
#include "isonbase.h"
#include "iblog.h"
#include "context.h"
#include "stage.h"
#include "actor.h"
#include "event.h"
#include "ibprotocol.h"

#include <fstream>
#include <time.h>
#include "windows.h"

#include "MessageHeader.hpp"
#include "SDS_Level2.hpp"

using namespace ison::base;
using namespace std;

const char* g_str_pubid = "pub1";
const char* g_str_pubendpoint = "tcp://127.0.0.1:1212";
const char* g_str_pubtopic = "88";

const char* g_str_subtopic = "1112";
const char* g_str_subendpoint = "tcp://192.168.15.200:7788";
const char* g_str_subid = "sub1";

FILE*      g_file_outputfile;         //输出文本文件，用于测试
int        g_num_count = 0;              //计算接收次数，用于计算avg
SYSTEMTIME g_time;               //输出时间，用于测试g_nSum
char       g_chtimenow[20];      //输出时间，用于测试
char       buffer[1024];

class PubActor : public Actor {
public:
	PubActor(const std::string& id) : Actor(id) {}
	int OnStart(ActorContext&)
	{
		AddTimer(1000);
		cout << "test start ok" << endl;
		GetSystemTime(&g_time);
		sprintf_s(g_chtimenow, "%d%d%d %d%d%d %d", g_time.wYear, g_time.wMonth, g_time.wDay, g_time.wHour + 8, g_time.wMinute, g_time.wSecond, g_time.wMilliseconds);
		fprintf(g_file_outputfile, "sub start time:%s\n", g_chtimenow);
		return 0;
	}
	int OnEvent(Event& e)
	{
		string smss;
		int m_num_sdstime = 0, m_num_atradeintime = 0, m_num_localtime = 0, m_num_timecostOne = 0, m_num_timecostTwo = 0;
		//smss = e.message();

		GetSystemTime(&g_time);
		sprintf_s(g_chtimenow, "%d%d%d %d%d%d %d", g_time.wYear, g_time.wMonth, g_time.wDay, g_time.wHour + 8, g_time.wMinute, g_time.wSecond, g_time.wMilliseconds);

		baseline::MessageHeader hdr;
		baseline::SDS_Level2 Level2data;
		hdr.wrap(buffer, 0, 0, 1024)
			.blockLength(baseline::SDS_Level2::sbeBlockLength())
			.templateId(baseline::SDS_Level2::sbeTemplateId())
			.schemaId(baseline::SDS_Level2::sbeSchemaId())
			.version(baseline::SDS_Level2::sbeSchemaVersion());	
		Level2data.wrapForEncode(buffer, sizeof(baseline::MessageHeader), 1024)
			.seqno(4095905)
			.putCode("600446.SH")
			.date(20150106)
			.time(102304123)
			.sDSTime(103204123)
			.sn(4095930)
			.preClose(59480)
			.open(55790)
			.high(61100)
			.low(58800)
			.price(59900)
			.numTrades(2238)
			.volume(13300)
			.turnover(17181913)
			.totalBidVol(5505500)
			.totalAskVol(3612273)
			.avgBidPrice(58900)
			.avgAskPrice(59900)
			.highLimited(61200)
			.lowLimited(57700);
			/*.putAskPrice("526500\0528000\0528500\0529800\0529900\0530000\0530500\0531000\0531300\0533000")
			.putAskVol("200\02100\02400\04500\05000\0400\012500\020200\0165\026500")
			.putBidPrice("526000\0525100\0525000\0524900\0524600\0524500\0524400\0524300\0524200\0524100")
			.putBidVol("11157\0959\04200\018466\0400\0700\0200\02200\01300\0600");*/

		string message(buffer, 1024);

		if (Publish(g_str_pubid, "88", message) != 0)
		{
			cout << "Pub fail." << endl;
		}
		printf("data:%s\n", buffer);
		return 0;
	}
};

class TimerActor : public Actor {
public:
	TimerActor(const std::string& id) : Actor(id) {}
	int OnStart(ActorContext&)
	{
		AddTimer(120000);
		return 0;
	}
	int OnEvent(Event& e)
	{
		return 0;
	}
};

int main(int argc, char* argv[]) {
	iblog_init(argv[0]);
	iblog_v(0);//自定义VLOG(v)时，v值小于此处设置值的语句才有输出

	Context ctx;
	Stage stage(ctx);

	string m_str_FilePath;
	GetSystemTime(&g_time);
	sprintf_s(g_chtimenow, "%d%d%d %d%d%d", g_time.wYear, g_time.wMonth, g_time.wDay, g_time.wHour + 8, g_time.wMinute, g_time.wSecond);
	m_str_FilePath.append("out");
	m_str_FilePath.append(g_chtimenow);
	m_str_FilePath.append(".txt");
	fopen_s(&g_file_outputfile, m_str_FilePath.c_str(), "a+");

	stage.Bind("tcp://*:5555");
	stage.AddSubscriber(g_str_subid, g_str_subendpoint);
	stage.AddPublisher(g_str_pubid, g_str_pubendpoint);
	ActorPtr actor(new PubActor("aa"));
	//ActorPtr timer(new TimerActor("timer"));
	stage.AddActor(actor);
	//stage.AddActor(timer);
	//stage.Connect(g_str_connid, g_str_subendpoint);
	stage.Start();
	stage.Join();
	return 0;
}