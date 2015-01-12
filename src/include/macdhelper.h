#ifndef __ISON_SDS_MACD_HELPER__
#define __ISON_SDS_MACD_HELPER__

#include <iostream>
#include <queue>

#include "ta_common.h"
#include "ta_func.h"

#include "document.h"
#include "isonsdsdataapi.h"
#include "jsonhelper.h"
#include "data_struct.h"

int Queue2Array(std::queue<double>& src, double dest[], int offset, int length);
int CalcMacdResult(std::queue<double>& src, int& MACD, int& MACDSignal, int& MACDHist);
int MacdData2String(MacdData src, std::string& dest);

#endif
