#ifndef SDS_ISON_IBS_DATA_DEF_H_
#define SDS_ISON_IBS_DATA_DEF_H_

#define	IBSSELLINTIME     2001
#define	IBSLEVEL2        1009
#include<string>
struct IBSSellintime{
	std::string    SCR_CODE;	 // ֤ȯ�ֶ���	String
	int 	TM;  // �ɽ�ʱ��	INT32
	int	    TRDD_PRC;	//  �ɽ��۸�	INT32
	long long BGN_AMT; //�ɽ����	INT64
};
#endif