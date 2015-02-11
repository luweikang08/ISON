#include "POIndicator.h"

using namespace std;
using namespace ison::base;

char g_str_version[10];
char g_str_releasedate[12];
char g_str_subendpoint_klo[30];
char g_str_pubendpoint[30];

unsigned int g_num_loglevel;

unsigned g_num_SH_sn;
unsigned g_num_SZ_sn;

map<string, string> g_map_configmap;

ison::indicator::MacdBase g_Macd;
ison::indicator::KdjBase g_Kdj;
ison::indicator::RsiBase g_Rsi;
ison::indicator::BollBase g_Boll;

class SubActor : public Actor {
public:
	SubActor(const std::string& id) : Actor(id) {}
	int OnStart(ActorContext&)
	{
		Subscribe(SUBID_OF_SUB, itostring(ISON_SDS2TGW::TSZ_LEVEL2));
		Subscribe(SUBID_OF_SUB, itostring(ISON_SDS2TGW::TSH_LEVEL2));
		Subscribe(SUBID_OF_SUB, itostring(ISON_TRADEPUBTOPIC::TSZ_KLINEONEMINUTE));
		Subscribe(SUBID_OF_SUB, itostring(ISON_TRADEPUBTOPIC::TSH_KLINEONEMINUTE));
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
			std::memcpy(buffer, smss.c_str(), smss.size());

			TOPICHEAD* m_TopicHead;
			m_TopicHead = (TOPICHEAD*)buffer;
			Publish(PUBID_OF_SUB, itostring(m_TopicHead->topic), smss);
		}
		return 0;
	}
};

