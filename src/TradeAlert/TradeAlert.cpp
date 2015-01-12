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
char g_str_subtopic[12] = ALERT_SUBTOPIC;
char g_str_subendpoint[30] = ALERT_SUBENDPOINT;
char g_str_pubtopic[12] = ALERT_PUBTOPIC;
char g_str_pubendpoint[30] = ALERT_PUBENDPOINT;

char* g_str_filepath = JSON_FILE_DEFAULT;
char* g_str_configkey[] = CONFIG_KEY_VECTOR_DEFAULT;
const char* g_str_configvalue[] = { ALERT_VERSION, ALERT_RELEASEDATE, ALERT_LOGLEVEL, ALERT_SUBTOPIC, ALERT_SUBENDPOINT, ALERT_PUBTOPIC, ALERT_PUBENDPOINT };

int g_num_loglevel = 0;
int g_num_volum_limit = volum_limit_level1;
int g_num_turnover_limit = turnover_limit_level1;
int g_num_extra_sell_volum_limit = volum_limit_level1;
int g_num_extra_sell_turnover_limit = turnover_limit_level1;
int g_num_extra_buy_volum_limit = volum_limit_level1;
int g_num_extra_buy_turnover_limit = turnover_limit_level1;

unsigned g_num_ms;
unsigned g_num_tm;
unsigned g_num_sn;

FILE* g_file;
vector<string> g_vector_configkey = CONFIG_KEY_VECTOR_DEFAULT;
map<string, string> g_map_configmap;

TranData g_TranData;

class ProcActor : public Actor {
public:
	ProcActor(const std::string& id) : Actor(id) {}
	int OnStart(ActorContext&)
	{
		//if (g_str_subtopic[5])
		Subscribe(g_str_subid, g_str_subtopic);
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

			char recvBuf[1024];
			memcpy(recvBuf, smss.c_str(), smss.size());
			/*TOPICHEAD  * A;
			A = (TOPICHEAD*)recvBuf;*/

			baseline::SDS_Transaction  CC;
			baseline::MessageHeader hdr;
			int messageHeaderVersion = 0;
			hdr.wrap(recvBuf + sizeof(TOPICHEAD), 0, messageHeaderVersion, 1024);//parse messageheader
			CC.wrapForDecode(recvBuf, sizeof(TOPICHEAD) + hdr.size(), hdr.blockLength(), hdr.version(), 1024);

			sbe2struct(CC, g_TranData); //store data to struct

			baseline::MessageHeader hdr_send;
			baseline::SDS_Signal Signal;
			char sendBuf[256];
			if (g_TranData.Volum >= g_num_volum_limit)     //large buy and sell with volum
			{
				//printf("volum warning:%d\n", g_TranData.Volum);
				LOG_IF(INFO, g_num_loglevel > 5) << "volum warning:" << g_TranData.Volum;
				TOPICHEAD m_TopicHeadSend;
				m_TopicHeadSend.topic = atoi(ALERT_PUBTOPIC);
				DateAndTime m_dtm=GetDateAndTime();
				m_TopicHeadSend.ms = (m_dtm.time % 1000);
				m_TopicHeadSend.kw = atoi(g_TranData.Code);
				m_TopicHeadSend.sn = g_num_sn;
				DateTime2Second(m_dtm.date, m_dtm.time, g_num_tm);
				m_TopicHeadSend.tm = g_num_tm;
				memcpy(sendBuf, &m_TopicHeadSend, sizeof(TOPICHEAD));

				hdr_send.wrap(sendBuf, sizeof(TOPICHEAD), 0, 256)                          //wrap messageheader
					.blockLength(baseline::SDS_Signal::sbeBlockLength())
					.templateId(baseline::SDS_Signal::sbeTemplateId())
					.schemaId(baseline::SDS_Signal::sbeSchemaId())
					.version(baseline::SDS_Signal::sbeSchemaVersion());
				Signal.wrapForEncode(sendBuf, hdr.size() + sizeof(TOPICHEAD), 256); //wrap data
				Signal.signalID(SIGNALID);
				Signal.putCode(g_TranData.Code);
				Signal.date(g_TranData.Date);
				Signal.time(g_TranData.Time);
				string info;
				makeinfo(CC, info);
				Signal.putInfo(info.c_str());

				string m_str_SendMess(sendBuf, 256);//sizeof(TOPICHEAD) + sizeof(baseline::MessageHeader) + sizeof(baseline::SDS_Signal));
				Publish(g_str_pubid, g_str_pubtopic, m_str_SendMess);
				LOG_IF(INFO, g_num_loglevel > 5) << "sn:" << g_num_sn << " publish success.";
				g_num_sn++;
			}
			//if (g_TranData.Turnover >= g_num_turnover_limit)     //large buy and sell with turnover
			//{
			//	printf("turnover warning:%d\n", g_TranData.Turnover);
			//	Publish(g_str_pubid, g_str_pubtopic, smss);
			//}
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

	//std::vector<string> configkey = CONFIG_KEY_VECTOR_DEFAULT;
	//std::map<string, string> configmap;
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
		for (int i = (sizeof(g_str_configvalue) / sizeof(g_str_configvalue[0])); i < (sizeof(g_str_configkey) / sizeof(g_str_configkey[0])); i++)  //add int to json
		{
			m_Value_ALERT.AddMember(g_str_configkey[i], volum_limit_level1, m_Allocator);
		}

		m_Document.AddMember(argv[0], m_Value_ALERT, m_Allocator);
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

	SnInit(g_num_sn);

	Context ctx;
	Stage stage(ctx);

	stage.AddSubscriber(g_str_subid, g_str_subendpoint);
	stage.AddPublisher(g_str_pubid, g_str_pubendpoint);
	ActorPtr aProcActor(new ProcActor("proc"));
	stage.AddActor(aProcActor);
	stage.Start();
	stage.Join();
	return 0;
}