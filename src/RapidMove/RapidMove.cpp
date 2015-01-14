#include "RapidMove.h"

using namespace ison::base;

const char* g_str_subid = "sub1";
const char* g_str_pubid = "pub1";
const char* g_clog_dir = "RMLog";

char g_str_version[10] = MOVE_VERSION;
char g_str_releasedate[12] = MOVE_RELEASEDATE;
char g_str_subtopic[24] = MOVE_SUBTOPIC;       //topic, ex:1014,1114
char g_str_subendpoint[30] = MOVE_SUBENDPOINT;
char g_str_pubtopic[24] = MOVE_PUBTOPIC;       //topic, ex:2011,2111
char g_str_pubendpoint[30] = MOVE_PUBENDPOINT;

char* g_str_filepath = JSON_FILE_DEFAULT;
char* g_str_configkey[] = CONFIG_KEY_VECTOR_DEFAULT;
const char* g_str_configvalue[] = { MOVE_VERSION, MOVE_RELEASEDATE, MOVE_LOGLEVEL, MOVE_SUBTOPIC, MOVE_SUBENDPOINT, MOVE_PUBTOPIC, MOVE_PUBENDPOINT };

int g_num_loglevel = atoi(MOVE_LOGLEVEL);
double g_num_riselimit = 1.8;
double g_num_falllimit = 1.8;

unsigned g_num_ms;
unsigned g_num_tm;
unsigned g_num_SH_sn;
unsigned g_num_SZ_sn;

vector<string> g_vector_configkey = CONFIG_KEY_VECTOR_DEFAULT;
map<string, string> g_map_configmap;    //config file data
map<string, map<int,int>> g_map_data;   //<code,<time,price>>

TranData g_TranData;

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
	int OnEvent(Event& e)
	{
		if (e.type() == kPublish)
		{
			LOG_IF(INFO, g_num_loglevel > 6) << "Data receive.";
			cout << ".";
			string smss = e.message();
			string m_pubtopic_all;
			string m_pubtopic_single;
			int m_num_sn;

			m_pubtopic_all.append(g_str_pubtopic);

			char recvBuf[1024];
			memcpy(recvBuf, smss.c_str(), smss.size());
			TOPICHEAD  * m_topichead_rec;
			m_topichead_rec = (TOPICHEAD*)recvBuf;

			baseline::SDS_Transaction  CC;
			baseline::MessageHeader hdr;
			int messageHeaderVersion = 0;
			hdr.wrap(recvBuf + sizeof(TOPICHEAD), 0, messageHeaderVersion, 1024);//parse messageheader
			CC.wrapForDecode(recvBuf, sizeof(TOPICHEAD) + hdr.size(), hdr.blockLength(), hdr.version(), 1024);

			sbe2struct(CC, g_TranData); //store data to struct
			if (g_TranData.Code[8] == 'Z' || g_TranData.Code[8] == 'z')  //shenzhen stock
			{
				m_pubtopic_single = m_pubtopic_all.substr(0, 4);
				m_num_sn = g_num_SZ_sn;
			}
			else       //shanghai stock and others
			{
				m_pubtopic_single = m_pubtopic_all.substr(5, 4);
				m_num_sn = g_num_SH_sn;
			}
			map<string, map<int,int>>::iterator it = g_map_data.find(g_TranData.Code); //find curent stock is exist in map or not
			map<int, int> m_map;
			if (it == g_map_data.end())
			{
				string m_str_code(g_TranData.Code, 16);
				m_map.insert(pair<int, int>(g_TranData.Time, g_TranData.Price));
				g_map_data.insert(pair<string, map<int, int>>(m_str_code, m_map));
			}
			else
			{
				m_map = it->second;
			}
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
	else if (argc == 2)
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
		if ((_stricmp(g_str_version, MOVE_VERSION) == 0) && (_stricmp(g_str_releasedate, MOVE_RELEASEDATE) == 0))  //Performs a lowercase comparison of strings.
		{
			g_num_loglevel = atoi(g_map_configmap["loglevel"].c_str());
			strcpy_s(g_str_subtopic, g_map_configmap["subtopic"].c_str());
			strcpy_s(g_str_subendpoint, g_map_configmap["subendpoint"].c_str());
			strcpy_s(g_str_pubtopic, g_map_configmap["pubtopic"].c_str());
			strcpy_s(g_str_pubendpoint, g_map_configmap["pubendpoint"].c_str());
			cout << "get setting success.";
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

		rapidjson::Value m_Value_MOVE;
		m_Value_MOVE.SetObject();
		rapidjson::Value m_Value_Temp;
		m_Value_Temp.SetObject();

		int i = 0;
		for (; i < (sizeof(g_str_configvalue) / sizeof(g_str_configvalue[0])); i++)// add string to json
		{
			m_Value_Temp.SetString(MOVE_VERSION, strlen(MOVE_VERSION), m_Allocator);
			m_Value_MOVE.AddMember(g_str_configkey[i], g_str_configvalue[i], m_Allocator);
		}
		m_Value_MOVE.AddMember(g_str_configkey[i++], MOVE_RISELIMIT, m_Allocator);
		m_Value_MOVE.AddMember(g_str_configkey[i], MOVE_FALLLIMIT, m_Allocator);


		m_Document.AddMember(m_str_appname.c_str(), m_Value_MOVE, m_Allocator);
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

	SnInit(g_num_SH_sn);
	SnInit(g_num_SZ_sn);

	Context ctx;
	Stage stage(ctx);

	stage.AddSubscriber(g_str_subid, g_str_subendpoint);
	stage.AddPublisher(g_str_pubid, g_str_pubendpoint);
	ActorPtr Proc_Actor(new ProcActor("proc"));
	stage.AddActor(Proc_Actor);
	stage.Start();
	stage.Join();
	return 0;
}