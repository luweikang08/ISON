#include "KLine.h"

using namespace ison::base;
using namespace std;
using namespace rapidjson;
using namespace ison::indicator;

char g_str_version[10];
char g_str_releasedate[12];
char g_str_subendpoint[30];
char g_str_pubendpoint[30];

int g_num_loglevel;

unsigned g_num_ms;
unsigned g_num_tm;
unsigned g_num_SH_sn;
unsigned g_num_SZ_sn;

map<string, string> g_map_configmap;

KLineBase g_Klb;

class ProcActor : public Actor {
public:
	ProcActor(const std::string& id) : Actor(id) {}
	int OnStart(ActorContext&)
	{
		Subscribe(SUBID, SUBTOPIC_SZ);
		Subscribe(SUBID, SUBTOPIC_SH);
		cout << "Actor start ok" << endl;
		LOG(INFO) << "ProcActor start ok";
		return 0;
	}
	int OnEvent(Event& e)
	{
		char m_ch_code[16];
		int m_pubtopic;
		int m_num_sn;

		if (e.type() == kPublish)
		{
			cout << ".";
			g_Klb.Store(e.message());
			if (g_Klb.isNeedPub())
			{
				std::memcpy(m_ch_code, g_Klb.GetDataCode(), 16);//new
				if (m_ch_code[8] == 'Z' || m_ch_code[8] == 'z')  //shenzhen stock
				{
					m_pubtopic = ISON_TRADEPUBTOPIC::TSZ_KLINEONEMINUTE;
					m_num_sn = g_num_SZ_sn;
				}
				else       //shanghai stock and others
				{
					m_pubtopic = ISON_TRADEPUBTOPIC::TSH_KLINEONEMINUTE;
					m_num_sn = g_num_SH_sn;
				}
				std::string m_SendStr = g_Klb.MakeSendStr(m_pubtopic, m_num_sn);
				Publish(PUBID, itostring(m_pubtopic), m_SendStr);
				g_Klb.resetPubFlag();
				
				char Buffer_temp[BUFFELENGTH];
				TOPICHEAD TopicHead_temp;
				std::memcpy(Buffer_temp, m_SendStr.c_str(), m_SendStr.size());
				baseline::MessageHeader hdr_temp;
				baseline::SDS_Kline KK_temp;
				hdr_temp.wrap(Buffer_temp, sizeof(TOPICHEAD), MESSAGEHEADERVERSION, BUFFELENGTH);
				KK_temp.wrapForDecode(Buffer_temp, sizeof(TOPICHEAD) + hdr_temp.size(), hdr_temp.blockLength(), hdr_temp.version(), BUFFELENGTH);
				LOG(INFO) << "Code:" << KK_temp.code() << "Time:" << KK_temp.time() << "Open:" << KK_temp.open() << "Close:" << KK_temp.close() << "High:" << KK_temp.high() << "Low:" << KK_temp.low()\
					<< "Volume:" << KK_temp.volume() << "Turnover:" << KK_temp.turnover();
			}
		}
		return 0;
	}
};

class TimerActor : public Actor {
public:
	TimerActor(const std::string& id) : Actor(id) {}
	int OnStart(ActorContext&)
	{
		//AddTimer(30000);
		LOG(INFO) << "TimerActor start ok";
		return 0;
	}
	int OnEvent(Event& e)
	{
		DateAndTime m_DateAndTime = GetDateAndTime();
		int m_pubtopic;
		int m_num_sn;
		
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
	RF_RetCode m_rf_retcode = readjsonfile(m_jsonFileName, m_str_appname.c_str(), VECTOR_CONFIGKEY, g_map_configmap);
	if (m_rf_retcode == RF_SUCCESS)
	{
		LOG(INFO) << m_jsonFileName << ":read success.";
		strcpy_s(g_str_version, g_map_configmap[VECTOR_CONFIGKEY[0]].c_str());
		strcpy_s(g_str_releasedate, g_map_configmap[VECTOR_CONFIGKEY[1]].c_str());
		if ((_stricmp(g_str_version, VECTOR_CONFIGVALUE_DEFAULT[0].c_str()) == 0) && (_stricmp(g_str_releasedate, VECTOR_CONFIGVALUE_DEFAULT[1].c_str()) == 0))  //Performs a lowercase comparison of strings.
		{
			g_num_loglevel = atoi(g_map_configmap[VECTOR_CONFIGKEY[2]].c_str());
			strcpy_s(g_str_subendpoint, g_map_configmap[VECTOR_CONFIGKEY[3]].c_str());
			strcpy_s(g_str_pubendpoint, g_map_configmap[VECTOR_CONFIGKEY[4]].c_str());
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
		createjsonfile(m_jsonFileName, m_str_appname.c_str(), VECTOR_CONFIGKEY, VECTOR_CONFIGVALUE_DEFAULT);
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

	stage.AddSubscriber(SUBID, g_str_subendpoint);
	stage.AddPublisher(PUBID, g_str_pubendpoint);
	ActorPtr Proc_Actor(new ProcActor("proc"));
	ActorPtr Timer_Actor(new TimerActor("timer"));
	stage.AddActor(Proc_Actor);
	stage.AddActor(Timer_Actor);
	stage.Start();
	stage.Join();
	return 0;
}