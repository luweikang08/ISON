#ifndef  ISON_BASE_SDS_MSDD_MSDI_H_
#define   ISON_BASE_SDS_MSDD_MSDI_H_
#define ushort unsigned short
#define uint unsigned int
enum Datatype
{
	MSD_Level2 = 83,//Level2//����
	MSD_Transaction=84,//��ʳɽ�
	MSD_Order=85,//���ί��
	MSD_OrderQueue=86,//ί�ж���

};
struct MdsHeader
{
	ushort Version;//�汾��	2
	ushort Reserved1;	//����	2
	uint  Crc_all;	//У��ֵ(��ͷ��)	4
	ushort Data_type;//	��������	2
	ushort Item_size;//	���ݿ鳤��	2
	uint	 Item_count;//	���ݿ����	4
	uint	 Crc_data;// ����У��ֵ	4
	uint	 Crc_item_count;// ��У�����ݳ���	4
	uint  Start_date;// ��ʼ����(YYYYMMDD)	4
	uint  Start_time;// ��ʼʱ��(HHMMSSmmm)	4
	uint	 End_date;//	��������(YYYYMMDD)	4
	uint	 End_time;// ����ʱ��(HHMMSSmmm)	4
	char Market_id[4];//	�г���ʶ	4
	char Stock_id[4];//	��ı�ʶ	4
	char Stock_code[80];// ��Ĵ���	80
	char Reserved2[128];// ����	128
};
struct Key
{   
	//uint  Key_date;// ��ʼ����(YYYYMMDD)	4
	//uint  Key_time;// ��ʼʱ��(HHMMSSmmm)	4
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
	//��ȡ����
	int Get(int idx, void**buf, size_t* len);
	//������ȡ�������
	int Get(int idx, int* count, void** buf, size_t* len);
private:
	const char *path_;
	MdsHeader mds_header_;
};
//�����ļ�
class Mdsi{
public:
	int Open(const char*);
	int Insert(Key&, int);//����key����ŵ������ļ�
	int Lookup(Key&, int*);//����key��Ӧ�����
private:
	const char * path_;
};
#endif
