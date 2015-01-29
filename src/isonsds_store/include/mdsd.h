#ifndef  ISON_BASE_SDS_MSDD_MSDI_H_
#define   ISON_BASE_SDS_MSDD_MSDI_H_
#define ushort unsigned short
#define uint unsigned int
enum Datatype
{
	MSD_Level2 = 83,//Level2//行情
	MSD_Transaction=84,//逐笔成交
	MSD_Order=85,//逐笔委托
	MSD_OrderQueue=86,//委托队列

};
struct MdsHeader
{
	ushort Version;//版本号	2
	ushort Reserved1;	//保留	2
	uint  Crc_all;	//校验值(含头部)	4
	ushort Data_type;//	数据类型	2
	ushort Item_size;//	数据块长度	2
	uint	 Item_count;//	数据块个数	4
	uint	 Crc_data;// 数据校验值	4
	uint	 Crc_item_count;// 已校验数据长度	4
	uint  Start_date;// 开始日期(YYYYMMDD)	4
	uint  Start_time;// 开始时间(HHMMSSmmm)	4
	uint	 End_date;//	结束日期(YYYYMMDD)	4
	uint	 End_time;// 结束时间(HHMMSSmmm)	4
	char Market_id[4];//	市场标识	4
	char Stock_id[4];//	标的标识	4
	char Stock_code[80];// 标的代码	80
	char Reserved2[128];// 保留	128
};
struct Key
{   
	//uint  Key_date;// 开始日期(YYYYMMDD)	4
	//uint  Key_time;// 开始时间(HHMMSSmmm)	4
	char key[18];

};

class Mdsd
{
public:
	//Mdsd();
	//~Mdsd();
public:
	int Open(const char* path);
	int Init(MdsHeader&);
	const MdsHeader& GetHeader();

	int Append(void* data, size_t len);
	int Update(int idx, void* data, size_t len);

	int ItemSize();
	int Count();
	//读取数据
	int Get(int idx, void**buf, size_t* len);
	//连续读取多个数据
	int Get(int idx, int* count, void** buf, size_t* len);
private:
	const char *path_;
	MdsHeader mds_header_;
};
//索引文件
class Mdsi{
public:
	int Open(const char*);
	int Insert(Key&, int);//插入key与序号到索引文件
	int Lookup(Key&, int*);//查找key对应的序号
private:
	const char * path_;
};
#endif