class MacdActor : public Actor {
public:
	MacdActor(const std::string& id) : Actor(id) {}
	int OnStart(ActorContext&)
	{
		Subscribe(SUBID_OF_PROC, itostring(ISON_TRADEPUBTOPIC::TSZ_KLINEONEMINUTE));
		Subscribe(SUBID_OF_PROC, itostring(ISON_TRADEPUBTOPIC::TSH_KLINEONEMINUTE));
		cout << "MacdActor start ok" << endl;
		LOG(INFO) << "MacdActor start ok";
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
			if (g_Macd.Store(e.message()) == MACD_STORE_RetCode::MACD_REPLACE)
			{
				std::memcpy(m_ch_code, g_Macd.GetDataCode(), 16);//new
				if (m_ch_code[8] == 'Z' || m_ch_code[8] == 'z')  //shenzhen stock
				{
					m_pubtopic = ISON_TRADEPUBTOPIC::TSZ_POINDICATOR;
					m_num_sn = g_num_SZ_sn;
				}
				else       //shanghai stock and others
				{
					m_pubtopic = ISON_TRADEPUBTOPIC::TSH_POINDICATOR;
					m_num_sn = g_num_SH_sn;
				}
				std::string m_SendStr = g_Macd.MakeSendStr(m_pubtopic, m_num_sn);
				Publish(PUBID_OF_PROC, itostring(m_pubtopic), m_SendStr);

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

class KdjActor : public Actor {
public:
	KdjActor(const std::string& id) : Actor(id) {}
	int OnStart(ActorContext&)
	{
		Subscribe(SUBID_OF_PROC, itostring(ISON_TRADEPUBTOPIC::TSZ_KLINEONEMINUTE));
		Subscribe(SUBID_OF_PROC, itostring(ISON_TRADEPUBTOPIC::TSH_KLINEONEMINUTE));
		cout << "KdjActor start ok" << endl;
		LOG(INFO) << "KdjActor start ok";
		return 0;
	}
	int OnEvent(Event& e)
	{
		char m_ch_code[16];
		int m_pubtopic;
		int m_num_sn;

		if (e.type() == kPublish)
		{
			TOPICHEAD * A;
			baseline::MessageHeader hdr;
			baseline::SDS_Kline KK;
			char recBuffer[BUFFELENGTH];
			std::memcpy(recBuffer, e.message().c_str(), e.message().size());
			/*hdr.wrap(recBuffer, 0, MESSAGEHEADERVERSION, BUFFELENGTH);
			KK.wrapForDecode(recBuffer, 0, hdr.blockLength(), hdr.version(), BUFFELENGTH);*/
			A = (TOPICHEAD*)recBuffer;
			//cout << A->kw << endl;
			if (A->kw == 600446)
			{
				cout << ".";
				if (g_Kdj.Store(e.message()) == KDJ_STORE_RetCode::KDJ_REPLACE)
				{
					std::memcpy(m_ch_code, g_Kdj.GetDataCode(), 16);//new
					if (m_ch_code[8] == 'Z' || m_ch_code[8] == 'z')  //shenzhen stock
					{
						m_pubtopic = ISON_TRADEPUBTOPIC::TSZ_POINDICATOR;
						m_num_sn = g_num_SZ_sn;
					}
					else       //shanghai stock and others
					{
						m_pubtopic = ISON_TRADEPUBTOPIC::TSH_POINDICATOR;
						m_num_sn = g_num_SH_sn;
					}
					std::string m_SendStr = g_Kdj.MakeSendStr(m_pubtopic, m_num_sn);
					Publish(PUBID_OF_PROC, itostring(m_pubtopic), m_SendStr);

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

		}
		return 0;
	}
};


class RsiActor : public Actor {
public:
	RsiActor(const std::string& id) : Actor(id) {}
	int OnStart(ActorContext&)
	{
		Subscribe(SUBID_OF_PROC, itostring(ISON_TRADEPUBTOPIC::TSZ_KLINEONEMINUTE));
		Subscribe(SUBID_OF_PROC, itostring(ISON_TRADEPUBTOPIC::TSH_KLINEONEMINUTE));
		cout << "RsiActor start ok" << endl;
		LOG(INFO) << "RsiActor start ok";
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
			if (g_Rsi.Store(e.message()) == RSI_STORE_RetCode::RSI_REPLACE)
			{
				std::memcpy(m_ch_code, g_Rsi.GetDataCode(), 16);//new
				if (m_ch_code[8] == 'Z' || m_ch_code[8] == 'z')  //shenzhen stock
				{
					m_pubtopic = ISON_TRADEPUBTOPIC::TSZ_POINDICATOR;
					m_num_sn = g_num_SZ_sn;
				}
				else       //shanghai stock and others
				{
					m_pubtopic = ISON_TRADEPUBTOPIC::TSH_POINDICATOR;
					m_num_sn = g_num_SH_sn;
				}
				std::string m_SendStr = g_Rsi.MakeSendStr(m_pubtopic, m_num_sn);
				Publish(PUBID_OF_PROC, itostring(m_pubtopic), m_SendStr);

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

class BollActor : public Actor {
public:
	BollActor(const std::string& id) : Actor(id) {}
	int OnStart(ActorContext&)
	{
		Subscribe(SUBID_OF_PROC, itostring(ISON_TRADEPUBTOPIC::TSZ_KLINEONEMINUTE));
		Subscribe(SUBID_OF_PROC, itostring(ISON_TRADEPUBTOPIC::TSH_KLINEONEMINUTE));
		cout << "BollActor start ok" << endl;
		LOG(INFO) << "BollActor start ok";
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
			if (g_Boll.Store(e.message()) == BOLL_STORE_RetCode::BOLL_REPLACE)
			{
				std::memcpy(m_ch_code, g_Boll.GetDataCode(), 16);//new
				if (m_ch_code[8] == 'Z' || m_ch_code[8] == 'z')  //shenzhen stock
				{
					m_pubtopic = ISON_TRADEPUBTOPIC::TSZ_POINDICATOR;
					m_num_sn = g_num_SZ_sn;
				}
				else       //shanghai stock and others
				{
					m_pubtopic = ISON_TRADEPUBTOPIC::TSH_POINDICATOR;
					m_num_sn = g_num_SH_sn;
				}
				std::string m_SendStr = g_Boll.MakeSendStr(m_pubtopic, m_num_sn);
				Publish(PUBID_OF_PROC, itostring(m_pubtopic), m_SendStr);

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
		Subscribe(SUBID_OF_PUB, itostring(ISON_TRADEPUBTOPIC::TSZ_POINDICATOR));
		Subscribe(SUBID_OF_PUB, itostring(ISON_TRADEPUBTOPIC::TSH_POINDICATOR));
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
			std::memcpy(buffer, smss.c_str(), smss.size());

			TOPICHEAD* m_TopicHead;
			m_TopicHead = (TOPICHEAD*)buffer;
			Publish(PUBID_OF_PUB, itostring(m_TopicHead->topic), smss);
			cout << "x";
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
				strcpy_s(g_str_subendpoint_klo, g_map_configmap[VECTOR_CONFIGKEY[3]].c_str());
				strcpy_s(g_str_pubendpoint, g_map_configmap[VECTOR_CONFIGKEY[4]].c_str());
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

	stage_sub.AddSubscriber(SUBID_OF_SUB, g_str_subendpoint_klo);
	stage_sub.AddPublisher(PUBID_OF_SUB, STR_EP_SUB);
	stage_proc.AddSubscriber(SUBID_OF_PROC, STR_EP_SUB);
	stage_proc.AddPublisher(PUBID_OF_PROC, STR_EP_PUB);
	stage_pub.AddSubscriber(SUBID_OF_PUB, STR_EP_PUB);
	stage_pub.AddPublisher(PUBID_OF_PUB, g_str_pubendpoint);

	ActorPtr Sub_Actor(new SubActor("sub"));
	stage_sub.AddActor(Sub_Actor);
	//ActorPtr Macd_Actor(new MacdActor("macd"));
	//stage_proc.AddActor(Macd_Actor);
	ActorPtr Kdj_Actor(new KdjActor("kdj"));
	stage_proc.AddActor(Kdj_Actor);
	ActorPtr Rsi_Actor(new KdjActor("rsi"));
	stage_proc.AddActor(Rsi_Actor);
	ActorPtr Boll_Actor(new KdjActor("boll"));
	stage_proc.AddActor(Boll_Actor);
	ActorPtr Pub_Actor(new PubActor("pub"));
	stage_pub.AddActor(Pub_Actor);

	stage_sub.Start();
	stage_proc.Start();
	stage_pub.Start();
	stage_sub.Join();
	stage_proc.Join();
	return 0;
}