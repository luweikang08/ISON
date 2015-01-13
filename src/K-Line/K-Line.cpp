#include "K-Line.h"

using namespace ison::base;
using namespace std;
using namespace rapidjson;

const char*  g_str_subid = "sub1";
const char*  g_str_pubid = "pub1";

const char* g_clog_dir = "KLINELog";

char g_str_version[10] = KLINE_VERSION;
char g_str_releasedate[12] = KLINE_RELEASEDATE;
char g_str_subtopic[24] = KLINE_SUBTOPIC;       //topic, ex:1014,1114
char g_str_subendpoint[30] = KLINE_SUBENDPOINT;
char g_str_pubtopic[24] = KLINE_PUBTOPIC;       //topic, ex:2011,2111
char g_str_pubendpoint[30] = KLINE_PUBENDPOINT;

char* g_str_filepath = JSON_FILE_DEFAULT;
char* g_str_configkey[] = CONFIG_KEY_VECTOR_DEFAULT;
const char* g_str_configvalue[] = { KLINE_VERSION, KLINE_RELEASEDATE, KLINE_LOGLEVEL, KLINE_SUBTOPIC, KLINE_SUBENDPOINT, KLINE_PUBTOPIC, KLINE_PUBENDPOINT };

int g_num_loglevel = atoi(KLINE_LOGLEVEL);

unsigned g_num_ms;
unsigned g_num_tm;
unsigned g_num_SH_sn;
unsigned g_num_SZ_sn;

FILE* g_file;
vector<string> g_vector_configkey = CONFIG_KEY_VECTOR_DEFAULT;
map<string, string> g_map_configmap;

map<string, vector<int, int>> g_map_level2datastore; //string point to stockid,int point to time,int point to price


time_t     g_timenow;            //���ʱ�䣬���ڲ���
int        g_count;              //����1000�ν���ʱ�䣬���ڲ���
SYSTEMTIME g_time;               //���ʱ�䣬���ڲ���g_nSum
char       g_chtimenow[20];      //���ʱ�䣬���ڲ���

int g_num_DataStoreCnt = 0;          //�洢һ�����ڽ��յ����ݣ�ƽ��3������һ������
int g_num_KlineOneMinuteCnt = 0;     //�洢1���������ݣ�242������/��
int g_num_KlineFiveMinuteCnt = 0;    //�洢5���������ݣ�48������/��
int g_num_KlineTenMinuteCnt = 0;     //�洢10���������ݣ�24������/��
int g_num_KlineFifteenMinutCnt = 0;  //�洢15���������ݣ�16������/��
int g_num_KlineThirtyMinutCnt = 0;   //�洢30���������ݣ�8������/��
int g_num_KlineOneHourCnt = 0;       //�洢1Сʱ�����ݣ�4������/��

SDS_Level2 g_md_DataStore[g_num_DataStoreCntMax];                     //�洢һ�����ڵĽ������ݣ�����ͳ��
KLineData g_md_KlineOneMinute[g_num_KlineOneMinuteCntMax];            //�洢һ�����ڵ�ͳ�����ݣ��ۼ�һ��
KLineData g_md_KlineFiveMinute[g_num_KlineFiveMinuteCntMax];          //�洢������ڵ�ͳ�����ݣ��ۼ�һ��
KLineData g_md_KlineTenMinute[g_num_KlineTenMinuteCntMax];            //�洢ʮ����
KLineData g_md_KlineFifteenMinut[g_num_KlineFifteenMinutCntMax];      //ʮ�����
KLineData g_md_KlineThirtyMinut[g_num_KlineThirtyMinutCntMax];        //��ʮ����
KLineData g_md_KlineOneHour[g_num_KlineOneHourCntMax];                //һСʱ

