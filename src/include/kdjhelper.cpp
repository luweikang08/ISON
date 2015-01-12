#include "kdjhelper.h"

/*
*transform queue to array,used for calculate MACD
*src : source queue
*dest : destination array
*offset : start index at queue
*length ; transform length in queue,start from offset
*/
int Queue2Array(std::queue<double>& src, double dest[], int offset, int length)
{
	double temp = 0;
	if (src.empty())
		return -1;
	for (int i = 0; i < src.size(); i++)
	{
		temp = src.front();
		src.pop();
		if ((i >= offset) && (i < (length + offset)))
		{
			dest[i - offset] = temp;
			//printf("array %d:%d\n", i - offset, dest[i - offset]);
		}
		src.push(temp);
	}
	return 0;
}

/*
*calculate KDJ from Kline Data
*startIdx : start index
*endIdx : end index
*src : source data
*optInTimePeriod : Number of period for the RSV
--RSV=100*(Ct-Ln)/(Hn-Ln);Ct is last ClosePrice,Hn is highest price in optInTimePeriod days,Ln is lowest price.
*outBegIdx : 
*outNBElement : number of outElement
*outRealOfK : K result array
*outRealOfD : D result array
*outRealOfJ : J result array
*/
int KDJ(int startIdx, int endIdx,KLineData src[], int optInTimePeriod, int*outBegIdx, int*outNBElement, double outRealOfK[], double outRealOfD[], double outRealOfJ[])
{
	int tempHigh, tempLow, tempLast, tempIdx, outIdx;
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
		tempHigh = src[i].High;
		tempLow = src[i].Low;
		tempLast = src[i].Close;
		for (int j = 1; j < optInTimePeriod; j++)
		{
			tempHigh = tempHigh > src[i + j].High ? tempHigh : src[i + j].High;
			tempLow = tempLow < src[i + j].Low ? tempLow : src[i + j].Low;
			tempLast = src[i + j].Close;
		}
		outRealOfK[tempIdx++] = 100 * (tempLast - tempLow) / (tempHigh - tempLow); //restore RSV
	}
	outIdx = 0;
	outRealOfK[0] = (outRealOfK[0] + 100) / 3;                //K=prevK*2/3+RSV*1/3,if there is no prevK,prevK=50
	outRealOfD[0] = (outRealOfK[0] + 100) / 3;                //D=prevD*2/3+K*1/3,if there is no prevD,prevD=50
	outRealOfJ[0] = 3 * outRealOfD[0] - 2 * outRealOfK[0];    //J=3D-2K
	outIdx++;
	for (; outIdx < tempIdx; outIdx++)
	{
		outRealOfK[outIdx] = (outRealOfK[outIdx - 1] * 2 + outRealOfK[outIdx]) / 3;    //K=prevK*2/3+RSV*1/3
		outRealOfD[outIdx] = (outRealOfD[outIdx - 1] * 2 + outRealOfK[outIdx]) / 3;    //D=prevD*2/3+K*1/3
		outRealOfJ[outIdx] = 3 * outRealOfD[outIdx] - 2 * outRealOfK[outIdx];          //J=3D-2K
	}
	*outBegIdx = startIdx;
	*outNBElement = outIdx - 1;
	return 0;
}