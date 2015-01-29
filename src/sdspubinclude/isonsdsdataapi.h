#ifndef __ISON_SDS_DATA_API_H__ 
#define __ISON_SDS_DATA_API_H__

#include<vector>
#ifdef _WIN32
#ifndef __int64
#define __int64 long long
#endif
#define N8 __int64
#define N4 __int32
#define N2 __int16
#define C char
#else 
#define N8 long long
#define N4 int
#define N2 short
#define C char
#endif
#define LEVLEL2LENGTH 10
#define ABVOLUMELENGTH 50
#define CODELENGTH 16
#define CODENAMELENGTH 32
#define MARKETLENGTH 6
typedef  unsigned short un_short;
typedef unsigned int un_int;

//系统消息和数据消息的定义
enum ISON_MSG_ID{
	SYSMSG = 10,
	CodeTableMSG = 11,
	SYSCloseMarket = 12,

	
	SubscribeMarketData = 21,//订阅行情
	SubscribeMarketDataResp,//	订阅行情应答
	UnSubscribeMarketData,	//取消订阅市场行情
	UnSubscribeMarketDataResp,//	取消订阅市场行情应答
	SubscribeSignal,//订阅信号
	SubscribeSignalResp,//订阅信号应答
	UnSubscribeSignal,//取消订阅信号
	UnSubscribeSignalResp,//取消订阅信号应答
	SubscribeIndicator,//订阅指标
	SubscribeIndicatorResp,//订阅指标应答

	GetMarketInfo = 41,//查询市场信息
	GetMarketInfoResp,//查询市场信息应答
	GetStockInfo,//查询股票信息
	GetStockInfoResp,//查询股票信息应答
	GetMarketData,//查询市场行情
	GetMarketDataResp,//查询市场行情应答
	GetIndicator,//查询历史指标
	GetIndicatorResp,//查询历史指标应答
	GetMarketRank,//查询实时排行
	GetMarketRankResp,//查询实时排行应答
	GetCodeTable,
	GetCodeTableResp,


	MatchPrice = 81,//成交价格
	MaketData,//基本行情
	Level2,//Level2//行情
	Transaction,//逐笔成交
	Order,//逐笔委托
	OrderQueue,//委托队列
	Signal,//信号推送
	Indicator ,//指标推送

	
};

//错误码定义

enum SDS_ERR
{

	SDS_ERR_UNKOWN = 2000,                // 未知错误

	SDS_ERR_INITIALIZE_FAILURE = 2001,  // 初始化环境失败
	SDS_ERR_NETWORK_ERROR,              // 网络连接出现问题
	SDS_ERR_INVALID_PARAMS,             // 输入参数无效
	SDS_ERR_VERIFY_FAILURE,             // 登陆验证失败：原因为用户名或者密码错误；超出登陆数量
	SDS_ERR_NO_AUTHORIZED_MARKET,       // 所有请求的市场都没有授权
	SDS_ERR_NO_CODE_TABLE,              // 所有请求的市场该天都没有代码表

	SDS_ERR_SUCCESS = 0,                // 成功
};


/*
enum ISON_SDS2TGW{
	TSZ_SYSDATA = 1000,//深圳交易所	SYSDATA	系统数据	
	TSZ_LEVEL1DATA = 1001,	//深圳交易所	LEVEL1DATA	LEVEL1数据	
	TSZ_LEVEL2 = 1002,	//深圳交易所	LEVEL2DATA	LEVEL2数据
	TSZ_ORDERQUEUE = 1003,	//深圳交易所	ORDERQUEUE	订单队列	
	TSZ_TRANSACTION = 1004,	//深圳交易所	TRANSACTION	逐笔成交	

	TSH_SYSDATA = 1100,	//上海交易所	SYSDATA	系统数据	
	TSH_LEVEL1DATA = 1101,//上海交易所	LEVEL1DATA	LEVEL1数据	
	TSH_LEVEL2 = 1102,	//上海交易所	LEVEL2DATA	LEVEL2数据	
	TSH_ORDERQUEUE = 1103,	//上海交易所	ORDERQUEUE	订单队列	
	TSH_TRANSACTION = 1104	//上海交易所	TRANSACTION	逐笔成交	
};*/

enum ISON_SDS2TGW{
TSZ_SYSDATA = 1010,//深圳交易所	SYSDATA	系统数据
TSZ_LEVEL1DATA = 1011,	//深圳交易所	LEVEL1DATA	LEVEL1数据
TSZ_LEVEL2 = 1012,	//深圳交易所	LEVEL2DATA	LEVEL2数据
TSZ_ORDERQUEUE = 1013,	//深圳交易所	ORDERQUEUE	订单队列
TSZ_TRANSACTION = 1014,	//深圳交易所	TRANSACTION	逐笔成交

TSH_SYSDATA = 1110,	//上海交易所	SYSDATA	系统数据
TSH_LEVEL1DATA = 1111,//上海交易所	LEVEL1DATA	LEVEL1数据
TSH_LEVEL2 = 1112,	//上海交易所	LEVEL2DATA	LEVEL2数据
TSH_ORDERQUEUE = 1113,	//上海交易所	ORDERQUEUE	订单队列
TSH_TRANSACTION = 1114	//上海交易所	TRANSACTION	逐笔成交
};

