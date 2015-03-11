#include "ShortElf.h"

using namespace std;
using namespace ison::base;
using namespace ison::signal;
using namespace baseline;

char g_str_version[10];
char g_str_releasedate[12];
char g_str_subendpoint_src[30];
char g_str_subendpoint_klo[30];
char g_str_pubendpoint[30];

unsigned int g_num_loglevel;
unsigned int g_num_volum_limit;
unsigned int g_num_turnover_limit;
unsigned int g_num_extra_sell_volum_limit;
unsigned int g_num_extra_sell_turnover_limit;
unsigned int g_num_extra_buy_volum_limit;
unsigned int g_num_extra_buy_turnover_limit;
double g_num_rise_limit;
double g_num_fall_limit;

unsigned g_num_SH_sn;
unsigned g_num_SZ_sn;

map<string, string> g_map_configmap;

LargeTranMonitor g_Ltm;
RapidMoveMinitor g_Rmm;
FluctuationMonitor g_Fm;

class SubActor : public Actor {
public:
	SubActor(const std::string& id) : Actor(id) {}
	int OnStart(ActorContext&)
	{
#ifdef NOTOPICHEAD
		Subscribe(SUBID_OF_SUB1, PreTopicLevel2);
		Subscribe(SUBID_OF_SUB1, PreTopicTrans);
		Subscribe(SUBID_OF_SUB2, PreTopicKline);
#else
		Subscribe(SUBID_OF_SUB1, itostring(ISON_SDS2TGW::TSZ_LEVEL2));
		Subscribe(SUBID_OF_SUB1, itostring(ISON_SDS2TGW::TSH_LEVEL2));
		Subscribe(SUBID_OF_SUB1, itostring(ISON_SDS2TGW::TSZ_TRANSACTION));
		Subscribe(SUBID_OF_SUB1, itostring(ISON_SDS2TGW::TSH_TRANSACTION));
		Subscribe(SUBID_OF_SUB2, itostring(ISON_TRADEPUBTOPIC::TSZ_KLINEONEMINUTE));
		Subscribe(SUBID_OF_SUB2, itostring(ISON_TRADEPUBTOPIC::TSH_KLINEONEMINUTE));
#endif
		cout << "SubActor start ok" << endl;
		LOG(INFO) << "SubActor start ok";
		return 0;
	}
	int OnEvent(Event& e)
	{
		if (e.type() == kPublish)
		{
			string smss;
			smss = e.message();
			char buffer[BUFFELENGTH];
			memcpy(buffer, smss.c_str(), smss.size());
#ifdef NOTOPICHEAD
			baseline::MessageHeader hdr;
			hdr.wrap(buffer,0,MESSAGEHEADERVERSION,BUFFELENGTH);
			if (hdr.templateId()<10)
			{
				if (VECTOR_TOPIC[hdr.templateId() - 1] == "")
				{
					LOG_IF(INFO, g_num_loglevel > 5) << "parase MessageHeader error.";
				}
				else
				{
					Publish(PUBID_OF_SUB, VECTOR_TOPIC[hdr.templateId() - 1], smss);
				}
			}
#else
			TOPICHEAD* m_TopicHead;
			m_TopicHead = (TOPICHEAD*) buffer;
			Publish(PUBID_OF_SUB, itostring(m_TopicHead->topic), smss);
#endif
		}
		return 0;
	}
};

