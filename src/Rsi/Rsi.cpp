#include "Rsi.h"

using namespace ison::base;
using namespace std;

const char* g_str_subendpoint = "tcp://192.168.15.200:7799";
const char* g_str_subid = "sub1";
const char* g_str_topic = "83";
const char* g_str_pubendpoint = "tcp://127.0.0.1:1212";
const char* g_str_pubid = "pub1";
const char* g_str_pubtopic = "88";

FILE* g_file_outputfile;
SYSTEMTIME g_time;
char g_chtimenow[20];

SDS_Level2 g_Level2Data;
queue<double> g_num_PriceQueue;
RsiData g_RsiData;

class SubActor : public Actor {
public:
	SubActor(const std::string& id) : Actor(id) {}
	int OnStart(ActorContext&) {
		Subscribe(g_str_subid, g_str_topic);
		//AddTimer(1000);
		cout << "test start ok" << endl;
		//time(&g_timenow);
		GetSystemTime(&g_time);
		sprintf_s(g_chtimenow, "%d%d%d %d%d%d %d", g_time.wYear, g_time.wMonth, g_time.wDay, g_time.wHour + 8, g_time.wMinute, g_time.wSecond, g_time.wMilliseconds);
		fprintf(g_file_outputfile, "start time:%s\n", g_chtimenow);
		return 0;
	}
	int OnEvent(Event& e) {
		string smss;
		string StockCode;
		smss = e.message();
		if (Parase2Object(smss, g_Level2Data) == 0) //parase success
		{
			StockCode.append(g_Level2Data.Code);
			if ((StockCode == "000001.SZ") && (g_Level2Data.Price > 0))//if (g_Level2Data.Code == "600446.SH")
			{
				if (g_num_PriceQueue.size() < 24)
				{
					g_num_PriceQueue.push((double)g_Level2Data.Price);
					printf("size:%d\n", g_num_PriceQueue.size());
				}
				else   //queue's size is 50,calculate macd
				{
					g_num_PriceQueue.pop();
					g_num_PriceQueue.push((double)g_Level2Data.Price);
					strcpy_s(g_RsiData.Code, g_Level2Data.Code);
					g_RsiData.Date = g_Level2Data.Date;
					g_RsiData.Time = g_Level2Data.Time;
					CalcRsiResult(g_num_PriceQueue, g_RsiData.Rsi6, g_RsiData.Rsi12, g_RsiData.Rsi24);

					string m_str_message;
					RsiData2String(g_RsiData, m_str_message);
					fprintf(g_file_outputfile, "data:%s\n", m_str_message);
					if (Publish(g_str_pubid, g_str_pubtopic, m_str_message) == 0)
					{
						printf("publish success.\n");
						return 0;
					}
					else
					{
						printf("publish fail.\n");
						return -1;
					}
				}
			}
		}
	}
};

int main(int argc, char* argv[]) 
{
	Context ctx;
	Stage stage(ctx);

	string m_str_FilePath;
	GetSystemTime(&g_time);
	sprintf_s(g_chtimenow, "%d%d%d %d%d%d", g_time.wYear, g_time.wMonth, g_time.wDay, g_time.wHour + 8, g_time.wMinute, g_time.wSecond);
	m_str_FilePath.append("out");
	m_str_FilePath.append(g_chtimenow);
	m_str_FilePath.append(".txt");
	fopen_s(&g_file_outputfile, m_str_FilePath.c_str(), "a+");

	stage.Bind("tcp://*:7799");
	stage.AddSubscriber(g_str_subid, g_str_subendpoint);
	stage.AddPublisher(g_str_pubid, g_str_pubendpoint);
	ActorPtr actor(new SubActor("pub"));
	stage.AddActor(actor);
	//stage.Connect(g_str_connid, g_str_subendpoint);
	stage.Start();
	stage.Join();
	return 0;
}