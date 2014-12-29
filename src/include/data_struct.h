#ifndef __ISON_SDS_DATA_STRUCT_H__
#define __ISON_SDS_DATA_STRUCT_H__

//#define N8 long long
//#define N4 int
//#define N2 short
//#define Cc char
#define TS TimeStatus

#ifdef _WIN32
#ifndef __int64
#define __int64 long long
#endif
#define N8 __int64
#define N4 __int32
#define N2 __int16
#define Cc char
#else 
#define N8 long long
#define N4 int
#define N2 short
#define Cc char
#endif

#define LEVLEL2LENGTH 10

enum TimeStatus  //枚举计算K线的时间长度
{
	OneMinute = 1,
	FiveMinute = 5,
	TenMinute = 10,
	FifteenMinute = 15,
	ThirtyMinute = 30,
	OneHour = 60,
	OneDay = 61,
	OneWeek = 62,
	OneMonth = 63,
	OneSeason = 64,
	OneYear = 65
};

struct KLineData
{
	Cc Code[16];        //证券代码
	N4 Date;            //日期
	N4 Time;            //时间
	TS TimeStatus;      //计算K线的时间长度
	N4 PreClose;        //前收盘价
	N4 Open;            //开盘价
	N4 Close;           //收盘价
	N4 High;            //最高价
	N4 Low;             //最低价
	N8 Volume;          //成交总量
	N8 Turnover;        //成交总额
};

#endif