struct TOPICHEAD{
	un_short topic; // 主题,见后面列表．
	un_short ms; // 毫秒0~999
	un_int kw;  // 证券代码，一般直接用atoi就可以了．
	un_int sn;  // 序列号,最好连续
	un_int tm;  // 从１９７１到现在的秒数．
};

struct SDS_Level2{
	N4	Seqno;	//消息编号
	C Code[CODELENGTH];	//证券代码, 如000001.SH
	N4 Date ;	//,日期YYYYMMDD
	N4 Time;	//时间HHMMSSmmm
	N4 SDSTime;//进入SDS的时间 //12.23
	N4 Sn;//序列号，用于测试数据是否丢失//12.23
	N4 PreClose; //前收盘价			
	N4 Open; // 开盘价
	N4 High; // 最高价
	N4	Low; // 最低价
	N4	Price; // 最新价
	N4 NumTrades;//成交笔数
	N8	Volume; // 成交总量
	N8  Turnover; //成交总金额
	N8  TotalBidVol;//委托买入总量  //add in 11-27
	N8 TotalAskVol;//委托卖出总量
	N4 AvgBidPrice;//加权平均委买价格
	N4 AvgAskPrice;//加权平均委卖价格
	N4 HighLimited;//涨停价
	N4 LowLimited;//跌停价        //add in 11-27

	N4	AskPrice[LEVLEL2LENGTH];	//10档申卖价
	N4	AskVol[LEVLEL2LENGTH];  //	10档申卖量
	N4	BidPrice[LEVLEL2LENGTH]; // 10档申买价
	N4	BidVol[LEVLEL2LENGTH]; //	10档申买量
};
struct SDS_Transaction{
	N4  Seqno;//消息编号
	C Code[CODELENGTH];//证券代码
	N4 	Date;//日期
	N4 Time;//时间
	N4 SDSTime;//进入SDS的时间 //12.23
	N4 Sn;//序列号，用于测试数据是否丢失//12.23
	N4 Index;//成交编号
	N4 Price;//成交价格
	N4 Volume;//成交数量
	N4 Turnover;//成交金额
	C BSFlag;//买卖方向
	C OrderKind;//成交类别
	N4 FunctionCode;//成交代码
	N4 AskOrder;//叫卖方委托序号
	N4 BidOrder;//叫买房委托序号		
};
struct SDS_Order{
	N4 Seqno;//
	C Code[CODELENGTH];
	N4 Date;
	N4 Time;
	N4 SDSTime;//进入SDS的时间 //12.23
	N4 Sn;//序列号，用于测试数据是否丢失//12.23
	N4 Order;
	N4 Price;
	N4 Volume;
	C OrderKind;
	C FunctionCode;
};
struct SDS_OderQueen{
	N4 Seqno;//消息编号
	C Code[CODELENGTH];//证券代码
	N4 Date;//日期
	N4 Time;//时间
	N4 SDSTime;//进入SDS的时间 //12.23
	N4 Sn;//序列号，用于测试数据是否丢失//12.23
	C Side;//买卖方向
	N4 Price;//委托价格
	N4 Orders;//订单数量
	N4 ABItems;//ABItems
	N4 ABVolume[ABVOLUMELENGTH];//ABVolume 
};
struct SDS_Code{
	C Code[CODELENGTH];    //Wind Code: AG1302.SHF
	C ENName[CODENAMELENGTH];
	C CNName[CODENAMELENGTH];      //chinese name: 沪银1302
};
struct SDS_CloseMarket{
	N4 Seqno;//消息编号  12
	N4 Time;//闭市时间  SH因为有港股所以16：00闭市
	C Market[MARKETLENGTH];//闭市市场SZ或者SH
};
struct SubscribeMarketDataReq{
	N4  seqno;     //	消息编号客户指令编号(不能重复)
	N2	date_type;//	订阅的数据类型
	N4 	begin_date;//	开始日期 YYMMDD， 0为当天
	N4	begin_time;	//开始时间	HHMMSSmmm，0为当时
	N4	end_date;//	结束日期	YYMMDD, 0为不考虑
	N4 	end_time;//结束时间	HHMMSSmmm，0为忽略
	std::vector <std::string> code_table;//证券代码表C…	品种代码表，分号隔开，如000001.SH; 600001.SZ	
};
struct SYSTEMDATA{
	un_short Type;//系统信息类型 12 闭市消息
	char info[70];//具体信息  用“|”分割数据
};

#endif

