#ifndef __ISON_KDJ_FUNC_HELPER__
#define __ISON_KDJ_FUNC_HELPER__

#include <iostream>
#include <queue>

#include "jsonhelper.h"
#include "data_struct.h"
#include "isonsdsdataapi.h"

int KDJ(int startIdx, int endIdx, const double src[][3], int optInTimePeriod, int optInKPeriod, int optInDPeriod, int*outBegIdx, int*outNBElement, double outRealOfK[], double outRealOfD[], double outRealOfJ[]);

#endif