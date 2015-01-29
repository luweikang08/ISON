#include "ShortElf.h"

using namespace std;
using namespace ison::base;
using namespace ison::signal;
using namespace baseline;

char g_str_version[10];
char g_str_releasedate[12];
char g_str_subendpoint[30];
char g_str_pubendpoint[30];

unsigned int g_num_loglevel;
unsigned int g_num_volum_limit;
unsigned int g_num_turnover_limit;
unsigned int g_num_extra_sell_volum_limit;
unsigned int g_num_extra_sell_turnover_limit;
unsigned int g_num_extra_buy_volum_limit;
unsigned int g_num_extra_buy_turnover_limit;

unsigned g_num_SH_sn;
unsigned g_num_SZ_sn;

map<string, string> g_map_configmap;

LargeTranMonitor g_Ltm;

class ProcActor : public Actor {
public:
	ProcActor(const std::string& id) : Actor(id) {}
	int OnStart(ActorContext&)
	{
		Subscribe(SUBID, SUBTOPIC_SZ);
		Subscribe(SUBID, SUBTOPIC_SH);
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
			int m_pubtopic;
			int m_num_sn;
			char m_ch_code[16];

			g_Ltm.Store(e.message());//new
			std::memcpy(m_ch_code, g_Ltm.GetDataCode(), 16);//new

			if ((g_Ltm.GetDataTime() < 91500000) || (g_Ltm.GetDataTime() > 151000000))//new
			{
				LOG_IF(INFO, g_num_loglevel > 5) << "data time is not available.";
				return -1;
			}

			if ((m_ch_code[0] != '0') && (m_ch_code[0] != '3') && (m_ch_code[0] != '6'))
			{
				LOG_IF(INFO, g_num_loglevel > 6) << "stockid is not 0* 3* 6*.";
				return -1;
			}
			if (g_Ltm.CompStatus(g_num_volum_limit,g_num_turnover_limit)==EF_Large)     //large buy and sell with volum||turnover,if >=,then pub
			{
				LOG_IF(INFO, g_num_loglevel > 5) << "volum||turnover warning:" << g_Ltm.GetDataVolume();
				if (m_ch_code[8] == 'Z' || m_ch_code[8] == 'z')  //shenzhen stock
				{
					m_pubtopic = ISON_TRADEPUBTOPIC::TSZ_LARGETRANMONITOR;
					m_num_sn = g_num_SZ_sn;
				}
				else       //shanghai stock and others
				{
					m_pubtopic = ISON_TRADEPUBTOPIC::TSH_LARGETRANMONITOR;
					m_num_sn = g_num_SH_sn;
				}

				Publish(PUBID, itostring(m_pubtopic), g_Ltm.MakeSendStr(m_pubtopic, m_num_sn));
				cout << "*";
				LOG_IF(INFO, g_num_loglevel > 5) << "topic:" << m_pubtopic << "  sn:" << m_num_sn << "volum case publish success.";
				if (m_ch_code[8] == 'Z' || m_ch_code[8] == 'z')  //shenzhen stock
				{
					g_num_SZ_sn++;
				}
				else       //shanghai stock and others
				{
					g_num_SH_sn++;
				}
			}
		}
		return 0;
	}
};

int main(int argc, char* argv[])
{
	iblog_init(argv[0], CLOG_DIR);
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
			try{
				g_num_loglevel = atoi(g_map_configmap[VECTOR_CONFIGKEY[2]].c_str());
				strcpy_s(g_str_subendpoint, g_map_configmap[VECTOR_CONFIGKEY[3]].c_str());
				strcpy_s(g_str_pubendpoint, g_map_configmap[VECTOR_CONFIGKEY[4]].c_str());
				g_num_volum_limit = atoi(g_map_configmap[VECTOR_CONFIGKEY[5]].c_str());
				g_num_turnover_limit = atoi(g_map_configmap[VECTOR_CONFIGKEY[6]].c_str());
				g_num_extra_sell_volum_limit = atoi(g_map_configmap[VECTOR_CONFIGKEY[7]].c_str());
				g_num_extra_sell_turnover_limit = atoi(g_map_configmap[VECTOR_CONFIGKEY[8]].c_str());
				g_num_extra_buy_volum_limit = atoi(g_map_configmap[VECTOR_CONFIGKEY[9]].c_str());
				g_num_extra_buy_turnover_limit = atoi(g_map_configmap[VECTOR_CONFIGKEY[10]].c_str());
				cout << "get setting success.";
			}
			catch (exception ex)
			{
				return -1;
			}
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
	stage.AddActor(Proc_Actor);
	stage.Start();
	stage.Join();
	return 0;
}