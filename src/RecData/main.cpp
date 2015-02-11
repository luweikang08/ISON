#include "RecData.h"

using namespace ison::base;
using namespace std;

const string g_str_connid = "con1";
const string g_str_endpoint = "tcp://127.0.0.1:7799";
//const char * g_str_filepath = "result.txt";
string  g_str_subendpoint = "tcp://192.168.15.200:7799";
const char*  g_str_subid = "sub1";

const char*  g_str_pubendpoint = "tcp://127.0.0.1:1212";
const char*  g_str_pubid = "pub1";
const char*  g_str_pubtopic = "88";

FILE*      g_file_outputfile;         //输出文本文件，用于测试
SYSTEMTIME g_time;               //输出时间，用于测试
char       g_chtimenow[16];      //输出时间，用于测试
string     g_str_topic[2];
string     m_str_FilePath;

ofstream   outFile;

int s = -1;

int g_num_link = 0;

class ReplyActor : public Actor {
public:
	ReplyActor(const std::string& id) : Actor(id) {}
	int OnStart(ActorContext&)
	{
		cout << "endpoint:" << g_str_subendpoint << " topic:" << g_str_topic[0] << "," << g_str_topic[1] << endl;
		if (g_str_topic[0] != "")
			Subscribe(g_str_subid, g_str_topic[0]);
		if (g_str_topic[1] != "")
			Subscribe(g_str_subid, g_str_topic[1]);
		GetSystemTime(&g_time);
		/*outFile <<"start time:"<< g_time.wYear << g_time.wMonth << g_time.wDay << " " << g_time.wHour + 8 << g_time.wMinute << g_time.wSecond << " " << g_time.wMilliseconds << endl;
		outFile.close();*/
		LOG(INFO) << "start time:" << g_time.wYear << g_time.wMonth << g_time.wDay << " " << g_time.wHour + 8 << g_time.wMinute << g_time.wSecond << " " << g_time.wMilliseconds;
		cout << "test start ok" << endl;
		return 0;
	}
	int OnEvent(Event& e)
	{
		string smss;
		smss = e.message();
		//cout << smss << endl;
		cout << ".";
		//outFile.open(m_str_FilePath, ios::app);
		//outFile << smss << endl;
		////fprintf(g_file_outputfile, "data:%s\n", smss);
		//GetSystemTime(&g_time);
		////sprintf_s(g_chtimenow, "%d-%d-%d %d:%d:%d.%d", g_time.wYear, g_time.wMonth, g_time.wDay, g_time.wHour + 8, g_time.wMinute, g_time.wSecond, g_time.wMilliseconds);
		////fprintf(g_file_outputfile, "stop time:%d-%d-%d %d:%d:%d.%d\n", g_time.wYear, g_time.wMonth, g_time.wDay, g_time.wHour + 8, g_time.wMinute, g_time.wSecond, g_time.wMilliseconds);
		//outFile << "stop time:" << g_time.wYear << g_time.wMonth << g_time.wDay << " " << g_time.wHour + 8 << g_time.wMinute << g_time.wSecond << " " << g_time.wMilliseconds << endl;
		int temp;
		switch (s)
		{
		case 2120:
		case 2021:
		case 21:
		case 20:parse_sbe_indicator(smss);
			break;
		case 1918:
		case 1819:
		case 19:
		case 18:
		case 1514:
		case 1415:
		case 15:
		case 14:parse_sbe_kline(smss);
			break;
		case 1716:
		case 1617:
		case 17:
		case 16:
		case 1213:
		case 1312:
		case 13:
		case 12:parse_sbe_signal(smss);
			break;
		case 810:
		case 10:
		case 8:
			temp = parse_sbe_level2(smss);
			if (g_num_link==0)
			{
				g_num_link = temp;
			}
			else
			{
				if ((temp - g_num_link) > 1)
				{
					cout << g_num_link << "," << (temp - g_num_link) << "," << temp;
				}
				g_num_link = temp;
			}
			
			break;
		default:
			break;
		}
		//outFile.close();
		return 0;
	}
};

