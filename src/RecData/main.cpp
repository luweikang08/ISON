#include "RecData.h"

using namespace ison::base;
using namespace std;

const string g_str_connid = "con1";
const string g_str_endpoint = "tcp://127.0.0.1:7799";
//const char * g_str_filepath = "result.txt";
const char*  g_str_subendpoint = "tcp://192.168.15.200:7799";
const char*  g_str_subid = "sub1";

const char*  g_str_pubendpoint = "tcp://127.0.0.1:1212";
const char*  g_str_pubid = "pub1";
const char*  g_str_pubtopic = "88";

FILE*      g_file_outputfile;         //����ı��ļ������ڲ���
SYSTEMTIME g_time;               //���ʱ�䣬���ڲ���
char       g_chtimenow[16];      //���ʱ�䣬���ڲ���
string     g_str_topic;
string     m_str_FilePath;

ofstream   outFile;

int s = -1;

class ReplyActor : public Actor {
public:
	ReplyActor(const std::string& id) : Actor(id) {}
	int OnStart(ActorContext&)
	{
		Subscribe(g_str_subid, g_str_topic);
		GetSystemTime(&g_time);
		outFile <<"start time:"<< g_time.wYear << g_time.wMonth << g_time.wDay << " " << g_time.wHour + 8 << g_time.wMinute << g_time.wSecond << " " << g_time.wMilliseconds << endl;
		outFile.close();
		cout << "test start ok" << endl;
		return 0;
	}
	int OnEvent(Event& e)
	{
		string smss;
		smss = e.message();
		//cout << smss << endl;
		cout << ".";
		outFile.open(m_str_FilePath, ios::app);
		outFile << smss << endl;
		//fprintf(g_file_outputfile, "data:%s\n", smss);
		GetSystemTime(&g_time);
		//sprintf_s(g_chtimenow, "%d-%d-%d %d:%d:%d.%d", g_time.wYear, g_time.wMonth, g_time.wDay, g_time.wHour + 8, g_time.wMinute, g_time.wSecond, g_time.wMilliseconds);
		//fprintf(g_file_outputfile, "stop time:%d-%d-%d %d:%d:%d.%d\n", g_time.wYear, g_time.wMonth, g_time.wDay, g_time.wHour + 8, g_time.wMinute, g_time.wSecond, g_time.wMilliseconds);
		outFile << "stop time:" << g_time.wYear << g_time.wMonth << g_time.wDay << " " << g_time.wHour + 8 << g_time.wMinute << g_time.wSecond << " " << g_time.wMilliseconds << endl;
		switch (s)
		{
		case 10:
		default:
			break;
		}
		parsesbe(smss);
		outFile.close();
		return 0;
	}
};

int main(int argc, char* argv[]) {

	iblog_init(argv[0]);
	iblog_v(0);//�Զ���VLOG(v)ʱ��vֵС�ڴ˴�����ֵ�����������

	Context ctx;
	Stage stage(ctx);

	GetSystemTime(&g_time);
	sprintf_s(g_chtimenow, "%d%d%d %d%d%d", g_time.wYear, g_time.wMonth, g_time.wDay, g_time.wHour + 8, g_time.wMinute, g_time.wSecond);
	m_str_FilePath.append("out");
	m_str_FilePath.append(g_chtimenow);
	m_str_FilePath.append(".txt");
	//fopen_s(&g_file_outputfile, m_str_FilePath.c_str(), "a+");
	outFile.open(m_str_FilePath, ios::app);

	stage.Bind("tcp://*:7711");
	cout << "1)192.168.15.200:7799,2)192.168.15.200:7788,3)127.0.0.1:1212,\n4)192.168.15.200:2010,5)192.168.15.200:2012,6)tcp://192.166.1.204:2010,7)tcp://192.166.1.204:2012\nselect addr:";
	cin >> s;
	switch (s)
	{
	case 7:g_str_subendpoint = "tcp://192.166.1.204:2012"; //(g_str_subid, "tcp://192.168.15.200:7788");
		break;
	case 6:g_str_subendpoint = "tcp://192.166.1.204:2010"; //(g_str_subid, "tcp://192.168.15.200:7788");
		break;
	case 5:g_str_subendpoint = "tcp://192.168.15.200:2012"; //(g_str_subid, "tcp://192.168.15.200:7788");
		break;
	case 4:g_str_subendpoint = "tcp://192.168.15.200:2010"; //(g_str_subid, "tcp://192.168.15.200:7788");
		break;
	case 3:g_str_subendpoint = "tcp://127.0.0.1:1212";;
		break;
	case 2:g_str_subendpoint = "tcp://192.168.15.200:7788"; //(g_str_subid, "tcp://192.168.15.200:7788");
		break;
	case 1:
	default:g_str_subendpoint = "tcp://192.168.15.200:7799";
		break;
	}
	s = -1;
	cout << "1)83,2)600446,3)88,4)1002,5)1004,6)1102,7)1104,8)1012,9)1014,10)1112,11)1114,12)2011\nselect topic:";
	cin >> s;
	switch (s)
	{
	case 12:g_str_topic = "2011";
		break;
	case 11:g_str_topic = "1114";
		break;
	case 10:g_str_topic = "1112";
		break;
	case 9:g_str_topic = "1014";
		break;
	case 8:g_str_topic = "1012";
		break;
	case 7:g_str_topic = "1104";
		break;
	case 6:g_str_topic = "1102";
		break;
	case 5:g_str_topic = "1004";
		break;
	case 4:g_str_topic = "1002";
		break;
	case 3:g_str_topic = "88";
		break;
	case 2:g_str_topic = "600446";
		break;
	case 1:g_str_topic = "83";
		break;
	case 0:g_str_topic = "";
		break;
	}
	stage.AddSubscriber(g_str_subid, g_str_subendpoint);
	ActorPtr actor(new ReplyActor("aa"));
	stage.AddActor(actor);
	//stage.Connect(g_str_connid, g_str_subendpoint);
	stage.Start();
	stage.Join();
	return 0;
}