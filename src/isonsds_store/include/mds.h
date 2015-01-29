#ifndef  ISON_BASE_SDS_MDS_H_
#define   ISON_BASE_SDS_MDS_H_
class Mds
{
public:
	Mds();
	~Mds();
	//�洢ĳ��ʱ��������
	int Put(const char* market_id, const char* stock_id, int data_type, int date, int time, void* data, size_t len);
	//��ȡĳ��ʱ��������
	int Get(const char* market_id, const char* stock_id, int data_type, int date, int time, void* data, size_t len);
	//��ȡĳ��ʱ��ε�����
	int Get(const char* market_id, const char* stock_id, int data_type, int begin_date, int begin_time, int end_date, int end_time, void* data, size_t len);
	//��ȡĳ��ʱ����Ժ�ļ�������
	int GetAfter(const char* market_id, const char* stock_id, int data_type, int date, int time, int* count, void* data, size_t len);
	//��ȡ����ļ�������
	int GetLastest(const char* market_id, const char* stock_id, int data_type, int date, int time, int* count, void* data, size_t len);
};

#endif