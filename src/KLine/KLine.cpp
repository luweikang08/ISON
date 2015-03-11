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
int g_num_rcvhwm;
int g_num_sndhwm;
bool g_bool_playback; //run in playback mode or not
unsigned int g_num_Tran_sn;

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
#ifdef NOTOPICHEAD
		Subscribe(SUBID, PreTopicTrans);
#else
		Subscribe(SUBID, itostring(ISON_SDS2TGW::TSZ_TRANSACTION));
		Subscribe(SUBID, itostring(ISON_SDS2TGW::TSH_TRANSACTION));
#endif
		cout << "ProcActor start ok" << endl;
		LOG(INFO) << "ProcActor start ok";
		return 0;
	}
	int OnEvent(Event& e)
	{
		char m_ch_code[16];
		string m_pubtopic;
		int m_num_sn;

		if (e.type() == kPublish)
		{
			LOG_IF(INFO, g_num_loglevel > 5) << ".";
			string smss;
			smss = e.message();
			if (smss.size() != (TOPICHEADSIZE + baseline::MessageHeader::size() + baseline::SDS_Transaction::sbeBlockLength()))
			{
				LOG(INFO) << "receive data size error.";
				return -1;
			}
			g_Klb.Store(e.message());

			if ((g_Klb.GetSn() - g_num_Tran_sn) != 1) //check if data is miss or not
			{
				LOG(INFO) << "data miss:" << (g_Klb.GetSn() - g_num_Tran_sn);
			}
			g_num_Tran_sn = g_Klb.GetSn();

			if (g_Klb.IsNeedPub())
			{
				//memcpy(m_ch_code, g_Klb.GetDataCode(), 16);//new
				m_pubtopic.append(PreTopicKline);
				m_pubtopic.append(g_Klb.GetDataCode());
				std::string m_SendStr = g_Klb.MakeSendStr();
				Publish(PUBID, m_pubtopic, m_SendStr);
				LOG_IF(INFO, g_num_loglevel > 6) << "x";
				g_Klb.ResetPubFlag();

				if (g_num_loglevel > 1)
				{
					char Buffer_temp[BUFFELENGTH];
					memcpy(Buffer_temp, m_SendStr.c_str(), m_SendStr.size());
					baseline::MessageHeader hdr_temp;
					baseline::SDS_Kline KK_temp;
					hdr_temp.wrap(Buffer_temp, TOPICHEADSIZE, MESSAGEHEADERVERSION, BUFFELENGTH);
					KK_temp.wrapForDecode(Buffer_temp, TOPICHEADSIZE + hdr_temp.size(), hdr_temp.blockLength(), hdr_temp.version(), BUFFELENGTH);
					LOG(INFO) << "Code:" << KK_temp.code() << " Time:" << KK_temp.time() << " Open:" << KK_temp.open() << " Close:" << KK_temp.close() << " High:" << KK_temp.high() << " Low:" << KK_temp.low()\
						<< " Volume:" << KK_temp.volume() << " Turnover:" << KK_temp.turnover();
				}
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
		AddTimer(30000);
		cout << "TimerActor start ok" << endl;
		LOG(INFO) << "TimerActor start ok";
		return 0;
	}
	int OnEvent(Event& e)
	{
		DateAndTime m_DateAndTime_Now = GetDateAndTime();
		string m_pubtopic;
		int m_num_sn;
		char m_ch_code[16];
		
		g_Klb.InitIt();
		for (int i = 0; i < g_Klb.GetDataMapSize(); i++)
		{
			if (g_Klb.ChkItNeedPub(g_bool_playback))
			{
				memcpy(m_ch_code, g_Klb.GetCodeInIt(), 16);//new
				m_pubtopic.append(PreTopicKline);
				m_pubtopic.append(m_ch_code);
				std::string m_SendStr = g_Klb.MakeSendStr();
				Publish(PUBID, m_pubtopic, m_SendStr);
				LOG_IF(INFO, g_num_loglevel > 6) << "y";
			}
			g_Klb.IncreIt();//iterator++
		}

		return 0;
	}
};

int main(int argc, char* argv[]) 
{
	iblog_init(argv[0], g_clog_dir);
	iblog_v(0);//自定义VLOG(v)时，v值小于此处设置值的语句才有输出
	iblog_logbufsecs(0);

	char m_jsonFileName[21];
	m_jsonFileName[0] = '\0';
	g_num_Tran_sn = 0;

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
			g_num_rcvhwm = atoi(g_map_configmap[VECTOR_CONFIGKEY[5]].c_str());
			g_num_sndhwm = atoi(g_map_configmap[VECTOR_CONFIGKEY[6]].c_str());
			if (atoi(g_map_configmap[VECTOR_CONFIGKEY[7]].c_str()))
			{
				g_bool_playback = true;
			}
			else
			{
				g_bool_playback = false;
			}
			cout << "get setting success." << endl;
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

	//stage.AddSubscriber(SUBID, g_str_subendpoint);
	//stage.AddPublisher(PUBID, g_str_pubendpoint);
	ConnectionConfig m_Config_Sub, m_Config_Pub;
	m_Config_Sub.id = SUBID;
	m_Config_Sub.endpoint = g_str_subendpoint;
	m_Config_Sub.rcvhwm = g_num_rcvhwm;
	m_Config_Pub.id = PUBID;
	m_Config_Pub.endpoint = g_str_pubendpoint;
	m_Config_Pub.sndhwm = g_num_sndhwm;
	stage.AddSubscriber(m_Config_Sub);
	stage.AddPublisher(m_Config_Pub);

	ActorPtr Proc_Actor(new ProcActor("proc"));
	ActorPtr Timer_Actor(new TimerActor("timer"));
	stage.AddActor(Proc_Actor);
	stage.AddActor(Timer_Actor);
	stage.Start();
	stage.Join();
	return 0;
}