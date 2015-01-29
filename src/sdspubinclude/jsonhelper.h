#ifndef ISON_SDS_PUB_JSONHELPER_H_
#define ISON_SDS_PUB_JSONHELPER_H_
#include <iostream> 
#include<fstream>
#include"iblog.h"
#include<vector>
#include<string>
#include<map>
#include "ibsdstool.h"
#include "document.h"
#include "isonsdsdataapi.h"
#include "sbeinterface.h"
enum JSON_ERROR{
	ERR_SUCESS = 1600,
	ERR_FILE_NO_EXIT,
	ERR_PARASE,
	ERR_NO_KEY,
	ERR_VALUETPYE_ERROR
};
int ReadConfig(const char* filename, std::vector <std::string> configkey,std::map<std::string,std::string> &configmap);
int Parase2Object(std::string json_data, baseline::SDS_Level2 * sds_level2, char * buffer, int &bufferLength);
int Parase2Object(std::string json_data, baseline::SDS_Transaction *sds_transaction, char * buffer, int &bufferLength);
int Parase2Object(std::string json_data, SDS_Order &sds_order);
int Parase2Object(std::string json_data, baseline::SDS_OderQueen *sds_order_queen,char * buffer, int &bufferLength);
int QueryReply(std::vector<SDS_Order> &object, const char * code, std::string result);
int Parase2Object(std::string json_data, SDS_CloseMarket &sds_close_market);
int Sring2Dac(baseline::SDS_Level2 &sds_level2, string  &out2dac);
int Sring2Dac(baseline::SDS_Transaction &sds_trans, string  &out2dac);
int Sring2Dac(baseline::SDS_OderQueen &sds_orderqueue, string  &out2dac);
void SBE2Json(const std::string &sbe_message, baseline::SDS_Level2 &sds_level2, std::string &json_message);
void SBE2Json(const std::string &sbe_message, baseline::SDS_Transaction &sds_trans, std::string &json_message);
void SBE2Json(const std::string &sbe_message, baseline::SDS_OderQueen &sds_queue, std::string &json_message);
#endif