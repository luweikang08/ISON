#include "K-Line.h"

using namespace ison::base;
using namespace std;
using namespace rapidjson;

const string g_str_connid = "con1";
const string g_str_endpoint = "tcp://127.0.0.1:7799";
//const char * g_str_filepath = "result.txt";
const char*  g_str_subendpoint = "tcp://192.168.15.200:7799";
const char*  g_str_subid = "sub1";

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

class ReplyActor : public Actor {
public:
	ReplyActor(const std::string& id) : Actor(id) {}
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
				printf("code match fail!%s\n", g_md_DataStore[g_num_DataStoreCnt].Code);
				return -1;
			}
			else
			{
				fprintf(g_file_outputfile, "stop time:%s\n", g_chtimenow);
				fprintf(g_file_outputfile, "data:%s\n", e.message());
				printf("data:%s\n", e.message());
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

				//����5������
				if ((g_num_KlineOneMinuteCnt > 0) && ((((g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Time / 100000) % 100 + 1) % 5) == 0)) //����λ��5����
				{
					KlineFromKline(g_num_KlineFiveMinuteCnt * 5, g_num_KlineOneMinuteCnt, g_md_KlineOneMinute, g_md_KlineFiveMinute[g_num_KlineFiveMinuteCnt]);
					fprintf(g_file_outputfile, "5 time:%d,open:%d,high:%d,low:%d,close:%d,volume:%d,turnover:%d\n", g_md_KlineFiveMinute[g_num_KlineFiveMinuteCnt].Time, g_md_KlineFiveMinute[g_num_KlineFiveMinuteCnt].Open, g_md_KlineFiveMinute[g_num_KlineFiveMinuteCnt].High,
						g_md_KlineFiveMinute[g_num_KlineFiveMinuteCnt].Low, g_md_KlineFiveMinute[g_num_KlineFiveMinuteCnt].Close, g_md_KlineFiveMinute[g_num_KlineFiveMinuteCnt].Volume, g_md_KlineFiveMinute[g_num_KlineFiveMinuteCnt].Turnover);

					g_num_KlineFiveMinuteCnt++;   //5������ͳ�������±����
				}

				////����10������
				if ((g_num_KlineOneMinuteCnt > 0) && ((((g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Time / 100000) % 100 + 1) % 10) == 0))
				{
					KlineFromKline(g_num_KlineTenMinuteCnt * 10, g_num_KlineOneMinuteCnt, g_md_KlineOneMinute, g_md_KlineTenMinute[g_num_KlineTenMinuteCnt]);
					g_num_KlineTenMinuteCnt++;//10������ͳ�������±����
				}
				////����15������
				//if ((g_num_KlineOneMinuteCnt > 0) && ((((g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Time / 100000) % 100 + 1) % 15) == 0))
				//{
				//	KlineFromKline(g_num_KlineFifteenMinutCnt * 15 + 1, g_num_KlineOneMinuteCnt, g_md_KlineOneMinute, g_md_KlineFifteenMinut[g_num_KlineFifteenMinutCnt]);
				//	g_num_KlineFifteenMinutCnt++;//15������ͳ�������±����
				//}
				////����30������
				//if ((g_num_KlineOneMinuteCnt > 0) && ((((g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Time / 100000) % 100 + 1) % 30) == 0))
				//{
				//	KlineFromKline(g_num_KlineThirtyMinutCnt * 10 + 1, g_num_KlineOneMinuteCnt, g_md_KlineOneMinute, g_md_KlineThirtyMinut[g_num_KlineThirtyMinutCnt]);
				//	g_num_KlineThirtyMinutCnt++;//30������ͳ�������±����
				//}
				////����60������
				//if ((g_num_KlineOneMinuteCnt > 0) && ((((g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Time / 100000) % 100 + 1) % 60) == 0))
				//{
				//	KlineFromKline(g_num_KlineOneHourCnt * 60 + 1, g_num_KlineOneMinuteCnt, g_md_KlineOneMinute, g_md_KlineOneHour[g_num_KlineOneHourCnt]);
				//	g_num_KlineOneHourCnt++;//1Сʱ��ͳ�������±����
				//}
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
	ActorPtr actor(new ReplyActor("aa"));
	stage.AddActor(actor);
	//stage.Connect(g_str_connid, g_str_subendpoint);
	stage.Start();
	stage.Join();
	return 0;
}