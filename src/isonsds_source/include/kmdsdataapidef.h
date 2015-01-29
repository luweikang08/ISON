#ifndef ISON_SDS_KMDS_DATA_API_H_
#define ISON_SDS_KMDS_DATA_API_H_
#include <sstream>
#define STOCK_APL_SELL_BID_SIZE 10
struct HQ_CODETABLE_HEAD //ʵʱ����_ͷ
{
	int           TOT_RECORD;//��¼����	
	int		      MKT_TP_CODE;//�����г�	
	unsigned int  CODETABLE_VER;//�г��汾��
};
struct HQ_PUB_HEAD//ʵʱ����_ͷ
{
	int     TOT_RECORD;//��¼����	
	int		MKT_TP_CODE;//�����г�	
};
struct HQ_CODETABLE
{
	int MKT_TP_CODE;//	�г����ʹ���
	char SCR_CODE[10];//	֤ȯ����
	char SCR_NM[128];//֤ȯ����	String
	int STK_TYPE;	//֤ȯ���	INT32
	long long YSTD_CLS_QTN_PRICE;// �����̼�	
	long long OPN_QTN_PRICE;	//���̼�
	long long RISE_LMT_PRICE;	//��ͣ��
	long long FALL_LMT_PRICE;	//��ͣ��
	long long YSTD_DELTA;	//����ʵ��
	long long YSTD_MKT_MAKE_POS_TOT_NUM;	//��ֲ�
	long long TDY_SETL_PRICE;//	���ս���
	long long YSTD_SETL_PRICE;// �����
	unsigned int CODETABLE_VER;//	�汾��
	long long ID_NUM_L1;// ��ͨ���鱾�ձ��
	long long ID_NUM_L2;// L2���鱾�ձ��
	

};

struct HQ_PUB_INDEX//ʵʱ����_ָ��
{
	int      TM;  //����ʱ��		HHMMSSmmm
	int      MKT_TP_CODE;//�����г�	
	char     SCR_CODE[10]; //֤ȯ�ֶ���
	long long  SEQ_ID;	  //�������	
	int	     HIGH_PRICE; // ��߼�	
	int	     LWS_PRICE;  //  ��ͼ�	
	int	     LTST_PRICE;//  ���¼�	
	long long	 BGN_TOT_NUM;// �ɽ�����	INT64
	long long	 BGN_TOT_AMT;//  �ɽ��ܽ��
	long long	 PUBLISH_TM1;//  һ������ʱ��	
	long long  PUBLISH_TM2;// ��������ʱ��	
	long long	 ERROR_MARK;//  �����ֶ���	
};
struct HQ_PUB_SELLINTIME//ʵʱ����_��ʳɽ�
{
	int     MKT_TP_CODE; // �����г�	INT32
	char    SCR_CODE[10];	 // ֤ȯ�ֶ���	String
	int 	TM;  // �ɽ�ʱ��	INT32
	int     DT;//�ɽ����� 
	long long	BGN_ID;//	  �ɽ����	INT64
	int	     TRDD_PRC;	//  �ɽ��۸�	INT32
	long long BGN_QTY;	  //�ɽ�����	INT64
	long long BGN_AMT; //�ɽ����	INT64
	char	BGN_FLG[56];	 // �ɽ����	String
	char	NSR_TXN_TP_CODE[64];//  ָ�������	String
};
struct HQ_PUB_STOCK
{
	int TM;//����ʱ��	
	int DT;
	int MKT_TP_CODE;//  ������
	char SCR_CODE[10];// ֤ȯ�ֶ���	
	long long SEQ_ID ; // �������	
	int PART_SNPST_ST_TP_CODE;//�ֱʿ���״̬���ʹ���	
	int HIGH_PRICE;// ��߼�	
	int LWS_PRICE;// ��ͼ�	
	int LTST_PRICE;// ���¼�

	int APL_SELL_PRC [10];//  ������	
	int APL_SELL_AMT [10];//������
	int APL_BID_PRC [10];//  �����	
	int APL_BID_AMT [10];//������

	int BGN_CNT;//  �ɽ�����
	long long BGN_TOT_NUM;//  �ɽ�����	
	long long BGN_TOT_AMT; // �ɽ��ܽ��	
	long long ENTRT_BUY_TOT_NUM; //	  ί����������	
	long long ENTRT_SELL_TOT_NUM;//	  ί����������	
	int WGHT_AVG_ENTRT_BUY_PRC; // ��Ȩƽ��ί��۸�	
	int WGHT_AVG_ENTRT_SELL_PRC; //��Ȩƽ��ί���۸�	
//	int IOPV_NET_VAL_VALT;//  IOPV��ֵ��ֵ	
	//int MTUR_YLD;// ����������	
	//int PE_RATIO_1;//  ��ӯ��1	
	//int PE_RATIO_2;// ��ӯ��2	
	//long long PUBLISH_TM1;//  һ������ʱ��	
	//long long PUBLISH_TM2;// ��������ʱ��	
	//char PRE_FIX[64];//	  ֤ȯ��Ϣǰ׺	
	//int ERROR_MARK; // �����ֶ���		
	

};
#endif