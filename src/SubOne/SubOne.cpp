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
#include "rapidjson/document.h"

#define g_num_DataRecCntMax        30
#define g_num_AvgPricePreSecCntMax 240

using namespace ison::base;
using namespace std;
using namespace rapidjson;

const string g_str_connid = "con1";
const string g_str_endpoint = "tcp://127.0.0.1:7799";
const char*  g_str_subendpoint = "tcp://192.168.15.200:7788";
const char*  g_str_subid = "sub1";

FILE*      g_file_outputfile;         //输出文本文件，用于测试
int        g_num_count = 0;              //计算接收次数，用于计算avg
SYSTEMTIME g_time;               //输出时间，用于测试g_nSum
char       g_chtimenow[20];      //输出时间，用于测试

short      g_num_DataRecCnt = 0;         //保存当前一分钟内的数据计数
double     g_num_AvgPricePreSec[g_num_AvgPricePreSecCntMax] = { 0 }; //保存当天每一分钟的平均值
short      g_num_AvgPricePreSecCnt = 0;       //保存当天分钟平均值的计数

int      g_num_sumone = 0, g_num_minone = 0, g_num_maxone = 0;
int      g_num_sumtwo = 0, g_num_mintwo = 0, g_num_maxtwo = 0;

bool ParseTimeFromString(string head, const string Signal, int& sdstime, int& atradeintime)
{
	Document m_Document, m_m_Document;
	m_Document.Parse<0>(Signal.c_str());
	if (m_Document.HasParseError())
	{
		printf("GetParseError %s\n", m_Document.GetParseError());
		return false;
	}
	if (m_Document["head"].GetString() == head)
	{
		if (m_Document.HasMember("body"))//这个时候要保证m_Document是IsObject类型 最好是 if(d.Isobject() && d.HasMember("a"))  
		{
			rapidjson::Value &body = m_Document["body"];
			if (body.IsObject())
			{
				sdstime = body["sdstime"].GetInt();
				atradeintime = body["atradeintime"].GetInt();
				return true;
			}
		}
	}
	return false;
}

class ReplyActor : public Actor {
public:
	ReplyActor(const std::string& id) : Actor(id) {}
	int OnStart(ActorContext&)
	{
		Subscribe(g_str_subid, "600446");
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
		smss = e.message();

		GetSystemTime(&g_time);
		sprintf_s(g_chtimenow, "%d%d%d %d%d%d %d", g_time.wYear, g_time.wMonth, g_time.wDay, g_time.wHour + 8, g_time.wMinute, g_time.wSecond, g_time.wMilliseconds);
		
		//fprintf(g_file_outputfile, "data:%s\n", smss);
		fprintf(g_file_outputfile, "sub store time:%s\n", g_chtimenow);
		m_num_localtime = g_time.wMilliseconds + g_time.wSecond * 1000 + g_time.wMinute * 100000 + (g_time.wHour + 8) * 10000000;

		if (ParseTimeFromString("1009", smss, m_num_sdstime, m_num_atradeintime)==false)
		{
			return 0;
		}
		m_num_timecostOne = m_num_atradeintime - m_num_sdstime;
		m_num_timecostTwo = m_num_localtime - m_num_atradeintime;
		//first time cost
		g_num_sumone += m_num_timecostOne;
		if (g_num_minone == 0)
		{
			g_num_minone = m_num_timecostOne;
			g_num_maxone = m_num_timecostOne;
		}
		else
		{
			if (g_num_minone > m_num_timecostOne)
			{
				g_num_minone = m_num_timecostOne;
			}
			else if (g_num_maxone<m_num_timecostOne)
			{
				g_num_maxone = m_num_timecostOne;
			}
		}
		//second time cost
		g_num_sumtwo += m_num_timecostTwo;
		if (g_num_mintwo == 0)
		{
			g_num_mintwo = m_num_timecostTwo;
			g_num_maxtwo = m_num_timecostTwo;
		}
		else
		{
			if (g_num_mintwo > m_num_timecostTwo)
			{
				g_num_mintwo = m_num_timecostTwo;
			}
			else if (g_num_maxtwo<m_num_timecostTwo)
			{
				g_num_maxtwo = m_num_timecostTwo;
			}
		}
		g_num_count++;
		fprintf(g_file_outputfile, "timedata:sdstime:%d,atradetime:%d,localtime:%d\n", m_num_sdstime, m_num_atradeintime, m_num_localtime);
		//fprintf(g_file_outputfile, "timedata:One:%d,Two:%d,Min:%d,Max:%d,Sum:%d,Cnt:%d,Avg:%d\n", m_num_timecostOne, m_num_timecostTwo, g_num_mintwo, g_num_maxtwo, g_num_sumtwo, g_num_count, (g_num_sumtwo / g_num_count));
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
		if (g_num_count > 0)
		{
			fprintf(g_file_outputfile, "timeonedata:Min:%d,Max:%d,Sum:%d,Cnt:%d,Avg:%d\n", g_num_minone, g_num_maxone, g_num_sumone, g_num_count, (g_num_sumone / g_num_count));
			fprintf(g_file_outputfile, "timetwodata:Min:%d,Max:%d,Sum:%d,Cnt:%d,Avg:%d\n", g_num_mintwo, g_num_maxtwo, g_num_sumtwo, g_num_count, (g_num_sumtwo / g_num_count));
			printf("done calculate in timer event\n");
			g_num_mintwo = 0;
			g_num_maxtwo = 0;
			g_num_sumtwo = 0;
			g_num_count = 0;
		}
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
	fopen_s(&g_file_outputfile,m_str_FilePath.c_str(), "a+");

	stage.Bind("tcp://*:5555");
	stage.AddSubscriber(g_str_subid, g_str_subendpoint);
	ActorPtr actor(new ReplyActor("aa"));
	ActorPtr timer(new TimerActor("timer"));
	stage.AddActor(actor);
	stage.AddActor(timer);
	//stage.Connect(g_str_connid, g_str_subendpoint);
	stage.Start();
	stage.Join();
	return 0;
}