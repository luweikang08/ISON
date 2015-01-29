#include "TradeAlert.h"

using namespace std;
using namespace ison::base;
using namespace rapidjson;
using namespace baseline;

const char* g_str_subid = "sub1";
const char* g_str_pubid = "pub1";

const char* g_clog_dir = "TALog";

char g_str_version[10] = ALERT_VERSION;
char g_str_releasedate[12] = ALERT_RELEASEDATE;
char g_str_subtopic[24] = ALERT_SUBTOPIC;       //topic, ex:1014,1114
char g_str_subendpoint[30] = ALERT_SUBENDPOINT;
char g_str_pubtopic[24] = ALERT_PUBTOPIC;       //topic, ex:2011,2111
char g_str_pubendpoint[30] = ALERT_PUBENDPOINT;

char* g_str_filepath = JSON_FILE_DEFAULT;
char* g_str_configkey[] = CONFIG_KEY_VECTOR_DEFAULT;
const char* g_str_configvalue[] = { ALERT_VERSION, ALERT_RELEASEDATE, ALERT_LOGLEVEL, ALERT_SUBTOPIC, ALERT_SUBENDPOINT, ALERT_PUBTOPIC, ALERT_PUBENDPOINT };
const int g_num_configvalue[] = { VOLUM_LIMIT, TURNOVER_LIMIT, EXTRA_SELL_VOLUM_LIMIT, EXTRA_SELL_TURNOVER_LIMIT, EXTRA_BUY_VOLUM_LIMIT, EXTRA_BUY_TURNOVER_LIMIT };

int g_num_loglevel = atoi(ALERT_LOGLEVEL);
int g_num_volum_limit = VOLUM_LIMIT;
int g_num_turnover_limit = TURNOVER_LIMIT;
int g_num_extra_sell_volum_limit = EXTRA_SELL_VOLUM_LIMIT;
int g_num_extra_sell_turnover_limit = EXTRA_SELL_TURNOVER_LIMIT;
int g_num_extra_buy_volum_limit = EXTRA_BUY_VOLUM_LIMIT;
int g_num_extra_buy_turnover_limit = EXTRA_BUY_TURNOVER_LIMIT;

unsigned g_num_ms;
unsigned g_num_tm;
unsigned g_num_SH_sn;
unsigned g_num_SZ_sn;

