#ifndef  ISON_BASE_SDS_TOOL_H_
#define   ISON_BASE_SDS_TOOL_H_
#include <string>
#include<stdio.h>
#include <sys/timeb.h>
#include <sstream>
//#include <iconv.h>
#include "document.h"
#include "prettywriter.h"
#include "filestream.h"
#include "stringbuffer.h"
#include "writer.h"

using namespace std;

inline string Document2String(rapidjson::Document &document){
rapidjson::StringBuffer  buffer;
rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
document.Accept(writer);
string reststring = buffer.GetString();
return reststring;
}

inline string concatenate(std::string const& name, int i)//字符串和整数的拼接函数 如DATA和1拼接成DATA1
{
	stringstream s;
	s << name << i;
	return s.str();
}
inline string itostring( int i)//
{
	stringstream s;
	s <<i;
	return s.str();
}
inline string itostring(long long i)//
{
	stringstream s;
	s << i;
	return s.str();
}

/*
inline char *   str2charp(std::string  str)//string转化成char *
{

	char* c;
	unsigned int len = str.length();
	c = new char[len + 1];
	strncpy(c, str.c_str(), len + 1);//考虑跨平台
	return c;

}*/

inline int iconv_code(const char* tocode, const char* fromcode, const string& in, string* out)
{
	//size_t in_bytes = in.length();
	//size_t out_bytes = in.length() * 4;
	//out->assign(out_bytes, 0);
	//iconv_t cd = iconv_open(tocode, fromcode);
	//if (cd == (iconv_t)-1) return -1;
	//char* in_buf = const_cast<char*>(in.data());
	//char* out_buf = const_cast<char*>(out->data()); //trick
	//if (iconv(cd, &in_buf, &in_bytes, &out_buf, &out_bytes) == (size_t)-1) {
	//	iconv_close(cd);
	//	return -1;
	//}
	//out->resize(out_buf - out->data());
	//iconv_close(cd);
	return 0;
}

//inline void AddMember64(rapidjson::Value &body, rapidjson::Document::AllocatorType& allocator, const char * key, long long vlue){
	//rapidjson::Value temp;
	//const char*vuletemp = itostring(vlue).c_str();
	//temp.SetString(vuletemp, strlen(vuletemp), allocator);
	////LOG_IF(INFO, g_nlog_level>5) << key << ":" << vuletemp << "\n";
	//body.AddMember(key, temp, allocator);
//};
inline void AddMember64(rapidjson::Value &body, rapidjson::Document::AllocatorType& allocator,
	const char * key, long long vlue){
	//rapidjson::Value temp;
	//stringstream s;
	//s << vlue;
	//std::string stemp = s.str();
	//const char*vuletemp = stemp.c_str();
	//temp.SetString(vuletemp, strlen(vuletemp), allocator);
	////LOG_IF(INFO, g_nlog_level>5) << key << ":" << vuletemp << "\n";
	//body.AddMember(key, temp, allocator);
};



/*
inline char *  ccp2charp(const char* cpc)//const char*转化成char *
{
	char* pc = new char[128];//足够长
	strcpy(pc, cpc);
	return pc;
}*/
/*
inline string  ReadJsonFile(std::string file)
{
ifstream is;
is.open(file);
std::string str((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());//忽略空格把json文件转化成string类型
is.close();
return str;

}*/
//Get the  Current time 
/*
inline void gettime_(char *currentime)
{
	time_t now;
	struct tm 	*yy;
	struct timeb bnow;
	ftime(&bnow);
	time(&now);
	yy = localtime(&now);
	sprintf(currentime, "%04d.%02d.%02d %02d:%02d:%02d %03d'%03d", yy->tm_year + 1900, yy->tm_mon + 1, yy->tm_mday
		, yy->tm_hour, yy->tm_min, yy->tm_sec, bnow.time % 1000, bnow.millitm % 1000);

	return;
}
//Write log file
inline void  writelog(char * sender, char * logtxt)
{
	char idstr[40];
	char tmstr[40];
	// 记载文件日志 //
	printf("--testprint-[%s]%s\n", sender, logtxt);
	// 需要记载日志
	FILE * fp;
	char tmpfilename[512];
	char logbuf[10240];
	int  tt; // MMDDHHMM
	struct tm when;
	time_t now;
	time(&now);
	when = *localtime(&now);
	tt = ((int)when.tm_mon + 1) * 1000000 + ((int)when.tm_mday) * 10000 + ((int)when.tm_hour) * 100 + ((int)when.tm_min);
	memset(tmpfilename, 0, sizeof(tmpfilename));
	// modi by liaoj ,2007.08.28 when.tm_mon 居然是从0开始，0表示1月，汗.
	sprintf(tmpfilename, "%s%04d%02d%02d.txt", "d:\\mylog", when.tm_year + 1900, when.tm_mon + 1, when.tm_mday);
	fp = fopen(tmpfilename, "at");
	memset(tmstr, 0, sizeof(tmstr));
	gettime_(tmstr);
	sprintf(logbuf, "***LOG:[TM:%s][ID:%s]%s\n"
		, tmstr, sender, logtxt);
	if (fp != NULL){
		fputs(logbuf, fp);
		fclose(fp);
	}
	else{
		return;
	}
	return;
}
*/




#endif