#include "kcpolydb.h"
using namespace kyotocabinet;
extern PolyDB g_memorydb;
int OpenKCAndInsertKeyValue(PolyDB &db, const char * table, const char * key, const char* & value);
int GetKCAllKeyValue(PolyDB &db, const char * table);
int GetKCDataByRegex(PolyDB &db, const char * table, const std::string &  	regex,
	std::vector< std::string > *  value);
int ReadFileKeyValue2KC(PolyDB &db, const char * table, const char* fliename);
int ReadFileKeyValue2DB(PolyDB &db, const char * table, const char* fliename);