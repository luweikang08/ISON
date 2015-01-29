#ifndef ISON_SDS_STORE__DATA_API_H_
#define ISON_SDS_STORE__DATA_API_H_
#include <map>
#include<vector>
#include <string>
#define MemData map<std::string,std::vector<TimeIndexMessage>> 
struct TgwReqHead{
	int AppType;
	int PackType;
};
struct TimeIndexMessage{//按照时间来建立对应索引
	int time;
	std::string message;
};
struct HQRequest{
	N4  seqno;     //	消息编号客户指令编号(不能重复)
	N2	date_type;//	订阅的数据类型
	N4 	begin_date;//	开始日期 YYMMDD， 0为当天
	N4	begin_time;	//开始时间	HHMMSSmmm，0为当时
	N4	end_date;//	结束日期	YYMMDD, 0为不考虑
	N4 	end_time;//结束时间	HHMMSSmmm，0为忽略
	std::vector <std::string> code_table;//证券代码表C…	品种代码表，分号隔开，如000001.SH; 600001.SZ	
};

#endif