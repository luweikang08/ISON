 
#ifdef _WIN32
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include<string>
#define sprintf sprintf_s
#define LOCAL_TIME(t, ts) localtime_s(ts, t)
#else
#include <time.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#define LOCAL_TIME(t, ts) localtime_r(t, ts)

#endif
#include "timehelper.h"
DateAndTime  GetDateAndTime()
{
	DateAndTime  date_time = { 0 };
    char cdate[9];
	char ctime[10];
	struct timeb t_now;
	struct tm when;
	ftime(&t_now);
	LOCAL_TIME(&t_now.time,&when);
	memset(cdate, 0, sizeof(cdate));
	memset(ctime, 0, sizeof(ctime));
	sprintf(cdate, "%04d%02d%02d", when.tm_year+1900, when.tm_mon+1, when.tm_mday);//YYYYMMDD
	sprintf(ctime, "%02d%02d%02d%03d", when.tm_hour, when.tm_min, when.tm_sec,t_now.millitm);//HHMMSSmmm
	date_time.date = atoi(cdate);
	date_time.time = atoi(ctime);
	return date_time;
};

void  DateTime2Second( int &now_time,unsigned int &now_second)
{
	time_t now = time(NULL);//1970�굽���ڵ�s��
	struct tm when;
	LOCAL_TIME(&now, &when);//��ȡ��ǰϵͳʱ�� HHMMSSMMM
	now_second = now - ((when.tm_hour - now_time / 10000000) * 3600 + 
		((when.tm_min - (now_time % 10000000) / 100000) * 60) +
		(when.tm_sec - ((now_time % 10000000) % 100000) / 1000));
}

void  DateTime2Second(int &date,int &now_time, unsigned int &now_second)
{ 
	time_t timep;
	struct tm p;
	int YYYY = date / 10000 - 1900;
	int MM = (date %10000)/100-1;
	int DD=date%100;
	int hh = now_time / 10000000+8;
	int mm = (now_time % 10000000)/100000;
	int ss = ((now_time % 10000000) % 100000)/1000;
	p.tm_year = YYYY;
	p.tm_mon = MM;
	p.tm_mday = DD;
	p.tm_hour =hh ;
	p.tm_min = mm;
	p.tm_sec = ss;
	timep = mktime(&p);
	now_second = (unsigned int)timep;
}




void SnInit(unsigned &sn)
{
	struct tm when;
	time_t curtime = time(NULL);
	LOCAL_TIME(&(curtime), &when);
	sn = when.tm_hour * 3600 + when.tm_min * 60 + when.tm_sec;// ��֤ÿ������������sn�������ǰ���ظ������ǿ��ܲ�������
	// �����ǰһ����ҹ�����ڵ�����,Ȼ���20000��Ϊ��ʼ�ĺ���.
	sn *= 20000;
}
long long TransTime2ms(int time)
{
	//ȡ��HHMM
	int hhmm = time / 100000;
	return (((hhmm / 100) * 60 + hhmm % 100) * 60000 + time % 100000);

}


