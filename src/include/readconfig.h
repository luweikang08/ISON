#ifndef __ISON_SDS_READ_CONFIG_H__
#define __ISON_SDS_READ_CONFIG_H__

#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#include "rapidjson.h"
#include "document.h"
#include "filestream.h"
#include "ibsdstool.h"

enum RF_RetCode //readfile return code
{
	RF_SUCCESS=0,
	RF_FILE_NOT_EXIST = 1,
	RF_PARSE_ERROR,
	RF_KEY_NOT_EXIST,
	RF_FAIL
};

int readjsonfile(const char* filename, std::vector <std::string> configkey, std::map<std::string, double> &configmap);
RF_RetCode readjsonfile(const char* filename, const char* masterkey, std::vector <std::string> configkey, std::map<std::string, std::string> &configmap);

#endif