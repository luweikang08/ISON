#ifndef  ISON_BASE_SDS_MDS_H_
#define   ISON_BASE_SDS_MDS_H_
class Mds
{
public:
	Mds();
	~Mds();
	//存储某个时间点的数据
	int Put(const char* market_id, const char* stock_id, int data_type, int date, int time, void* data, size_t len);
	//获取某个时间点的数据
	int Get(const char* market_id, const char* stock_id, int data_type, int date, int time, void* data, size_t len);
	//获取某个时间段的数据
	int Get(const char* market_id, const char* stock_id, int data_type, int begin_date, int begin_time, int end_date, int end_time, void* data, size_t len);
	//获取某个时间点以后的几条数据
	int GetAfter(const char* market_id, const char* stock_id, int data_type, int date, int time, int* count, void* data, size_t len);
	//获取最近的几条数据
	int GetLastest(const char* market_id, const char* stock_id, int data_type, int date, int time, int* count, void* data, size_t len);
};

#endif