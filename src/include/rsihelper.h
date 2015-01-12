#ifndef __ISON_SDS_RSI_HELPER__
#define __ISON_SDS_RSI_HELPER__

#include <iostream>
#include <queue>

#include "ta_common.h"
#include "ta_func.h"

#include "document.h"
#include "isonsdsdataapi.h"
#include "jsonhelper.h"
#include "data_struct.h"

int Queue2Array(std::queue<double>& src, double dest[], int offset, int length);
int CalcRsiResult(std::queue<double>& src, int& MACD, int& MACDSignal, int& MACDHist);
int RsiData2String(RsiData src, std::string& dest);

#endif