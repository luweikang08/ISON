#include <iostream>
#include <vector>
#include <sstream>

#include "ta_common.h"
#include "ta_func.h"
#include <time.h>

#include "ta_gDataHigh.c"
#include "ta_gDataLow.c"
#include "ta_gDataClose.c"

#include "kdj_func.h"

using namespace std;

const TA_Real OpenPrice[50] = {
	29.375,  29.3302, 29.3226, 29.3149, 29.3519, 
	29.3472, 29.3478, 29.3517, 29.3539, 29.3248,
	29.3118, 29.3271, 29.3431, 29.3446,	29.3787, 
	29.367,  29.37,   29.3837, 29.4329, 29.4226, 
	29.4434, 29.3472, 29.3914, 29.4046, 29.4207,
	29.4139, 29.4645, 29.4695, 29.5003, 29.503,
	29.5088, 29.5324, 29.5217, 29.5454,	29.5178, 
	29.4761, 29.4432, 29.4431, 29.4664, 29.5172,
	29.4896, 29.5327, 29.5635, 29.5439, 29.5506,
	29.5165, 29.5413, 29.5801, 29.6222, 29.6168 };
TA_Real    closePrice[360];
TA_Real    out[51];
TA_Integer outBeg;
TA_Integer outNbElement;
TA_Real    outMACD[51];
TA_Real    outMACDSignal[51];
TA_Real    outMACDHist[51];

TA_Integer g_num_MaStartIdx = 0;
TA_Integer g_num_MaEndIdx = 0;
TA_Integer g_num_MaOptInTimePeriod = 5;
TA_MAType  g_enum_OptInMAType = TA_MAType_SMA;
TA_Integer g_num_MaOutBegIdx;
TA_Integer g_num_MaOutNBElement;
TA_Real    g_num_MaOutReal[240];

int main(int argc, char* argv[])
{
	//test kline timer, time increase
	//int timeSrc=145957330,timeTag;
	//timeTag=((timeSrc / 100000) % 100 >= 59 ? (timeSrc / 10000000+1) * 10000000 + timeSrc % 100000 : (timeSrc / 10000000) * 10000000 + ((timeSrc / 100000) % 100 + 1) * 100000 + timeSrc % 100000);

	//test KDJ function,
	/*double inReal[50][3];
	for (int i = 0; i < 50; i++)
	{
		inReal[i][0] = gDataHigh[i];
		inReal[i][1] = gDataLow[i];
		inReal[i][2] = gDataClose[i];
	}
	int OutBeg;
	int OutNBElement;
	double OutK[50];
	double OutD[50];
	double OutJ[50];

	KDJ(0, 49, inReal, 9, 3, 3, &OutBeg, &OutNBElement, OutK, OutD, OutJ);*/

	stringstream ss;
	double i = 1.0;
	ss << i;
	string str = ss.str();
	cout << str.size();

	std::system("pause");

	TA_RSI(0, 49, OpenPrice, 6, &outBeg, &outNbElement, outMACD);
	TA_RSI(0, 49, OpenPrice, 12, &outBeg, &outNbElement, outMACD);
	TA_RSI(0, 49, OpenPrice, 24, &outBeg, &outNbElement, outMACD);

	//TA_EMA(0, 50, &OpenPrice[0], 26, &outBeg, &outNbElement, &out[0]);
	TA_MACD(0, 49, &OpenPrice[0], 12, 26, 9, &outBeg, &outNbElement, &outMACD[0], &outMACDSignal[0], &outMACDHist[0]);
	for (int i = 0; i < 51; i++)
	{
		//printf("%.10f\n", out[i]); //EMA
		printf("%.10f %.10f %.10f %d\n", outMACD[i], outMACDSignal[i], outMACDHist[i], outBeg); //MACD
	}
	return 0;
}