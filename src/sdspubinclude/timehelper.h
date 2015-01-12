#ifndef  ISON_SDS_STORE_TIMEHELPER_H_
#define   ISON_SDS_STORE_TIMEHELPER_H_
struct DateAndTime{
	int date;//YYYYMMDD
	int time;//HHMMSS
};

DateAndTime  GetDateAndTime();
void  DateTime2Second(int &now_time,unsigned int &now_second);
void  DateTime2Second(int &date, int &now_time, unsigned int &now_second);
void SnInit(unsigned &sn);
long long TransTime2ms(int time);
#endif