class SubActor : public Actor {
public:
	SubActor(const std::string& id) : Actor(id) {}
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
		string smss;
		string StockCode;
		if (e.type() == kPublish)
		{
			smss = e.message();

#ifdef SBE_FORMAT //parase sbe data
			char recvBuf[1024];
			std::memcpy(recvBuf, smss.c_str(), smss.size());
			TOPICHEAD  * m_topichead_rec;
			m_topichead_rec = (TOPICHEAD*)recvBuf;

			baseline::SDS_Level2  CC;
			baseline::MessageHeader hdr;
			int messageHeaderVersion = 0;
			hdr.wrap(recvBuf + sizeof(TOPICHEAD), 0, messageHeaderVersion, 1024);//parse messageheader
			CC.wrapForDecode(recvBuf, sizeof(TOPICHEAD) + hdr.size(), hdr.blockLength(), hdr.version(), 1024);

#else JSON_FORMAT //parase json data
			if (Parase2Object(smss, g_md_DataStore[g_num_DataStoreCnt]) == -1) //�жϽ����Ƿ�ɹ���-1ʧ��
			{
				printf("Parase fail!\n");
				return -1;
			}

			StockCode.append(g_md_DataStore[g_num_DataStoreCnt].Code);
			if (StockCode != "000001.SZ")       //�ж��Ƿ�����Ҫ�����Ĺ�Ʊ����
			{
				return -1;
			}
			else
			{
				fprintf(g_file, "stop time:%s\n", g_chtimenow);
				fprintf(g_file, "data:%s\n", e.message());
				//printf("data:%s\n", e.message());
				cout << ".";
				if (g_num_DataStoreCnt < 1)
				{
					g_num_DataStoreCnt++;   //����9:30��ǰ��һ�����������
					return -1;
				}
				else if (g_md_DataStore[g_num_DataStoreCnt].Time < 93000000) //�����9:30֮ǰ�յ��������ݣ�������֮
				{
					return -1;
				}
			}
			if ((g_md_DataStore[g_num_DataStoreCnt].Time < 93000000) || (g_md_DataStore[g_num_DataStoreCnt].Time>150059999))//��9:30:00.000֮ǰ��15:00:59.999֮������ݲ�������
			{
				return -1;
			}
			printf("data:%s\n", e.message());
			if ((g_num_DataStoreCnt >= 2) &&
				(((g_md_DataStore[g_num_DataStoreCnt].Time / 100000) % 100) !=
				((g_md_DataStore[g_num_DataStoreCnt - 1].Time / 100000) % 100)))  //������λ���ж������Ƿ��Խ0�룬����ʼ����ǰһ�����е�ƽ��ֵ
			{
				KlineFromMaketData(0, g_num_DataStoreCnt, g_md_DataStore, g_md_KlineOneMinute[g_num_KlineOneMinuteCnt]);

				printf("1 time:%d,open:%d,high:%d,low:%d,close:%d,volume:%d,turnover:%d\n", g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Time, g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Open,g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].High,
					g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Low, g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Close, g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Volume, g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Turnover);
				fprintf(g_file, "1 time:%d,open:%d,high:%d,low:%d,close:%d,volume:%d,turnover:%d\n", g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Time, g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Open, g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].High,
					g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Low, g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Close, g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Volume, g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Turnover);

				g_md_DataStore[0] = g_md_DataStore[g_num_DataStoreCnt - 1]; //��һ���ӵĵ�һ����Ϊ�׸�����
				g_md_DataStore[1] = g_md_DataStore[g_num_DataStoreCnt];
				g_num_DataStoreCnt = 2;                                 //�����ڼ�����λ
				string m_str_message;
				KLineData2String(g_md_KlineOneMinute[g_num_KlineOneMinuteCnt], m_str_message);
				
				if (Publish(g_str_pubid, g_str_pubtopic, m_str_message))
				{
					printf("publish success.\n");
				}
				else
				{
					printf("publish fail.\n");
				}