class LtmActor : public Actor {
public:
	LtmActor(const std::string& id) : Actor(id) {}
	int OnStart(ActorContext&)
	{
#ifdef NOTOPICHEAD
		Subscribe(SUBID_OF_PROC, PreTopicTrans);
#else
		Subscribe(SUBID_OF_PROC, itostring(ISON_SDS2TGW::TSZ_TRANSACTION));
		Subscribe(SUBID_OF_PROC, itostring(ISON_SDS2TGW::TSH_TRANSACTION));
#endif
		cout << "LtmActor start ok" << endl;
		LOG(INFO) << "LtmActor start ok";
		return 0;
	}
	int OnEvent(Event& e)
	{
		if (e.type() == kPublish)
		{
			LOG_IF(INFO, g_num_loglevel > 6) << "Data receive.";
			int m_pubtopic;
			int m_num_sn;
			char m_ch_code[16];

			g_Ltm.Store(e.message());//new
			memcpy(m_ch_code, g_Ltm.GetDataCode(), 16);//new

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
					m_pubtopic = ISON_TRADEPUBTOPIC::TSZ_SHORTELF;
					m_num_sn = g_num_SZ_sn;
				}
				else       //shanghai stock and others
				{
					m_pubtopic = ISON_TRADEPUBTOPIC::TSH_SHORTELF;
					m_num_sn = g_num_SH_sn;
				}

				Publish(PUBID_OF_PROC, itostring(m_pubtopic), g_Ltm.MakeSendStr(m_pubtopic, m_num_sn));
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

class RmmActor : public Actor {
public:
	RmmActor(const std::string& id) : Actor(id) {}
	int OnStart(ActorContext&)
	{
#ifdef NOTOPICHEAD
		Subscribe(SUBID_OF_PROC, PreTopicKline);
#else
		Subscribe(SUBID_OF_PROC, itostring(ISON_TRADEPUBTOPIC::TSZ_KLINEONEMINUTE));
		Subscribe(SUBID_OF_PROC, itostring(ISON_TRADEPUBTOPIC::TSH_KLINEONEMINUTE));
#endif
		cout << "RmmActor start ok" << endl;
		LOG(INFO) << "RmmActor start ok";
		return 0;
	}
	int OnEvent(Event& e)
	{
		if (e.type() == kPublish)
		{
			LOG_IF(INFO, g_num_loglevel > 6) << "Data receive.";
			int m_pubtopic;
			int m_num_sn;
			char m_ch_code[16];

			if (g_Rmm.Store(e.message()) == RM_UPDATE)
			{
				g_Rmm.CompStatus(g_num_rise_limit, g_num_fall_limit);
				if (g_Rmm.IsNeedPub())
				{
					memcpy(m_ch_code, g_Rmm.GetDataCode(), 16);
					if ((m_ch_code[0] != '0') && (m_ch_code[0] != '3') && (m_ch_code[0] != '6'))
					{
						LOG_IF(INFO, g_num_loglevel > 6) << "stockid is not 0* 3* 6*.";
						return -1;
					}
					if (m_ch_code[8] == 'Z' || m_ch_code[8] == 'z')  //shenzhen stock
					{
						m_pubtopic = ISON_TRADEPUBTOPIC::TSZ_SHORTELF;
						m_num_sn = g_num_SZ_sn;
					}
					else       //shanghai stock and others
					{
						m_pubtopic = ISON_TRADEPUBTOPIC::TSH_SHORTELF;
						m_num_sn = g_num_SH_sn;
					}

					Publish(PUBID_OF_PROC, itostring(m_pubtopic), g_Rmm.MakeSendStr(m_pubtopic, m_num_sn, g_num_rise_limit, g_num_fall_limit));
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
				g_Rmm.ResetPubFlag();
			}
		}
		return 0;
	}
};

class FmActor : public Actor {
public:
	FmActor(const std::string& id) : Actor(id) {}
	int OnStart(ActorContext&)
	{
#ifdef NOTOPICHEAD
		Subscribe(SUBID_OF_PROC, PreTopicLevel2);
#else
		Subscribe(SUBID_OF_PROC, itostring(ISON_SDS2TGW::TSZ_LEVEL2));
		Subscribe(SUBID_OF_PROC, itostring(ISON_SDS2TGW::TSH_LEVEL2));
#endif
		cout << "FmActor start ok" << endl;
		LOG(INFO) << "FmActor start ok";
		return 0;
	}
	int OnEvent(Event& e)
	{
		if (e.type() == kPublish)
		{
			LOG_IF(INFO, g_num_loglevel > 6) << "Data receive.";
			int m_pubtopic;
			int m_num_sn;
			char m_ch_code[16];

			FM_STORE_RetCode rc = g_Fm.Store(e.message());//new
			if (rc == FM_HOLDHIGH || rc == FM_OPENHIGH || rc == FM_HOLDLOW || rc == FM_OPENLOW)
			{
				memcpy(m_ch_code, g_Fm.GetDataCode(), 16);//new

				if ((g_Fm.GetDataTime() < 91500000) || (g_Fm.GetDataTime() > 151000000))//new
				{
					LOG_IF(INFO, g_num_loglevel > 5) << "data time is not available.";
					return -1;
				}

				if ((m_ch_code[0] != '0') && (m_ch_code[0] != '3') && (m_ch_code[0] != '6'))
				{
					LOG_IF(INFO, g_num_loglevel > 6) << "stockid is not 0* 3* 6*.";
					return -1;
				}
				if (m_ch_code[8] == 'Z' || m_ch_code[8] == 'z')  //shenzhen stock
				{
					m_pubtopic = ISON_TRADEPUBTOPIC::TSZ_SHORTELF;
					m_num_sn = g_num_SZ_sn;
				}
				else       //shanghai stock and others
				{
					m_pubtopic = ISON_TRADEPUBTOPIC::TSH_SHORTELF;
					m_num_sn = g_num_SH_sn;
				}

				Publish(PUBID_OF_PROC, itostring(m_pubtopic), g_Fm.MakeSendStr(m_pubtopic, m_num_sn));
				LOG_IF(INFO, g_num_loglevel > 5) << "topic:" << m_pubtopic << "  sn:" << m_num_sn << "high/low limit case publish success.";
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


class PubActor : public Actor {
public:
	PubActor(const std::string& id) : Actor(id) {}
	int OnStart(ActorContext&)
	{
		Subscribe(SUBID_OF_PUB, itostring(ISON_TRADEPUBTOPIC::TSZ_SHORTELF));
		Subscribe(SUBID_OF_PUB, itostring(ISON_TRADEPUBTOPIC::TSH_SHORTELF));
		cout << "PubActor start ok" << endl;
		LOG(INFO) << "PubActor start ok";
		return 0;
	}
	int OnEvent(Event& e)
	{
		if (e.type() == kPublish)
		{
			string smss;
			smss = e.message();
			char buffer[BUFFELENGTH];
			memcpy(buffer, smss.c_str(), smss.size());

			TOPICHEAD* m_TopicHead;
			m_TopicHead = (TOPICHEAD*)buffer;
			Publish(PUBID_OF_PUB, itostring(m_TopicHead->topic), smss);
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
				strcpy_s(g_str_subendpoint_src, g_map_configmap[VECTOR_CONFIGKEY[3]].c_str());
				strcpy_s(g_str_subendpoint_klo, g_map_configmap[VECTOR_CONFIGKEY[4]].c_str());
				strcpy_s(g_str_pubendpoint, g_map_configmap[VECTOR_CONFIGKEY[5]].c_str());
				g_num_volum_limit = atoi(g_map_configmap[VECTOR_CONFIGKEY[6]].c_str());
				g_num_turnover_limit = atoi(g_map_configmap[VECTOR_CONFIGKEY[7]].c_str());
				g_num_extra_sell_volum_limit = atoi(g_map_configmap[VECTOR_CONFIGKEY[8]].c_str());
				g_num_extra_sell_turnover_limit = atoi(g_map_configmap[VECTOR_CONFIGKEY[9]].c_str());
				g_num_extra_buy_volum_limit = atoi(g_map_configmap[VECTOR_CONFIGKEY[10]].c_str());
				g_num_extra_buy_turnover_limit = atoi(g_map_configmap[VECTOR_CONFIGKEY[11]].c_str());
				g_num_rise_limit = atof(g_map_configmap[VECTOR_CONFIGKEY[12]].c_str());
				g_num_fall_limit = atof(g_map_configmap[VECTOR_CONFIGKEY[13]].c_str());
				cout << "get setting success." << endl;
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
	Stage stage_sub(ctx);
	Stage stage_proc(ctx);
	Stage stage_pub(ctx);

	stage_sub.AddSubscriber(SUBID_OF_SUB1, g_str_subendpoint_src);
	stage_sub.AddSubscriber(SUBID_OF_SUB2, g_str_subendpoint_klo);
	stage_sub.AddPublisher(PUBID_OF_SUB, STR_EP_SUB);
	stage_proc.AddSubscriber(SUBID_OF_PROC, STR_EP_SUB);
	stage_proc.AddPublisher(PUBID_OF_PROC, STR_EP_PUB);
	stage_pub.AddSubscriber(SUBID_OF_PUB, STR_EP_PUB);
	stage_pub.AddPublisher(PUBID_OF_PUB, g_str_pubendpoint);

	ActorPtr Sub_Actor(new SubActor("sub"));
	stage_sub.AddActor(Sub_Actor);
	ActorPtr Ltm_Actor(new LtmActor("ltm"));
	stage_proc.AddActor(Ltm_Actor);
	ActorPtr Rmm_Actor(new RmmActor("rmm"));
	stage_proc.AddActor(Rmm_Actor);
	ActorPtr Fm_Actor(new FmActor("fm"));
	stage_proc.AddActor(Fm_Actor);
	ActorPtr Pub_Actor(new PubActor("pub"));
	stage_pub.AddActor(Pub_Actor);
	
	stage_sub.Start();
	stage_proc.Start();
	stage_pub.Start();
	stage_sub.Join();
	stage_proc.Join();
	stage_pub.Join();
	return 0;
}