#ifndef  ISON_SDS_INTERFACE_TRANS_H_
#define    ISON_SDS_INTERFACE_TRANS_H_

#include <string>
#include <map>
#include "document.h"
#include "prettywriter.h"
#include "filestream.h"
#include "stringbuffer.h"
#include "writer.h"

class InterfaceTrans
{
private:
	static int json2map(const char *paramsin[], const char* paramsto[], rapidjson::Value& body, std::map < std::string, std::string > &outmap, unsigned intparamslen);//1对1的关系
	static int json2map(const char *paramsin[], const char* paramsto[], rapidjson::Value& body, std::map < std::string, std::string > &outmap, unsigned int paramslen, unsigned int  len);//1对多的关系
	static int map2json(int head, const char* paramsout[], std::map < std::string, std::string > &tempmap, std::string *out, unsigned int paramslen);

public:
	static int SZKLevel2ibs(rapidjson::Document& doc, std::string* out);
	static int SZKSellintime2ibs(rapidjson::Document& doc, std::string* out);
	static int level2_countibs(rapidjson::Document& doc, std::string* out);
	static int transaction_count2ibs(rapidjson::Document& doc, std::string* out);


};
#endif