				//����5������
				if ((g_num_KlineOneMinuteCnt > 0) && ((((g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Time / 100000) % 100 + 1) % 5) == 0)  //����λ��5����
					&& ((g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Time/100000)!=1129))   //ʱ����1129�Ļ���������������ȫ���ۼƸ�Timer����
				{
					KlineFromKline((g_num_KlineOneMinuteCnt > 4 ? g_num_KlineOneMinuteCnt - 4 : 0), g_num_KlineOneMinuteCnt, g_md_KlineOneMinute, g_md_KlineFiveMinute[g_num_KlineFiveMinuteCnt]);
					fprintf(g_file, "5 time:%d,open:%d,high:%d,low:%d,close:%d,volume:%d,turnover:%d\n", g_md_KlineFiveMinute[g_num_KlineFiveMinuteCnt].Time, g_md_KlineFiveMinute[g_num_KlineFiveMinuteCnt].Open, g_md_KlineFiveMinute[g_num_KlineFiveMinuteCnt].High,
						g_md_KlineFiveMinute[g_num_KlineFiveMinuteCnt].Low, g_md_KlineFiveMinute[g_num_KlineFiveMinuteCnt].Close, g_md_KlineFiveMinute[g_num_KlineFiveMinuteCnt].Volume, g_md_KlineFiveMinute[g_num_KlineFiveMinuteCnt].Turnover);

					//����10������
					if((((g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Time / 100000) % 100 + 1) % 10) == 0)
					{
						KlineFromKline((g_num_KlineFiveMinuteCnt > 0 ? g_num_KlineFiveMinuteCnt - 1 : 0), g_num_KlineFiveMinuteCnt, g_md_KlineFiveMinute, g_md_KlineTenMinute[g_num_KlineFiveMinuteCnt]);
						g_num_KlineTenMinuteCnt++;//10������ͳ�������±����
					}
					//����15������
					if ((((g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Time / 100000) % 100 + 1) % 15) == 0)
					{
						KlineFromKline((g_num_KlineFiveMinuteCnt > 0 ? g_num_KlineFiveMinuteCnt - 2 : 0), g_num_KlineFiveMinuteCnt, g_md_KlineFiveMinute, g_md_KlineFifteenMinut[g_num_KlineFifteenMinutCnt]);
						g_num_KlineFifteenMinutCnt++;//15������ͳ�������±����
					}
					//����30������
					if ((((g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Time / 100000) % 100 + 1) % 30) == 0)
					{
						KlineFromKline((g_num_KlineFiveMinuteCnt > 0 ? g_num_KlineFiveMinuteCnt - 5 : 0), g_num_KlineFiveMinuteCnt, g_md_KlineFiveMinute, g_md_KlineThirtyMinut[g_num_KlineThirtyMinutCnt]);
						g_num_KlineThirtyMinutCnt++;//30������ͳ�������±����
					}
					//����60������
					if ((g_num_KlineOneMinuteCnt > 0) && ((((g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Time / 100000) % 100 + 1) % 60) == 0))
					{
						KlineFromKline((g_num_KlineFiveMinuteCnt > 0 ? g_num_KlineFiveMinuteCnt - 11 : 0), g_num_KlineFiveMinuteCnt, g_md_KlineFiveMinute, g_md_KlineOneHour[g_num_KlineOneHourCnt]);
						g_num_KlineOneHourCnt++;//1Сʱ��ͳ�������±����
					}

					g_num_KlineFiveMinuteCnt++;   //5������ͳ�������±����
				}

				g_num_KlineOneMinuteCnt++;  //1������ͳ�������±����
				return 0;
			}

			g_num_DataStoreCnt++;
			if (g_num_DataStoreCnt >= g_num_DataStoreCntMax)
				g_num_DataStoreCnt = 0;
#endif
		}

		return 0;
	}
};

