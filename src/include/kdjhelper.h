#ifndef __ISON_SDS_RSI_HELPER__
#define __ISON_SDS_RSI_HELPER__

#include <iostream>
#include <queue>

#include "jsonhelper.h"
#include "data_struct.h"
#include "isonsdsdataapi.h"

int Queue2Array(std::queue<double>& src, double dest[], int offset, int length);

#endif