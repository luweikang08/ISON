#include <fstream>
#include "isonutil.h"
#include "memoryhelper.h"

//int OpenKCAndInsertKeyValue(std::string &table,const char * key,const char* & value){

int OpenKCAndInsertKeyValue(PolyDB &db ,const char * table, const char * key, const char* & value){
	if (!db.open(table, PolyDB::OWRITER | PolyDB::OCREATE))
	{ 	
		std::cerr << "open error: " << db.error().name() << std::endl;
	return -1;
	}
 
	if (!db.set(key, value))
{  
		std::cerr << "set error: " << db.error().name() << std::endl;
	return -1;
}

	if (!db.close()) {// close the database
		std::cerr << "close error: " << db.error().name() << std::endl;
}
return 0;
}



int GetKCAllKeyValue(PolyDB &db, const char * table){
	if (!db.open(table, PolyDB::OWRITER | PolyDB::OCREATE))
	{
		std::cerr << "open error: " << db.error().name() << std::endl;
		return -1;
	}
	DB::Cursor* cur = db.cursor();
	cur->jump();
	std::string ckey, cvalue;
	while (cur->get(&ckey, &cvalue, true)) {
		std::cout << ckey << ":" << cvalue << std::endl;
	}
	
	if (!db.close()) {// close the database
		std::cerr << "close error: " << db.error().name() << std::endl;
	}
}
int GetKCDataByRegex(PolyDB &db, const char * table, const std::string &  	regex,
	std::vector< std::string > *  value){
	if (!db.open(table, PolyDB::OWRITER | PolyDB::OCREATE))
	{
		std::cerr << "open error: " << db.error().name() << std::endl;
		return -1;
	};
	std::vector< std::string > *  strvec;//存储匹配的key
	db.match_regex(regex, strvec, -1, NULL);
	for (auto it = (*strvec).begin(); it != (*strvec).end(); it++)
	{
		string valuetemp;
		if (db.get((*it), &valuetemp)) {
			value->push_back(valuetemp);
		}
		else {
			std::cerr << "get error: " << db.error().name() << std::endl;
		}
	}

	if (!db.close()) {// close the database
		std::cerr << "close error: " << db.error().name() << std::endl;
	}

}
int ReadFileKeyValue2KC(PolyDB &db, const char * table, const char* fliename){
	if (!db.open(table, PolyDB::OWRITER | PolyDB::OCREATE))
	{
		std::cerr << "open error: " << db.error().name() << std::endl;
		return -1;
	}
	//const char * key, const char* & value;
	//读文件  一行一行读
	std::ifstream infile(fliename);//打开1.txt     
	std::string temp;
	if (!infile)
		std::cout << "文件打开失败！" << std::endl;   //不能打开文件，报错   
	else
	{
		//
		while (getline(infile, temp))
		{
			std::vector<std::string> params;
			split(temp, " ", &params);
			if (!db.set(params[0], params[1]))
			{
				std::cerr << "set error: " << db.error().name() << std::endl;
				return -1;
			}
		}
		

		if (!db.close()) {// close the database
			std::cerr << "close error: " << db.error().name() << std::endl;
		}
		return 0;
	}
}
int ReadFileKeyValue2DB(PolyDB &db, const char * table, const char* fliename){
	if (!db.open(table, PolyDB::OWRITER | PolyDB::OCREATE))
	{
		std::cerr << "open error: " << db.error().name() << std::endl;
		return -1;
	}
	//const char * key, const char* & value;
	//读文件  一行一行读
	std::ifstream infile(fliename);//打开1.txt     
	std::string temp;
	if (!infile)
		std::cout << "文件打开失败！" << std::endl;   //不能打开文件，报错   
	else
	{
		//
		while (getline(infile, temp))
		{
			std::vector<std::string> params;
			split(temp, " ", &params);
			if (!db.set(params[0], params[1]))//params是一种jason格式的字符串
			{
				std::cerr << "set error: " << db.error().name() << std::endl;
				return -1;
			}
			//解析成相应的
		}
		if (!db.close()) {// close the database
			std::cerr << "close error: " << db.error().name() << std::endl;
		}
		return 0;
	}
}


