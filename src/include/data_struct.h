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

enum TimeStatus  //ö�ټ���K�ߵ�ʱ�䳤��
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
	Cc Code[16];        //֤ȯ����
	N4 Date;            //����
	N4 Time;            //ʱ��
	TS TimeStatus;      //����K�ߵ�ʱ�䳤��
	N4 PreClose;        //ǰ���̼�
	N4 Open;            //���̼�
	N4 Close;           //���̼�
	N4 High;            //��߼�
	N4 Low;             //��ͼ�
	N8 Volume;          //�ɽ�����
	N8 Turnover;        //�ɽ��ܶ�
};

#endif