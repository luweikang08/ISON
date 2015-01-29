#ifndef ISON_SDS_STORE__DATA_API_H_
#define ISON_SDS_STORE__DATA_API_H_
#include <map>
#include<vector>
#include <string>
#define MemData map<std::string,std::vector<TimeIndexMessage>> 
struct TgwReqHead{
	int AppType;
	int PackType;
};
struct TimeIndexMessage{//����ʱ����������Ӧ����
	int time;
	std::string message;
};
struct HQRequest{
	N4  seqno;     //	��Ϣ��ſͻ�ָ����(�����ظ�)
	N2	date_type;//	���ĵ���������
	N4 	begin_date;//	��ʼ���� YYMMDD�� 0Ϊ����
	N4	begin_time;	//��ʼʱ��	HHMMSSmmm��0Ϊ��ʱ
	N4	end_date;//	��������	YYMMDD, 0Ϊ������
	N4 	end_time;//����ʱ��	HHMMSSmmm��0Ϊ����
	std::vector <std::string> code_table;//֤ȯ�����C��	Ʒ�ִ�����ֺŸ�������000001.SH; 600001.SZ	
};

#endif