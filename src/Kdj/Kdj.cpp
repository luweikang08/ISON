#include "Kdj.h"

using namespace std;
using namespace ison::base;

const char* g_str_subid = "sub1";
const char* g_str_subendpoint = "tcp://127.0.0.1:1212";
const char* g_str_subtopic = "88";

const char* g_str_pubid = "pub1";
const char* g_str_pubendpoint = "tcp://127.0.0.1:1212";
const char* g_str_pubtopic = "88";

SYSTEMTIME g_time;
char       g_chtimenow[20];
FILE*      g_file_outputfile;

class ProcActor : public Actor {
public:
	ProcActor(const std::string& id) : Actor(id) {}
	int OnStart(ActorContext&) {
		Subscribe(g_str_subid, g_str_subtopic);
		cout << "test start ok" << endl;

		GetSystemTime(&g_time);
		sprintf_s(g_chtimenow, "%d-%d-%d %d:%d:%d.%d", g_time.wYear, g_time.wMonth, g_time.wDay, g_time.wHour + 8, g_time.wMinute, g_time.wSecond, g_time.wMilliseconds);
		fprintf(g_file_outputfile, "start time:%s\n", g_chtimenow);
		return 0;
	}
	int OnEvent(Event& e) {
		string smss;
		string StockCode;
		smss = e.message();
		return 0;
	}
};

int main(int argc, char* argv[])
{
	char m_jsonFileName[21];
	m_jsonFileName[0] = '\0';

	if (argc == 1)  //store config file name
	{
		strcpy_s(m_jsonFileName, JSON_FILE_DEFAULT);
	}
	else if (argc==2)
	{
		strcpy_s(m_jsonFileName, argv[1]);
	}

	std::vector<string> configkey = CONFIG_KEY_VECTOR_DEFAULT;
	std::map<string, string> configmap;
	string m_str_appname;
	GetAppName(argv[0], m_str_appname);
	cout << m_str_appname << endl;
	RF_RetCode m_rf_retcode = readjsonfile(m_jsonFileName, m_str_appname.c_str(), configkey, configmap);
	if (m_rf_retcode == RF_FILE_NOT_EXIST)
	{
		cout << "config file not exist." << endl;
		ofstream ofs;
		ofs.open(m_jsonFileName, ofstream::out);

		rapidjson::Document m_Document;
		m_Document.SetObject();
		rapidjson::Document::AllocatorType& m_Allocator = m_Document.GetAllocator();

		rapidjson::Value m_Value_KDJ;
		m_Value_KDJ.SetObject();
		rapidjson::Value m_Value_Temp;
		m_Value_Temp.SetObject();

		m_Value_Temp.SetString(KDJ_VERSION, strlen(KDJ_VERSION), m_Allocator);
		m_Value_KDJ.AddMember("version", m_Value_Temp, m_Allocator);
		m_Value_Temp.SetString(KDJ_SUBTOPIC, strlen(KDJ_SUBTOPIC), m_Allocator);
		m_Value_KDJ.AddMember("subtopic", m_Value_Temp, m_Allocator);
		m_Value_Temp.SetString(KDJ_SUBENDPOINT, strlen(KDJ_SUBENDPOINT), m_Allocator);
		m_Value_KDJ.AddMember("subendpoint", m_Value_Temp, m_Allocator);
		m_Value_Temp.SetString(KDJ_PUBTOPIC, strlen(KDJ_PUBTOPIC), m_Allocator);
		m_Value_KDJ.AddMember("pubtopic", m_Value_Temp, m_Allocator);
		m_Value_Temp.SetString(KDJ_PUBENDPOINT, strlen(KDJ_PUBENDPOINT), m_Allocator);
		m_Value_KDJ.AddMember("pubendpoint", m_Value_Temp, m_Allocator);

		m_Document.AddMember("Kdj.exe", m_Value_KDJ, m_Allocator);

		typedef rapidjson::GenericStringBuffer<rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator<>> StringBuffer;
		StringBuffer buf(&m_Allocator);
		rapidjson::Writer<StringBuffer> writer(buf);
		m_Document.Accept(writer);
		std::string json(buf.GetString(), buf.Size());

		ofs << json;

		return -1;
	}
	else if (m_rf_retcode == RF_PARSE_ERROR)
	{
		cout << "json file parse error,please check context format" << endl;
		return -1;
	}
	else if (m_rf_retcode == RF_KEY_NOT_EXIST)
	{
		cout << "Cann't find wanted setting infomation in config file" << endl;
		return -1;
	}

	iblog_init(argv[0]);
	iblog_v(0);//自定义VLOG(v)时，v值小于此处设置值的语句才有输出
	Context ctx;
	Stage stage(ctx);

	stage.Bind("tcp://*:7799");
	stage.AddSubscriber(g_str_subid, g_str_subendpoint);
	stage.AddPublisher(g_str_pubid, g_str_pubendpoint);
	ActorPtr actor(new ProcActor("Proc"));
	stage.AddActor(actor);
	//stage.Connect(g_str_connid, g_str_subendpoint);
	stage.Start();
	stage.Join();

	return 0;
}