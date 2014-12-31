#include "K-Line.h"

using namespace ison::base;
using namespace std;
using namespace rapidjson;

const string g_str_connid = "con1";
const string g_str_endpoint = "tcp://127.0.0.1:7799";
//const char * g_str_filepath = "result.txt";
const char*  g_str_subendpoint = "tcp://192.168.15.200:7799";
const char*  g_str_subid = "sub1";
const char*  g_str_pubendpoint = "tcp://127.0.0.1:7711";
const char*  g_str_pubid = "pub1";
const char*  g_str_pubtopic = "88";

FILE*      g_file_outputfile;         //����ı��ļ������ڲ���
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
		Subscribe(g_str_subid, "83");
		//Subscribe(g_str_subid, "600446");
		cout << "test start ok" << endl;
		GetSystemTime(&g_time);
		sprintf_s(g_chtimenow, "%d%d%d %d%d%d %d", g_time.wYear, g_time.wMonth, g_time.wDay, g_time.wHour + 8, g_time.wMinute, g_time.wSecond, g_time.wMilliseconds);
		fprintf(g_file_outputfile, "start time:%s\n", g_chtimenow);
		return 0;
	}
	int OnEvent(Event& e)
	{
		string smss;
		string StockCode;
		//cout << e.message() << endl;
		if (e.type() == kPublish)
		{
			smss = e.message();
			
			//fprintf(g_file_outputfile, "data:%s\n", e.message());
			GetSystemTime(&g_time);
			sprintf_s(g_chtimenow, "%d%d%d %d%d%d %d", g_time.wYear, g_time.wMonth, g_time.wDay, g_time.wHour + 8, g_time.wMinute, g_time.wSecond, g_time.wMilliseconds);
			/*fprintf(g_file_outputfile, "stop time:%s\n", g_chtimenow);
			fprintf(g_file_outputfile, "data:%s\n", e.message());*/

			if (Parase2Object(smss, g_md_DataStore[g_num_DataStoreCnt]) == -1) //�жϽ����Ƿ�ɹ���-1ʧ��
			{
				printf("Parase fail!\n");
				return -1;
			}
			StockCode.append(g_md_DataStore[g_num_DataStoreCnt].Code);
			if (StockCode != "000001.SZ")       //�ж��Ƿ�����Ҫ�����Ĺ�Ʊ����
			{
				//printf("code match fail!%s\n", g_md_DataStore[g_num_DataStoreCnt].Code);
				return -1;
			}
			else
			{
				fprintf(g_file_outputfile, "stop time:%s\n", g_chtimenow);
				fprintf(g_file_outputfile, "data:%s\n", e.message());
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
				fprintf(g_file_outputfile, "1 time:%d,open:%d,high:%d,low:%d,close:%d,volume:%d,turnover:%d\n", g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Time, g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Open, g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].High,
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
					fprintf(g_file_outputfile, "5 time:%d,open:%d,high:%d,low:%d,close:%d,volume:%d,turnover:%d\n", g_md_KlineFiveMinute[g_num_KlineFiveMinuteCnt].Time, g_md_KlineFiveMinute[g_num_KlineFiveMinuteCnt].Open, g_md_KlineFiveMinute[g_num_KlineFiveMinuteCnt].High,
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
				g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].TimeStatus = OneMinute;
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

int main(int argc, char* argv[]) {
	iblog_init(argv[0]);
	iblog_v(0);//�Զ���VLOG(v)ʱ��vֵС�ڴ˴�����ֵ�����������

	Context ctx;
	Stage stage(ctx);

	string m_str_FilePath;
	GetSystemTime(&g_time);
	sprintf_s(g_chtimenow, "%d%d%d %d%d%d", g_time.wYear, g_time.wMonth, g_time.wDay, g_time.wHour + 8, g_time.wMinute, g_time.wSecond);
	m_str_FilePath.append("out");
	m_str_FilePath.append(g_chtimenow);
	m_str_FilePath.append(".txt");
	fopen_s(&g_file_outputfile,m_str_FilePath.c_str(), "a+");
	g_count = 0;

	stage.Bind("tcp://*:5555");
	stage.AddSubscriber(g_str_subid, g_str_subendpoint);
	stage.AddPublisher(g_str_pubid, g_str_pubendpoint);
	ActorPtr Sub_Actor(new SubActor("sub"));
	ActorPtr Timer_Actor(new TimerActor("timer"));
	stage.AddActor(Sub_Actor);
	stage.AddActor(Timer_Actor);
	//stage.Connect(g_str_connid, g_str_subendpoint);
	stage.Start();
	stage.Join();
	return 0;
}