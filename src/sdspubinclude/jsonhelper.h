#ifndef ISON_SDS_PUB_JSONHELPER_H_
#define ISON_SDS_PUB_JSONHELPER_H_
#include <iostream> 
#include<fstream>
#include"iblog.h"
#include<vector>
#include<string>
#include<map>
#include"ibsdstool.h"
#include "document.h"
#include "isonsdsdataapi.h"
int ReadConfig(const char* filename, std::vector <std::string> configkey,std::map<std::string,std::string> &configmap);
int Parase2Object(std::string json_data, SDS_Level2 &sds_level2);
int Parase2Object(std::string json_data, SDS_Transaction &sds_transaction);
int Parase2Object(std::string json_data, SDS_Order &sds_order);
int Parase2Object(std::string json_data, SDS_OderQueen &sds_order_queen);
int Parase2Object(std::string json_data, SDS_Code &sds_code);
int QueryReply(std::vector<SDS_Order> &object, const char * code, std::string result);
int Parase2Object(std::string json_data, SDS_CloseMarket &sds_close_market);
int Sring2Dac(SDS_Level2 &sds_level2, string  &out2dac);
int Sring2Dac(SDS_Transaction &sds_trans, string  &out2dac);
int Sring2Dac(SDS_OderQueen &sds_orderqueue, string  &out2dac);

#endif