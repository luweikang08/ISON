#ifndef __ISON_SDS_DATA_STRUCT_H__
#define __ISON_SDS_DATA_STRUCT_H__

//#define N8 long long
//#define N4 int
//#define N2 short
//#define Cc char
//#define TS TimeStatus

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
#define strcpy_s strcpy
#define _stricmp strcasecmp
#endif

#define LEVLEL2LENGTH 10

enum TS  //ö�ټ���K�ߵ�ʱ�䳤��
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
	C  Code[16];        //֤ȯ����
	N4 Date;            //����
	N4 Time;            //ʱ��
	TS TimeStatus;//����K�ߵ�ʱ�䳤��
	N4 PreClose;        //ǰ���̼�
	N4 Open;            //���̼�
	N4 Close;           //���̼�
	N4 High;            //��߼�
	N4 Low;             //��ͼ�
	N8 Volume;          //�ɽ�����
	N8 Turnover;        //�ɽ��ܶ�
};

struct MacdData
{
	C  Code[16];        //֤ȯ����
	N4 Date;            //����
	N4 Time;            //ʱ��
	N4 MACD;            //MACD(DIF)=EMA12-EMA26
	N4 MACDSignal;      //MACDSignal(DEA)=EMA(MACD,9),MACD��9���ƶ�ƽ����
	N4 MACDHist;        //MACDHist(MACD)=MACD-MACDSignal
};

struct RsiData
{
	C  Code[16];        //֤ȯ����
	N4 Date;            //����
	N4 Time;            //ʱ��
	N4 Rsi6;            //Rsi
	N4 Rsi12;           //Rsi12
	N4 Rsi24;           //Rsi24
};

struct TranData         //��ʳɽ�
{
	N4 Seqno;
	C  Code[16];        //֤ȯ����
	N4 Date;            //����
	N4 Time;            //ʱ��
	N4 SDSTime;
	N4 Sn;
	N4 Index;           //�ɽ����
	N4 Price;           //�ɽ��۸�
	N4 Volum;           //�ɽ�����
	N8 Turnover;        //�ɽ����
	C  BSFlag;          //��������
	C  OrderKind;
	N4 FunctionCode;
	N4 AskOrder;
	N4 BidOrder;
};

struct KdjData         //���ָ��
{
	C  Code[16];        //֤ȯ����
	N4 Date;            //����
	N4 Time;            //ʱ��
	N4 KValue;
	N4 DValue;
	N4 JValue;
};

#endif