class TimerActor : public Actor {
public:
	TimerActor(const std::string& id) : Actor(id) {}
	int OnStart(ActorContext&)
	{
		AddTimer(60000);
		return 0;
	}
	int OnEvent(Event& e)
	{
		DateAndTime m_DateAndTime = GetDateAndTime();
		if ((m_DateAndTime.time >= 113100000 && m_DateAndTime.time < 125959999) || (m_DateAndTime.time >= 150100000)) //ʱ���ڴ�������ʱ���������һ����Ϣ,ps:��ȡ����ʱ���жϲ�������(31�־�û������)���������������
		{
			if ((g_md_DataStore[g_num_DataStoreCnt].Time / 100000)<=1129) //���һ�����ݷ�����1130֮ǰ��ǿ�Ƽ���1130�������ݣ�����ȫ��Ϊ0������ͳ��
			{
				KlineFromMaketData(0, g_num_DataStoreCnt, g_md_DataStore, g_md_KlineOneMinute[g_num_KlineOneMinuteCnt]);//����1129���ӵ�����
				g_num_KlineOneMinuteCnt++;
				
				strcpy_s(g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Code, g_md_KlineOneMinute[g_num_KlineOneMinuteCnt - 1].Code);//����1130������
				g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Date = g_md_KlineOneMinute[g_num_KlineOneMinuteCnt - 1].Date;
				g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Time = 113059999;
				g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].TimeStat = OneMinute;
				g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].PreClose = g_md_KlineOneMinute[g_num_KlineOneMinuteCnt - 1].Close;
				g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Open = g_md_KlineOneMinute[g_num_KlineOneMinuteCnt - 1].Close;
				g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Close = g_md_KlineOneMinute[g_num_KlineOneMinuteCnt - 1].Close;
				g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].High = g_md_KlineOneMinute[g_num_KlineOneMinuteCnt - 1].Close;
				g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Low = g_md_KlineOneMinute[g_num_KlineOneMinuteCnt - 1].Close;
				g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Volume = 0;
				g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Turnover = 0;
			}
			KlineFromMaketData(0, g_num_DataStoreCnt, g_md_DataStore, g_md_KlineOneMinute[g_num_KlineOneMinuteCnt]);
			g_md_DataStore[0] = g_md_DataStore[g_num_DataStoreCnt]; //113100000ǰ�����һ�����ݴ�Ϊ��һ���ӵ��׸�����
			g_num_DataStoreCnt = 1;                                 //�����ڼ�����λ
			g_num_KlineOneMinuteCnt++;  //1������ͳ�������±����

			//publish
			string m_str_message;
			KLineData2String(g_md_KlineOneMinute[g_num_KlineOneMinuteCnt], m_str_message);
			if (Publish(g_str_pubid, g_str_pubtopic, m_str_message))
			{
				printf("publish success.\n");
			}
			else
			{
				printf("publish fail.\n");
			}

			KlineFromKline((g_num_KlineOneMinuteCnt > 4 ? g_num_KlineOneMinuteCnt - 4 : 0), g_num_KlineOneMinuteCnt, g_md_KlineOneMinute, g_md_KlineFiveMinute[g_num_KlineFiveMinuteCnt]);
			//����10������
			KlineFromKline((g_num_KlineFiveMinuteCnt > 0 ? g_num_KlineFiveMinuteCnt - 1 : 0), g_num_KlineFiveMinuteCnt, g_md_KlineFiveMinute, g_md_KlineTenMinute[g_num_KlineFiveMinuteCnt]);
			g_num_KlineTenMinuteCnt++;//10������ͳ�������±����
			//����15������
			KlineFromKline((g_num_KlineFiveMinuteCnt > 0 ? g_num_KlineFiveMinuteCnt - 2 : 0), g_num_KlineFiveMinuteCnt, g_md_KlineFiveMinute, g_md_KlineFifteenMinut[g_num_KlineFifteenMinutCnt]);
			g_num_KlineFifteenMinutCnt++;//15������ͳ�������±����
			//����30������
			KlineFromKline((g_num_KlineFiveMinuteCnt > 0 ? g_num_KlineFiveMinuteCnt - 5 : 0), g_num_KlineFiveMinuteCnt, g_md_KlineFiveMinute, g_md_KlineThirtyMinut[g_num_KlineThirtyMinutCnt]);
			g_num_KlineThirtyMinutCnt++;//30������ͳ�������±����
			//����60������
			KlineFromKline((g_num_KlineFiveMinuteCnt > 0 ? g_num_KlineFiveMinuteCnt - 11 : 0), g_num_KlineFiveMinuteCnt, g_md_KlineFiveMinute, g_md_KlineOneHour[g_num_KlineOneHourCnt]);
			g_num_KlineOneHourCnt++;//1Сʱ��ͳ�������±����

			g_num_KlineFiveMinuteCnt++;   //5������ͳ�������±����


		}
		return 0;
	}
};

int main(int argc, char* argv[]) 
{
	iblog_init(argv[0], g_clog_dir);
	iblog_v(0);//�Զ���VLOG(v)ʱ��vֵС�ڴ˴�����ֵ�����������

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
		if ((_stricmp(g_str_version, KLINE_VERSION) == 0) && (_stricmp(g_str_releasedate, KLINE_RELEASEDATE) == 0))  //Performs a lowercase comparison of strings.
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

		rapidjson::Value m_Value_KLINE;
		m_Value_KLINE.SetObject();
		rapidjson::Value m_Value_Temp;
		m_Value_Temp.SetObject();

		for (int i = 0; i < (sizeof(g_str_configvalue) / sizeof(g_str_configvalue[0])); i++)// add string to json
		{
			m_Value_Temp.SetString(KLINE_VERSION, strlen(KLINE_VERSION), m_Allocator);
			m_Value_KLINE.AddMember(g_str_configkey[i], g_str_configvalue[i], m_Allocator);
		}

		m_Document.AddMember(argv[0], m_Value_KLINE, m_Allocator);
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
	ActorPtr Sub_Actor(new SubActor("sub"));
	ActorPtr Timer_Actor(new TimerActor("timer"));
	stage.AddActor(Sub_Actor);
	stage.AddActor(Timer_Actor);
	stage.Start();
	stage.Join();
	return 0;
}