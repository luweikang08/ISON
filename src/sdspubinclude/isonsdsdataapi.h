#ifndef __ISON_SDS_DATA_API_H__ 
#define __ISON_SDS_DATA_API_H__

#include<vector>
#ifdef _WIN32
#ifndef __int64
#define __int64 long long
#endif
#define N8 __int64
#define N4 __int32
#define N2 __int16
#define C char
#else 
#define N8 long long
#define N4 int
#define N2 short
#define C char
#endif
#define LEVLEL2LENGTH 10
#define ABVOLUMELENGTH 50
#define CODELENGTH 16
#define CODENAMELENGTH 32
#define MARKETLENGTH 6
typedef  unsigned short un_short;
typedef unsigned int un_int;

//ϵͳ��Ϣ��������Ϣ�Ķ���
enum ISON_MSG_ID{
	SYSMSG = 10,
	CodeTableMSG = 11,
	SYSCloseMarket = 12,

	
	SubscribeMarketData = 21,//��������
	SubscribeMarketDataResp,//	��������Ӧ��
	UnSubscribeMarketData,	//ȡ�������г�����
	UnSubscribeMarketDataResp,//	ȡ�������г�����Ӧ��
	SubscribeSignal,//�����ź�
	SubscribeSignalResp,//�����ź�Ӧ��
	UnSubscribeSignal,//ȡ�������ź�
	UnSubscribeSignalResp,//ȡ�������ź�Ӧ��
	SubscribeIndicator,//����ָ��
	SubscribeIndicatorResp,//����ָ��Ӧ��

	GetMarketInfo = 41,//��ѯ�г���Ϣ
	GetMarketInfoResp,//��ѯ�г���ϢӦ��
	GetStockInfo,//��ѯ��Ʊ��Ϣ
	GetStockInfoResp,//��ѯ��Ʊ��ϢӦ��
	GetMarketData,//��ѯ�г�����
	GetMarketDataResp,//��ѯ�г�����Ӧ��
	GetIndicator,//��ѯ��ʷָ��
	GetIndicatorResp,//��ѯ��ʷָ��Ӧ��
	GetMarketRank,//��ѯʵʱ����
	GetMarketRankResp,//��ѯʵʱ����Ӧ��
	GetCodeTable,
	GetCodeTableResp,


	MatchPrice = 81,//�ɽ��۸�
	MaketData,//��������
	Level2,//Level2//����
	Transaction,//��ʳɽ�
	Order,//���ί��
	OrderQueue,//ί�ж���
	Signal,//�ź�����
	Indicator ,//ָ������

	
};

//�����붨��

enum SDS_ERR
{

	SDS_ERR_UNKOWN = 2000,                // δ֪����

	SDS_ERR_INITIALIZE_FAILURE = 2001,  // ��ʼ������ʧ��
	SDS_ERR_NETWORK_ERROR,              // �������ӳ�������
	SDS_ERR_INVALID_PARAMS,             // ���������Ч
	SDS_ERR_VERIFY_FAILURE,             // ��½��֤ʧ�ܣ�ԭ��Ϊ�û�������������󣻳�����½����
	SDS_ERR_NO_AUTHORIZED_MARKET,       // ����������г���û����Ȩ
	SDS_ERR_NO_CODE_TABLE,              // ����������г����춼û�д����

	SDS_ERR_SUCCESS = 0,                // �ɹ�
};


/*
enum ISON_SDS2TGW{
	TSZ_SYSDATA = 1000,//���ڽ�����	SYSDATA	ϵͳ����	
	TSZ_LEVEL1DATA = 1001,	//���ڽ�����	LEVEL1DATA	LEVEL1����	
	TSZ_LEVEL2 = 1002,	//���ڽ�����	LEVEL2DATA	LEVEL2����
	TSZ_ORDERQUEUE = 1003,	//���ڽ�����	ORDERQUEUE	��������	
	TSZ_TRANSACTION = 1004,	//���ڽ�����	TRANSACTION	��ʳɽ�	

	TSH_SYSDATA = 1100,	//�Ϻ�������	SYSDATA	ϵͳ����	
	TSH_LEVEL1DATA = 1101,//�Ϻ�������	LEVEL1DATA	LEVEL1����	
	TSH_LEVEL2 = 1102,	//�Ϻ�������	LEVEL2DATA	LEVEL2����	
	TSH_ORDERQUEUE = 1103,	//�Ϻ�������	ORDERQUEUE	��������	
	TSH_TRANSACTION = 1104	//�Ϻ�������	TRANSACTION	��ʳɽ�	
};*/

enum ISON_SDS2TGW{
TSZ_SYSDATA = 1010,//���ڽ�����	SYSDATA	ϵͳ����
TSZ_LEVEL1DATA = 1011,	//���ڽ�����	LEVEL1DATA	LEVEL1����
TSZ_LEVEL2 = 1012,	//���ڽ�����	LEVEL2DATA	LEVEL2����
TSZ_ORDERQUEUE = 1013,	//���ڽ�����	ORDERQUEUE	��������
TSZ_TRANSACTION = 1014,	//���ڽ�����	TRANSACTION	��ʳɽ�

TSH_SYSDATA = 1110,	//�Ϻ�������	SYSDATA	ϵͳ����
TSH_LEVEL1DATA = 1111,//�Ϻ�������	LEVEL1DATA	LEVEL1����
TSH_LEVEL2 = 1112,	//�Ϻ�������	LEVEL2DATA	LEVEL2����
TSH_ORDERQUEUE = 1113,	//�Ϻ�������	ORDERQUEUE	��������
TSH_TRANSACTION = 1114	//�Ϻ�������	TRANSACTION	��ʳɽ�
};

