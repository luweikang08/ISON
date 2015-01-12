#ifndef __ISON_SDS_KLINE_HELPER_H__
#define __ISON_SDS_KLINE_HELPER_H__

#include <iostream>
#include "document.h"
#include "isonsdsdataapi.h"
#include "data_struct.h"
#include "jsonhelper.h"

int KlineFromMaketData(int StartIdx, int EndIdx, SDS_Level2 src[], KLineData& dest);
int KlineFromKline(int StartIdx, int EndIdx, KLineData src[], KLineData& dest);
int KLineData2String(KLineData src, std::string& dest);
int Parase2Object(std::string json_data, KLineData &dest);

#endif