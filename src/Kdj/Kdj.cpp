#include "Kdj.h"

using namespace std;
using namespace ison::base;

const char* g_str_subid = "sub1";

const char* g_str_pubid = "pub1";

const char* g_clog_dir = "KDJLog";

char g_str_version[10] = KDJ_VERSION;
char g_str_releasedate[12] = KDJ_RELEASEDATE;
char g_str_subtopic[24] = KDJ_SUBTOPIC;
char g_str_subendpoint[30] = KDJ_SUBENDPOINT;
char g_str_pubtopic[24] = KDJ_PUBTOPIC;
char g_str_pubendpoint[30] = KDJ_PUBENDPOINT;

char* g_str_filepath = JSON_FILE_DEFAULT;
char* g_str_configkey[] = CONFIG_KEY_VECTOR_DEFAULT;
const char* g_str_configvalue[] = { KDJ_VERSION, KDJ_RELEASEDATE, KDJ_LOGLEVEL, KDJ_SUBTOPIC, KDJ_SUBENDPOINT, KDJ_PUBTOPIC, KDJ_PUBENDPOINT };

int g_num_loglevel = atoi(KDJ_LOGLEVEL);

SYSTEMTIME g_time;
char       g_chtimenow[20];
FILE*      g_file_outputfile;

vector<string> g_vector_configkey = CONFIG_KEY_VECTOR_DEFAULT;
map<string, string> g_map_configmap;

class ProcActor : public Actor {
public:
	ProcActor(const std::string& id) : Actor(id) {}
	int OnStart(ActorContext&) 
	{
		int m_num_position = 0;

		string m_str_subtopic_all;
		string m_str_subtopic_single;
		m_str_subtopic_all.append(g_str_subtopic);

		m_str_subtopic_single = m_str_subtopic_all.substr(m_num_position, 4);    //sub first topic
		Subscribe(g_str_subid, m_str_subtopic_single);
		while ((m_num_position = m_str_subtopic_all.find_first_of(",", m_num_position)) != string::npos) //if "," is exist,sub follows topic
		{
			m_num_position++;
			m_str_subtopic_single = m_str_subtopic_all.substr(m_num_position, 4);
			if (m_str_subtopic_single.length() == 4)
			{
				Subscribe(g_str_subid, m_str_subtopic_single);
			}
		}
		cout << "Actor start ok" << endl;
		LOG(INFO) << "Actor start ok";
		return 0;
	}
	int OnEvent(Event& e) {
		if (e.type() == kPublish)
		{
			LOG_IF(INFO, g_num_loglevel > 6) << "Data receive.";
			string smss;
			string StockCode;
			smss = e.message();
		}
		return 0;
	}
};

int main(int argc, char* argv[])
{
	iblog_init(argv[0], g_clog_dir);
	iblog_v(0);//自定义VLOG(v)时，v值小于此处设置值的语句才有输出

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

	string m_str_appname;
	GetAppName(argv[0], m_str_appname);
	RF_RetCode m_rf_retcode = readjsonfile(m_jsonFileName, m_str_appname.c_str(), g_vector_configkey, g_map_configmap);
	if (m_rf_retcode == RF_SUCCESS)
	{
		LOG(INFO) << m_jsonFileName << ":read success.";
		strcpy_s(g_str_version, g_map_configmap["version"].c_str());
		strcpy_s(g_str_releasedate, g_map_configmap["releasedate"].c_str());
		if ((_stricmp(g_str_version, KDJ_VERSION) == 0) && (_stricmp(g_str_releasedate, KDJ_RELEASEDATE) == 0))  //Performs a lowercase comparison of strings.
		{
			g_num_loglevel = atoi(g_map_configmap["loglevel"].c_str());
			strcpy_s(g_str_subtopic, g_map_configmap["subtopic"].c_str());
			strcpy_s(g_str_subendpoint, g_map_configmap["subendpoint"].c_str());
			strcpy_s(g_str_pubtopic, g_map_configmap["pubtopic"].c_str());
			strcpy_s(g_str_pubendpoint, g_map_configmap["pubendpoint"].c_str());
			cout << "get setting success."<<endl;
		}
		else
		{
			cout << "version or date set error,please check it!" << endl;
			LOG(INFO) << "version or date set error,please check it!";
			return -1;
		}
	}
	else if (m_rf_retcode == RF_FILE_NOT_EXIST)
	{
		cout << "config file not exist." << endl;
		ofstream ofs;
		ofs.open(m_jsonFileName, ofstream::out);

		rapidjson::Document m_Document;
		m_Document.SetObject();
		rapidjson::Document::AllocatorType& m_Allocator = m_Document.GetAllocator();

		rapidjson::Value m_Value_KDJ;
		m_Value_KDJ.SetObject();

		for (int i = 0; i < (sizeof(g_str_configvalue) / sizeof(g_str_configvalue[0])); i++)// add string to json
		{
			m_Value_KDJ.AddMember(g_str_configkey[i], g_str_configvalue[i], m_Allocator);
		}

		m_Document.AddMember(argv[0], m_Value_KDJ, m_Allocator);

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
		LOG(INFO) << "json file parse error,please check context format";
		return -1;
	}
	else if (m_rf_retcode == RF_KEY_NOT_EXIST)
	{
		cout << "Cann't find wanted setting infomation in config file" << endl;
		LOG(INFO) << "Cann't find wanted setting infomation in config file";
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