struct TOPICHEAD{
	un_short topic; // ����,�������б�
	un_short ms; // ����0~999
	un_int kw;  // ֤ȯ���룬һ��ֱ����atoi�Ϳ����ˣ�
	un_int sn;  // ���к�,�������
	un_int tm;  // �ӣ������������ڵ�������
};

struct SDS_Level2{
	N4	Seqno;	//��Ϣ���
	C Code[CODELENGTH];	//֤ȯ����, ��000001.SH
	N4 Date ;	//,����YYYYMMDD
	N4 Time;	//ʱ��HHMMSSmmm
	N4 SDSTime;//����SDS��ʱ�� //12.23
	N4 Sn;//���кţ����ڲ��������Ƿ�ʧ//12.23
	N4 PreClose; //ǰ���̼�			
	N4 Open; // ���̼�
	N4 High; // ��߼�
	N4	Low; // ��ͼ�
	N4	Price; // ���¼�
	N4 NumTrades;//�ɽ�����
	N8	Volume; // �ɽ�����
	N8  Turnover; //�ɽ��ܽ��
	N8  TotalBidVol;//ί����������  //add in 11-27
	N8 TotalAskVol;//ί����������
	N4 AvgBidPrice;//��Ȩƽ��ί��۸�
	N4 AvgAskPrice;//��Ȩƽ��ί���۸�
	N4 HighLimited;//��ͣ��
	N4 LowLimited;//��ͣ��        //add in 11-27

	N4	AskPrice[LEVLEL2LENGTH];	//10��������
	N4	AskVol[LEVLEL2LENGTH];  //	10��������
	N4	BidPrice[LEVLEL2LENGTH]; // 10�������
	N4	BidVol[LEVLEL2LENGTH]; //	10��������
};
struct SDS_Transaction{
	N4  Seqno;//��Ϣ���
	C Code[CODELENGTH];//֤ȯ����
	N4 	Date;//����
	N4 Time;//ʱ��
	N4 SDSTime;//����SDS��ʱ�� //12.23
	N4 Sn;//���кţ����ڲ��������Ƿ�ʧ//12.23
	N4 Index;//�ɽ����
	N4 Price;//�ɽ��۸�
	N4 Volume;//�ɽ�����
	N4 Turnover;//�ɽ����
	C BSFlag;//��������
	C OrderKind;//�ɽ����
	N4 FunctionCode;//�ɽ�����
	N4 AskOrder;//������ί�����
	N4 BidOrder;//����ί�����		
};
struct SDS_Order{
	N4 Seqno;//
	C Code[CODELENGTH];
	N4 Date;
	N4 Time;
	N4 SDSTime;//����SDS��ʱ�� //12.23
	N4 Sn;//���кţ����ڲ��������Ƿ�ʧ//12.23
	N4 Order;
	N4 Price;
	N4 Volume;
	C OrderKind;
	C FunctionCode;
};
struct SDS_OderQueen{
	N4 Seqno;//��Ϣ���
	C Code[CODELENGTH];//֤ȯ����
	N4 Date;//����
	N4 Time;//ʱ��
	N4 SDSTime;//����SDS��ʱ�� //12.23
	N4 Sn;//���кţ����ڲ��������Ƿ�ʧ//12.23
	C Side;//��������
	N4 Price;//ί�м۸�
	N4 Orders;//��������
	N4 ABItems;//ABItems
	N4 ABVolume[ABVOLUMELENGTH];//ABVolume 
};
struct SDS_Code{
	C Code[CODELENGTH];    //Wind Code: AG1302.SHF
	C ENName[CODENAMELENGTH];
	C CNName[CODENAMELENGTH];      //chinese name: ����1302
};
struct SDS_CloseMarket{
	N4 Seqno;//��Ϣ���  12
	N4 Time;//����ʱ��  SH��Ϊ�и۹�����16��00����
	C Market[MARKETLENGTH];//�����г�SZ����SH
};
struct SubscribeMarketDataReq{
	N4  seqno;     //	��Ϣ��ſͻ�ָ����(�����ظ�)
	N2	date_type;//	���ĵ���������
	N4 	begin_date;//	��ʼ���� YYMMDD�� 0Ϊ����
	N4	begin_time;	//��ʼʱ��	HHMMSSmmm��0Ϊ��ʱ
	N4	end_date;//	��������	YYMMDD, 0Ϊ������
	N4 	end_time;//����ʱ��	HHMMSSmmm��0Ϊ����
	std::vector <std::string> code_table;//֤ȯ�����C��	Ʒ�ִ�����ֺŸ�������000001.SH; 600001.SZ	
};
struct SYSTEMDATA{
	un_short Type;//ϵͳ��Ϣ���� 12 ������Ϣ
	char info[70];//������Ϣ  �á�|���ָ�����
};

#endif

