#ifndef ISON_SDS2ATRADE_PERFORMANCE_H_
#define ISON_SDS2ATRADE_PERFORMANCE_H_
#include <string>
#include "timehelper.h"
#include"ibsdstool.h"
#include "document.h"
#include "isonsdsdataapi.h"
struct InOutTime{
	int farther_time;
	int in_time;
	int out_time;
};
struct Performance{
	long long  min;
	long long  max;
	long long sum;
	long long  size;
	void reset(){
		min = 99999999999;
		max = 0;
		sum = 0;
		size = 0;
	};
};
void SumTimeDelay(Performance &threetime, long long &delaytime,string &message){
	if (delaytime < 0)
	{
		LOG(INFO) << "sub-ture time <0";//小于0不计算在延迟范围内
	}
	else{
		threetime.size++;
		if (delaytime < threetime.min)
			threetime.min = delaytime;
		if (delaytime > threetime.max)
		{
			threetime.max = delaytime;
			LOG(INFO) <<message.c_str();
		}
		threetime.sum += delaytime;
	};
}
#endif