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
double g_num_riselimit = MOVE_RISELIMIT;
double g_num_falllimit = MOVE_FALLLIMIT;

unsigned g_num_ms;
unsigned g_num_tm;
unsigned g_num_SH_sn;
unsigned g_num_SZ_sn;

vector<string> g_vector_configkey = CONFIG_KEY_VECTOR_DEFAULT;
map<string, string> g_map_configmap;    //config file data
map<string, string> g_map_KlineDataOne; //<code,sbebuf>

//TranData g_TranData;

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
			int m_num_signalid;

			m_pubtopic_all.append(g_str_pubtopic);

			char recvBuf[512];
			memcpy(recvBuf, smss.c_str(), smss.size());
			TOPICHEAD  * m_topichead_rec;
			m_topichead_rec = (TOPICHEAD*)recvBuf;

			baseline::SDS_Kline  KK;
			baseline::MessageHeader hdr;
			int messageHeaderVersion = 0;
			hdr.wrap(recvBuf + sizeof(TOPICHEAD), 0, messageHeaderVersion, 512);//parse messageheader
			KK.wrapForDecode(recvBuf, sizeof(TOPICHEAD) + hdr.size(), hdr.blockLength(), hdr.version(), 512);

			//sbe2struct(KK, g_TranData); //store data to struct
			char m_ch_Code[16];
			memcpy(m_ch_Code, KK.code(), 16);
			if ((m_ch_Code[0] != '0') && (m_ch_Code[0] != '3') && (m_ch_Code[0] != '6'))
			{
				LOG_IF(INFO, g_num_loglevel > 6) << "stockid is not 0* 3* 6*.";
				return -1;
			}
			if (m_ch_Code[8] == 'Z' || m_ch_Code[8] == 'z')  //shenzhen stock
			{
				m_pubtopic_single = m_pubtopic_all.substr(0, 4);
				m_num_sn = g_num_SZ_sn;
			}
			else       //shanghai stock and others
			{
				m_pubtopic_single = m_pubtopic_all.substr(5, 4);
				m_num_sn = g_num_SH_sn;
			}

			map<string, string>::iterator it = g_map_KlineDataOne.find(KK.code()); //find curent stock is exist in map or not
			string m_str_Store(recvBuf + sizeof(TOPICHEAD), hdr.size() + KK.sbeBlockLength());
			if (it == g_map_KlineDataOne.end())
			{
				g_map_KlineDataOne.insert(pair<string, string>(KK.code(), m_str_Store));
			}
			else
			{
				char readBuf[512];
				std::memcpy(readBuf, it->second.c_str(), hdr.size() + KK.sbeBlockLength());

				baseline::MessageHeader hdr_read;
				baseline::SDS_Kline KK_read;
				hdr_read.wrap(readBuf, 0, messageHeaderVersion, 512);
				KK_read.wrapForDecode(readBuf, hdr_read.size(), KK_read.sbeBlockLength(), hdr.version(), 512);
				float temp = volat(KK_read.preClose(), KK.close());
				if (temp >= g_num_riselimit)
				{
					m_num_signalid = RISE_SIGNALID;
				}
				else if (temp <= (0 - g_num_falllimit))
				{
					m_num_signalid = FALL_SIGNALID;
				}
				else
				{
					return -1;
				}
				cout << "code:" << KK_read.code() << " " << KK.code() << "preclose:" << KK_read.preClose() << " close:" << KK.close();

				char sendBuf[256];
				TOPICHEAD m_TopicHeadSend;
				baseline::MessageHeader hdr_send;
				baseline::SDS_Signal signal_send;

				m_TopicHeadSend.topic = atoi(m_pubtopic_single.c_str());
				DateAndTime m_dtm = GetDateAndTime();
				m_TopicHeadSend.ms = (m_dtm.time % 1000);
				m_TopicHeadSend.kw = atoi(KK.code());
				m_TopicHeadSend.sn = m_num_sn;
				DateTime2Second(m_dtm.date, m_dtm.time, g_num_tm);
				m_TopicHeadSend.tm = g_num_tm;
				memcpy(sendBuf, &m_TopicHeadSend, sizeof(TOPICHEAD));

				hdr_send.wrap(sendBuf, sizeof(TOPICHEAD), 0, 256)                          //wrap messageheader
					.blockLength(baseline::SDS_Signal::sbeBlockLength())
					.templateId(baseline::SDS_Signal::sbeTemplateId())
					.schemaId(baseline::SDS_Signal::sbeSchemaId())
					.version(baseline::SDS_Signal::sbeSchemaVersion());
				signal_send.wrapForEncode(sendBuf, hdr.size() + sizeof(TOPICHEAD), 256);       //wrap data
				signal_send.signalID(m_num_signalid);
				signal_send.putCode(KK.code());
				signal_send.date(KK.date());
				signal_send.time(KK.time());
				string info;
				makeinfo(temp, info);
				signal_send.putInfo(info.c_str());

				string m_str_SendMess(sendBuf, sizeof(TOPICHEAD) + hdr_send.size() + signal_send.sbeBlockLength());
				Publish(g_str_pubid, m_pubtopic_single, m_str_SendMess);
				cout << "*";
				LOG_IF(INFO, g_num_loglevel > 5) << "topic:" << m_pubtopic_single << "  sn:" << m_num_sn << "volat case publish success.";
				if (m_ch_Code[8] == 'Z' || m_ch_Code[8] == 'z')  //shenzhen stock
				{
					g_num_SZ_sn++;
				}
				else       //shanghai stock and others
				{
					g_num_SH_sn++;
				}
				g_map_KlineDataOne[it->first] = m_str_Store; //store new data
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
			g_num_riselimit = atof(g_map_configmap["riselimit"].c_str());
			g_num_falllimit = atof(g_map_configmap["falllimit"].c_str());
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