int main(int argc, char* argv[]) {

	iblog_init(argv[0],"log");
	iblog_v(0);//自定义VLOG(v)时，v值小于此处设置值的语句才有输出

	Context ctx;
	Stage stage(ctx);

	GetSystemTime(&g_time);
	/*sprintf_s(g_chtimenow, "%d%d%d %d%d%d", g_time.wYear, g_time.wMonth, g_time.wDay, g_time.wHour + 8, g_time.wMinute, g_time.wSecond);
	m_str_FilePath.append("out");
	m_str_FilePath.append(g_chtimenow);
	m_str_FilePath.append(".txt");*/
	//fopen_s(&g_file_outputfile, m_str_FilePath.c_str(), "a+");
	//outFile.open(m_str_FilePath, ios::app);

	stage.Bind("tcp://*:7711");
	cout << "Endpoint:(1)192.168.15.200,(2)tcp://192.166.1.203,(3)tcp://192.166.1.204,(4)tcp://127.0.0.1\n---select addr:";
	cin >> s;
	switch (s)
	{
	case 4:g_str_subendpoint = "tcp://127.0.0.1:";
		break;
	case 3:g_str_subendpoint = "tcp://192.166.1.204:"; //(g_str_subid, "tcp://192.168.15.200:7788");
		break;
	case 2:g_str_subendpoint = "tcp://192.166.1.203:"; //(g_str_subid, "tcp://192.168.15.200:7788");
		break;
	case 1:
	default:g_str_subendpoint = "tcp://192.168.15.200:";
		break;
	}

	cout << "Port:(1)2010,(2)2011,(3)2012,(4)1212,(5)7799,(6)7788\n(7)2030,(8)2031,(9)2032,(10)6001,(11)2003\n---select port:";
	cin >> s;
	switch (s)
	{
	case 11:g_str_subendpoint.append("2003");
		break;
	case 10:g_str_subendpoint.append("6001");
		break;
	case 9:g_str_subendpoint.append("2032");
		break;
	case 8:g_str_subendpoint.append("2031");
		break;
	case 7:g_str_subendpoint.append("2030");
		break;
	case 6:g_str_subendpoint.append("7788");
		break;
	case 5:g_str_subendpoint.append("7799");
		break;
	case 4:g_str_subendpoint.append("1212");
		break;
	case 3:g_str_subendpoint.append("2012");
		break;
	case 2:g_str_subendpoint.append("2011");
		break;
	case 1:g_str_subendpoint.append("2010");
	default:
		break;
	}

	s = -1;
	cout << "1)83,2)600446,3)88,4)1002,5)1004,6)1102,7)1104,\n8)1012,9)1014,10)1112,11)1114,\n12)2011,13)2111,14)2012,15)2112,\n"\
		<< "16)2030,17)2130,18)2031,19)2131,20)2032,21)2132,\n"\
		<< "22)HL,\n"\
		<< "---select topic:";
	cin >> s;
	if (s < 20)
		goto single;
	int i = 0;
	switch (s / 100)
	{
	case 22:g_str_topic[i] = "HL";
		break;
	case 21:g_str_topic[i] = "2132";
		break;
	case 20:g_str_topic[i] = "2032";
		break;
	case 19:g_str_topic[i] = "2131";
		break;
	case 18:g_str_topic[i] = "2031";
		break;
	case 17:g_str_topic[i] = "2130";
		break;
	case 16:g_str_topic[i] = "2030";
		break;
	case 15:g_str_topic[i] = "2112";
		break;
	case 14:g_str_topic[i] = "2012";
		break;
	case 13:g_str_topic[i] = "2111";
		break;
	case 12:g_str_topic[i] = "2011";
		break;
	case 11:g_str_topic[i] = "1114";
		break;
	case 10:g_str_topic[i] = "1112";
		break;
	case 9:g_str_topic[i] = "1014";
		break;
	case 8:g_str_topic[i] = "1012";
		break;
	case 7:g_str_topic[i] = "1104";
		break;
	case 6:g_str_topic[i] = "1102";
		break;
	case 5:g_str_topic[i] = "1004";
		break;
	case 4:g_str_topic[i] = "1002";
		break;
	case 3:g_str_topic[i] = "88";
		break;
	case 2:g_str_topic[i] = "600446";
		break;
	case 1:g_str_topic[i] = "83";
		break;
	case 0:g_str_topic[i] = "";
		break;
	}
	i++;
single:
	switch (s%100)
	{
	case 22:g_str_topic[i] = "HL";
		break;
	case 21:g_str_topic[i] = "2132";
		break;
	case 20:g_str_topic[i] = "2032";
		break;
	case 19:g_str_topic[i] = "2131";
		break;
	case 18:g_str_topic[i] = "2031";
		break;
	case 17:g_str_topic[i] = "2130";
		break;
	case 16:g_str_topic[i] = "2030";
		break;
	case 15:g_str_topic[i] = "2112";
		break;
	case 14:g_str_topic[i] = "2012";
		break;
	case 13:g_str_topic[i] = "2111";
		break;
	case 12:g_str_topic[i] = "2011";
		break;
	case 11:g_str_topic[i] = "1114";
		break;
	case 10:g_str_topic[i] = "1112";
		break;
	case 9:g_str_topic[i] = "1014";
		break;
	case 8:g_str_topic[i] = "1012";
		break;
	case 7:g_str_topic[i] = "1104";
		break;
	case 6:g_str_topic[i] = "1102";
		break;
	case 5:g_str_topic[i] = "1004";
		break;
	case 4:g_str_topic[i] = "1002";
		break;
	case 3:g_str_topic[i] = "88";
		break;
	case 2:g_str_topic[i] = "600446";
		break;
	case 1:g_str_topic[i] = "83";
		break;
	case 0:g_str_topic[i] = "";
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