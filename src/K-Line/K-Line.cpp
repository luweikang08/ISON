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

FILE*      g_file_outputfile;         //输出文本文件，用于测试
time_t     g_timenow;            //输出时间，用于测试
int        g_count;              //计算1000次接收时间，用于测试
SYSTEMTIME g_time;               //输出时间，用于测试g_nSum
char       g_chtimenow[20];      //输出时间，用于测试

int g_num_DataStoreCnt = 0;          //存储一分钟内接收的数据，平均3秒左右一个数据
int g_num_KlineOneMinuteCnt = 0;     //存储1分钟线数据，242个数据/天
int g_num_KlineFiveMinuteCnt = 0;    //存储5分钟线数据，48个数据/天
int g_num_KlineTenMinuteCnt = 0;     //存储10分钟线数据，24个数据/天
int g_num_KlineFifteenMinutCnt = 0;  //存储15分钟线数据，16个数据/天
int g_num_KlineThirtyMinutCnt = 0;   //存储30分钟线数据，8个数据/天
int g_num_KlineOneHourCnt = 0;       //存储1小时线数据，4个数据/天

SDS_Level2 g_md_DataStore[g_num_DataStoreCntMax];                     //存储一分钟内的接收数据，用于统计
KLineData g_md_KlineOneMinute[g_num_KlineOneMinuteCntMax];            //存储一分钟内的统计数据，累计一天
KLineData g_md_KlineFiveMinute[g_num_KlineFiveMinuteCntMax];          //存储五分钟内的统计数据，累计一天
KLineData g_md_KlineTenMinute[g_num_KlineTenMinuteCntMax];            //存储十分钟
KLineData g_md_KlineFifteenMinut[g_num_KlineFifteenMinutCntMax];      //十五分钟
KLineData g_md_KlineThirtyMinut[g_num_KlineThirtyMinutCntMax];        //三十分钟
KLineData g_md_KlineOneHour[g_num_KlineOneHourCntMax];                //一小时

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

			if (Parase2Object(smss, g_md_DataStore[g_num_DataStoreCnt]) == -1) //判断解析是否成功，-1失败
			{
				printf("Parase fail!\n");
				return -1;
			}
			StockCode.append(g_md_DataStore[g_num_DataStoreCnt].Code);
			if (StockCode != "000001.SZ")       //判断是否是需要解析的股票代码
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
					g_num_DataStoreCnt++;   //保存9:30分前的一个到达的数据
					return -1;
				}
				else if (g_md_DataStore[g_num_DataStoreCnt].Time < 93000000) //如果在9:30之前收到额外数据，则抛弃之
				{
					return -1;
				}
			}
			if ((g_md_DataStore[g_num_DataStoreCnt].Time < 93000000) || (g_md_DataStore[g_num_DataStoreCnt].Time>150059999))//在9:30:00.000之前和15:00:59.999之后的数据不做处理
			{
				return -1;
			}
			printf("data:%s\n", e.message());
			if ((g_num_DataStoreCnt >= 2) &&
				(((g_md_DataStore[g_num_DataStoreCnt].Time / 100000) % 100) !=
				((g_md_DataStore[g_num_DataStoreCnt - 1].Time / 100000) % 100)))  //检测分钟位，判断数据是否跨越0秒，是则开始计算前一分钟中的平均值
			{
				KlineFromMaketData(0, g_num_DataStoreCnt, g_md_DataStore, g_md_KlineOneMinute[g_num_KlineOneMinuteCnt]);

				printf("1 time:%d,open:%d,high:%d,low:%d,close:%d,volume:%d,turnover:%d\n", g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Time, g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Open,g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].High,
					g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Low, g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Close, g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Volume, g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Turnover);
				fprintf(g_file_outputfile, "1 time:%d,open:%d,high:%d,low:%d,close:%d,volume:%d,turnover:%d\n", g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Time, g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Open, g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].High,
					g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Low, g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Close, g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Volume, g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Turnover);

				g_md_DataStore[0] = g_md_DataStore[g_num_DataStoreCnt - 1]; //下一分钟的第一个存为首个数据
				g_md_DataStore[1] = g_md_DataStore[g_num_DataStoreCnt];
				g_num_DataStoreCnt = 2;                                 //分钟内计数复位
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

				//计算5分钟线
				if ((g_num_KlineOneMinuteCnt > 0) && ((((g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Time / 100000) % 100 + 1) % 5) == 0)  //分钟位对5求余
					&& ((g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Time/100000)!=1129))   //时间是1129的话，不处理，将数据全部累计给Timer处理
				{
					KlineFromKline((g_num_KlineOneMinuteCnt > 4 ? g_num_KlineOneMinuteCnt - 4 : 0), g_num_KlineOneMinuteCnt, g_md_KlineOneMinute, g_md_KlineFiveMinute[g_num_KlineFiveMinuteCnt]);
					fprintf(g_file_outputfile, "5 time:%d,open:%d,high:%d,low:%d,close:%d,volume:%d,turnover:%d\n", g_md_KlineFiveMinute[g_num_KlineFiveMinuteCnt].Time, g_md_KlineFiveMinute[g_num_KlineFiveMinuteCnt].Open, g_md_KlineFiveMinute[g_num_KlineFiveMinuteCnt].High,
						g_md_KlineFiveMinute[g_num_KlineFiveMinuteCnt].Low, g_md_KlineFiveMinute[g_num_KlineFiveMinuteCnt].Close, g_md_KlineFiveMinute[g_num_KlineFiveMinuteCnt].Volume, g_md_KlineFiveMinute[g_num_KlineFiveMinuteCnt].Turnover);

					//计算10分钟线
					if((((g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Time / 100000) % 100 + 1) % 10) == 0)
					{
						KlineFromKline((g_num_KlineFiveMinuteCnt > 0 ? g_num_KlineFiveMinuteCnt - 1 : 0), g_num_KlineFiveMinuteCnt, g_md_KlineFiveMinute, g_md_KlineTenMinute[g_num_KlineFiveMinuteCnt]);
						g_num_KlineTenMinuteCnt++;//10分钟线统计数据下标后移
					}
					//计算15分钟线
					if ((((g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Time / 100000) % 100 + 1) % 15) == 0)
					{
						KlineFromKline((g_num_KlineFiveMinuteCnt > 0 ? g_num_KlineFiveMinuteCnt - 2 : 0), g_num_KlineFiveMinuteCnt, g_md_KlineFiveMinute, g_md_KlineFifteenMinut[g_num_KlineFifteenMinutCnt]);
						g_num_KlineFifteenMinutCnt++;//15分钟线统计数据下标后移
					}
					//计算30分钟线
					if ((((g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Time / 100000) % 100 + 1) % 30) == 0)
					{
						KlineFromKline((g_num_KlineFiveMinuteCnt > 0 ? g_num_KlineFiveMinuteCnt - 5 : 0), g_num_KlineFiveMinuteCnt, g_md_KlineFiveMinute, g_md_KlineThirtyMinut[g_num_KlineThirtyMinutCnt]);
						g_num_KlineThirtyMinutCnt++;//30分钟线统计数据下标后移
					}
					//计算60分钟线
					if ((g_num_KlineOneMinuteCnt > 0) && ((((g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Time / 100000) % 100 + 1) % 60) == 0))
					{
						KlineFromKline((g_num_KlineFiveMinuteCnt > 0 ? g_num_KlineFiveMinuteCnt - 11 : 0), g_num_KlineFiveMinuteCnt, g_md_KlineFiveMinute, g_md_KlineOneHour[g_num_KlineOneHourCnt]);
						g_num_KlineOneHourCnt++;//1小时线统计数据下标后移
					}

					g_num_KlineFiveMinuteCnt++;   //5分钟线统计数据下标后移
				}

				g_num_KlineOneMinuteCnt++;  //1分钟线统计数据下标后移
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
		if ((m_DateAndTime.time >= 113100000 && m_DateAndTime.time < 125959999) || (m_DateAndTime.time >= 150100000)) //时间在此区间内时，发送最后一条消息,ps:获取数据时间判断不起作用(31分就没数据了)，所以用这个方法
		{
			if ((g_md_DataStore[g_num_DataStoreCnt].Time / 100000)<=1129) //最后一条数据发生在1130之前，强制加入1130分钟数据，数据全部为0，方便统计
			{
				KlineFromMaketData(0, g_num_DataStoreCnt, g_md_DataStore, g_md_KlineOneMinute[g_num_KlineOneMinuteCnt]);//处理1129分钟的数据
				g_num_KlineOneMinuteCnt++;
				
				strcpy_s(g_md_KlineOneMinute[g_num_KlineOneMinuteCnt].Code, g_md_KlineOneMinute[g_num_KlineOneMinuteCnt - 1].Code);//加入1130空数据
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
			g_md_DataStore[0] = g_md_DataStore[g_num_DataStoreCnt]; //113100000前的最后一个数据存为下一分钟的首个数据
			g_num_DataStoreCnt = 1;                                 //分钟内计数复位
			g_num_KlineOneMinuteCnt++;  //1分钟线统计数据下标后移

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
			//计算10分钟线
			KlineFromKline((g_num_KlineFiveMinuteCnt > 0 ? g_num_KlineFiveMinuteCnt - 1 : 0), g_num_KlineFiveMinuteCnt, g_md_KlineFiveMinute, g_md_KlineTenMinute[g_num_KlineFiveMinuteCnt]);
			g_num_KlineTenMinuteCnt++;//10分钟线统计数据下标后移
			//计算15分钟线
			KlineFromKline((g_num_KlineFiveMinuteCnt > 0 ? g_num_KlineFiveMinuteCnt - 2 : 0), g_num_KlineFiveMinuteCnt, g_md_KlineFiveMinute, g_md_KlineFifteenMinut[g_num_KlineFifteenMinutCnt]);
			g_num_KlineFifteenMinutCnt++;//15分钟线统计数据下标后移
			//计算30分钟线
			KlineFromKline((g_num_KlineFiveMinuteCnt > 0 ? g_num_KlineFiveMinuteCnt - 5 : 0), g_num_KlineFiveMinuteCnt, g_md_KlineFiveMinute, g_md_KlineThirtyMinut[g_num_KlineThirtyMinutCnt]);
			g_num_KlineThirtyMinutCnt++;//30分钟线统计数据下标后移
			//计算60分钟线
			KlineFromKline((g_num_KlineFiveMinuteCnt > 0 ? g_num_KlineFiveMinuteCnt - 11 : 0), g_num_KlineFiveMinuteCnt, g_md_KlineFiveMinute, g_md_KlineOneHour[g_num_KlineOneHourCnt]);
			g_num_KlineOneHourCnt++;//1小时线统计数据下标后移

			g_num_KlineFiveMinuteCnt++;   //5分钟线统计数据下标后移


		}
		return 0;
	}
};

int main(int argc, char* argv[]) {
	iblog_init(argv[0]);
	iblog_v(0);//自定义VLOG(v)时，v值小于此处设置值的语句才有输出

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