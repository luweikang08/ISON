#ifndef SDS_ISON_IBS_DATA_DEF_H_
#define SDS_ISON_IBS_DATA_DEF_H_

#define	IBSSELLINTIME     2001
#define	IBSLEVEL2        1009
#include<string>
struct IBSSellintime{
	std::string    SCR_CODE;	 // 证券字段名	String
	int 	TM;  // 成交时间	INT32
	int	    TRDD_PRC;	//  成交价格	INT32
	long long BGN_AMT; //成交金额	INT64
};
#endif