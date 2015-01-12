#ifndef __ISON_SDS_JSON_ANALYSIS_H__
#define __ISON_SDS_JSON_ANALYSIS_H__

#include <iostream> 
#include <string>
#include <vector>
#include "rapidjson/document.h"
#include "isonsdsdataapi.h"
#include "data_struct.h"

int ParseJsonToMaketDataLevel2(std::string Signal, SDS_Level2& dest);

#endif