FILE* g_file;
vector<string> g_vector_configkey = CONFIG_KEY_VECTOR_DEFAULT;
map<string, string> g_map_configmap;
ison::signal::LargeTranMonitor ltm;

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
			if (m_str_subtopic_single.length()==4)
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
			string smss = e.message();
			string m_pubtopic_all;
			string m_pubtopic_single;
			int m_num_sn;
			char m_ch_code[16];
			
			m_pubtopic_all.append(g_str_pubtopic);

			char recvBuf[256];
			memcpy(recvBuf, smss.c_str(), smss.size());
			TOPICHEAD  * m_topichead_rec;
			m_topichead_rec = (TOPICHEAD*)recvBuf;

			baseline::SDS_Transaction  CC;
			baseline::MessageHeader hdr;
			int messageHeaderVersion = 0;
			hdr.wrap(recvBuf + sizeof(TOPICHEAD), 0, messageHeaderVersion, 256);//parse messageheader
			CC.wrapForDecode(recvBuf, sizeof(TOPICHEAD) + hdr.size(), hdr.blockLength(), hdr.version(), 256);

			LOG_IF(INFO, g_num_loglevel > 6) << "receive data->seqno:" << CC.seqno() << " code:" << m_ch_code << " date:" << CC.date() << " time:" << CC.time()\
				<< " sdstime:" << CC.sDSTime() << " sn:" << CC.sn() << " index:" << CC.index() << " price:" << CC.price() << " volume:" << CC.volume()\
				<< " turnover:" << CC.turnover() << " bsflag:" << CC.bSFlag() << " orderkind:" << CC.orderKind() << " functioncode:" << CC.functionCode()\
				<< " askorder" << CC.askOrder() << " bidorder:" << CC.bidOrder();

			if ((CC.time()<91500000)||(CC.time()>151000000))
			{
				LOG_IF(INFO, g_num_loglevel > 5) << "data time is not available.";
				return -1;
			}

			//sbe2struct(CC, g_TranData); //store data to struct
			std::memcpy(m_ch_code, CC.code(), 16);
			if ((m_ch_code[0] != '0') && (m_ch_code[0] != '3') && (m_ch_code[0] != '6'))
			{
				LOG_IF(INFO, g_num_loglevel > 6) << "stockid is not 0* 3* 6*.";
				return -1;
			}
			if (m_ch_code[8] == 'Z' || m_ch_code[8] == 'z')  //shenzhen stock
			{
				m_pubtopic_single = m_pubtopic_all.substr(0, 4);
				m_num_sn = g_num_SZ_sn;
			}
			else       //shanghai stock and others
			{
				m_pubtopic_single = m_pubtopic_all.substr(5, 4);
				m_num_sn = g_num_SH_sn;
			}

			baseline::MessageHeader hdr_send;
			baseline::SDS_Signal Signal;
			char sendBuf[256];
			if ((CC.volume() >= g_num_volum_limit) || (CC.turnover() >= g_num_turnover_limit))     //large buy and sell with volum||turnover
			{
				LOG_IF(INFO, g_num_loglevel > 5) << "volum||turnover warning:" << CC.volume();
				TOPICHEAD m_TopicHeadSend;

				m_TopicHeadSend.topic = atoi(m_pubtopic_single.c_str());
				DateAndTime m_dtm=GetDateAndTime();
				m_TopicHeadSend.ms = (m_dtm.time % 1000);
				m_TopicHeadSend.kw = atoi(m_ch_code);
				m_TopicHeadSend.sn = m_num_sn;
				DateTime2Second(m_dtm.date, m_dtm.time, g_num_tm);
				m_TopicHeadSend.tm = g_num_tm;
				memcpy(sendBuf, &m_TopicHeadSend, sizeof(TOPICHEAD));

				hdr_send.wrap(sendBuf, sizeof(TOPICHEAD), 0, 256)                          //wrap messageheader
					.blockLength(baseline::SDS_Signal::sbeBlockLength())
					.templateId(baseline::SDS_Signal::sbeTemplateId())
					.schemaId(baseline::SDS_Signal::sbeSchemaId())
					.version(baseline::SDS_Signal::sbeSchemaVersion());
				Signal.wrapForEncode(sendBuf, hdr.size() + sizeof(TOPICHEAD), 256);       //wrap data

				/*
				*9:15-9:25 is auction time in both,14:57-15:00 is auction in shenzhen
				*/
				if (CC.time() < 93000000 && CC.time()>91459999)
				{
					Signal.signalID(AUCTION_SIGNAID);
				}
				else if ((CC.time() >= 145700000) && (CC.time() < 150500000) && (m_ch_code[8] == 'Z'))
				{
					Signal.signalID(AUCTION_SIGNAID);
				}
				else if (CC.bSFlag() == 'B' || CC.bSFlag() == 'b') //put signalid in data
				{
					Signal.signalID(BUY_SIGNALID);
				}
				else if (CC.bSFlag() == 'S' || CC.bSFlag() == 's')
				{
					Signal.signalID(SELL_SIGNALID);
				}
				else
				{
					Signal.signalID(AUCTION_SIGNAID);
				}
				/*else
				{
					LOG(INFO) << "seqno:" << CC.seqno() << " code:" << m_ch_code << " date:" << CC.date() << " time:" << CC.time()\
						<< " sdstime:" << CC.sDSTime() << " sn:" << CC.sn() << " index:" << CC.index() << " price:" << CC.price() << " volume:" << CC.volume()\
						<< " turnover:" << CC.turnover() << " bsflag:" << CC.bSFlag() << " orderkind:" << CC.orderKind() << " functioncode:" << CC.functionCode()\
						<< " askorder" << CC.askOrder() << " bidorder:" << CC.bidOrder();
				}*/
				
				Signal.putCode(m_ch_code);
				Signal.date(CC.date());
				Signal.time(CC.time());
				string info;
				makeinfo(CC, info);
				Signal.putInfo(info.c_str());

				string m_str_SendMess(sendBuf, 256);//sizeof(TOPICHEAD) + sizeof(baseline::MessageHeader) + sizeof(baseline::SDS_Signal));
				Publish(g_str_pubid, m_pubtopic_single, m_str_SendMess);
				LOG_IF(INFO, g_num_loglevel > 5) << "topic:" << m_pubtopic_single << "  sn:" << m_num_sn << "volum case publish success.";
				if (m_ch_code[8] == 'Z' || m_ch_code[8] == 'z')  //shenzhen stock
				{
					g_num_SZ_sn++;
				}
				else       //shanghai stock and others
				{
					g_num_SH_sn++;
				}
			}

			//if ((g_TranData.BSFlag == 'S') && (g_TranData.Volum >= g_num_extra_sell_volum_limit))   //特大主动卖
			//{

			//}
			//if ((g_TranData.BSFlag == 'S') && (g_TranData.Turnover >= g_num_extra_sell_turnover_limit))
			//{

			//}
			//if ((g_TranData.BSFlag == 'B') && (g_TranData.Volum >= g_num_extra_buy_volum_limit))   //特大主动买
			//{

			//}
			//if ((g_TranData.BSFlag == 'B') && (g_TranData.Turnover >= g_num_extra_buy_turnover_limit))
			//{

			//}
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
		LOG(INFO) << m_jsonFileName<< ":read success.";
		strcpy_s(g_str_version, g_map_configmap["version"].c_str());
		strcpy_s(g_str_releasedate, g_map_configmap["releasedate"].c_str());
		if ((_stricmp(g_str_version, ALERT_VERSION) == 0) && (_stricmp(g_str_releasedate, ALERT_RELEASEDATE) == 0))  //Performs a lowercase comparison of strings.
		{
			g_num_loglevel = atoi(g_map_configmap["loglevel"].c_str());
			strcpy_s(g_str_subtopic, g_map_configmap["subtopic"].c_str());
			strcpy_s(g_str_subendpoint, g_map_configmap["subendpoint"].c_str());
			strcpy_s(g_str_pubtopic, g_map_configmap["pubtopic"].c_str());
			strcpy_s(g_str_pubendpoint, g_map_configmap["pubendpoint"].c_str());
			g_num_volum_limit = atoi(g_map_configmap["volum_limit"].c_str());
			g_num_turnover_limit = atoi(g_map_configmap["turnover_limit"].c_str());
			g_num_extra_sell_volum_limit = atoi(g_map_configmap["extra_sell_volum_limit"].c_str());
			g_num_extra_sell_turnover_limit = atoi(g_map_configmap["extra_sell_turnover_limit"].c_str());
			g_num_extra_buy_volum_limit = atoi(g_map_configmap["extra_buy_volum_limit"].c_str());
			g_num_extra_buy_turnover_limit = atoi(g_map_configmap["extra_buy_turnover_limit"].c_str());
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

		rapidjson::Value m_Value_ALERT;
		m_Value_ALERT.SetObject();
		rapidjson::Value m_Value_Temp;
		m_Value_Temp.SetObject();

		for (int i = 0; i < (sizeof(g_str_configvalue) / sizeof(g_str_configvalue[0])); i++)// add string to json
		{
			m_Value_Temp.SetString(ALERT_VERSION, strlen(ALERT_VERSION), m_Allocator);
			m_Value_ALERT.AddMember(g_str_configkey[i], g_str_configvalue[i], m_Allocator);
		}
		for (int i = (sizeof(g_str_configvalue) / sizeof(g_str_configvalue[0])), j = 0; i < (sizeof(g_str_configkey) / sizeof(g_str_configkey[0])); i++, j++)  //add int to json
		{
			m_Value_ALERT.AddMember(g_str_configkey[i], g_num_configvalue[j], m_Allocator);
		}

		m_Document.AddMember(argv[0], m_Value_ALERT, m_Allocator);
		typedef rapidjson::GenericStringBuffer<rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator<>> StringBuffer;
		StringBuffer buf(&m_Allocator);
		rapidjson::Writer<StringBuffer> writer(buf);
		m_Document.Accept(writer);
		std::string json(buf.GetString(), buf.Size());

		ofs << json;
		ofs.close();

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