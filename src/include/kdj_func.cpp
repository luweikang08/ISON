#include "kdj_func.h"

/*
*calculate KDJ from Kline Data
*startIdx : start index
*endIdx : end index
*src : source data array, [idx][0] is high price,[idx][0] is low price,[idx][0] is last price,
*optInTimePeriod : Number of period for the RSV
--RSV=100*(Ct-Ln)/(Hn-Ln);Ct is last ClosePrice,Hn is highest price in optInTimePeriod days,Ln is lowest price.
*outBegIdx : 
*outNBElement : number of outElement
*outK : K result array
*outD : D result array
*outJ : J result array
*/
int KDJ(int startIdx, int endIdx, const double inReal[][3], int optInTimePeriod, int optInKPeriod, int optInDPeriod, int*outBegIdx, int*outNBElement, double outK[], double outD[], double outJ[])
{
	double tempHigh, tempLow, tempLast;
	int tempIdx, outIdx;
	if (startIdx < 0)
	{
		return -1;
	}
	if ((endIdx < 0) || (endIdx < startIdx))
	{
		return -1;
	}
	if ((optInTimePeriod <= 1) || (optInTimePeriod>10000))
	{
		return -1;
	}
	tempIdx = 0;
	for (int i = startIdx; i <= endIdx - 8; i++)
	{
		tempHigh = inReal[i][0];
		tempLow = inReal[i][1];
		tempLast = inReal[i][2];
		for (int j = 1; j < optInTimePeriod; j++)
		{
			tempHigh = tempHigh > inReal[i + j][0] ? tempHigh : inReal[i + j][0];
			tempLow = tempLow < inReal[i + j][1] ? tempLow : inReal[i + j][1];
			tempLast = inReal[i + j][2];
		}
		outK[tempIdx++] = 100 * (tempLast - tempLow) / (tempHigh - tempLow); //restore RSV
	}
	outIdx = 0;
	outK[0] = (outK[0] + 50 * (optInKPeriod - 1)) / optInKPeriod; //K=prevK*2/3+RSV*1/3,if there is no prevK,prevK=50,
	outD[0] = (outK[0] + 50 * (optInDPeriod - 1)) / optInDPeriod; //D=prevD*2/3+K*1/3,if there is no prevD,prevD=50,  optInDPeriod/optInDPeriod default value is 3
	outJ[0] = 3 * outD[0] - 2 * outK[0];    //J=3D-2K
	outIdx++;
	for (; outIdx < tempIdx; outIdx++)
	{
		outK[outIdx] = (outK[outIdx - 1] * (optInKPeriod - 1) + outK[outIdx]) / optInKPeriod;    //K=prevK*2/3+RSV*1/3
		outD[outIdx] = (outD[outIdx - 1] * (optInDPeriod - 1) + outK[outIdx]) / optInDPeriod;    //D=prevD*2/3+K*1/3
		outJ[outIdx] = 3 * outD[outIdx] - 2 * outK[outIdx];          //J=3D-2K
	}
	*outBegIdx = startIdx + optInTimePeriod - 1;
	*outNBElement = outIdx;
	return 0;
}