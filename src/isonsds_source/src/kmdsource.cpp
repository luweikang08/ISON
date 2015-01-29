#include <iostream>
#include <stdint.h>
#include <string>
#include<vector>
#include <map>
#include<fstream>
#include<mutex>
#include "isonbase.h"
#include "context.h"
#include "event.h"
#include "ibprotocol.h"
#include "socket.h"
#include"message.h"

#include <libckmdsapi/ckmdsdef.h>
#include <libckmdsapi/ckmdsuserapi.h>
#include <libckmdsapi/cmsgdata.h>
#include <libckmdsapi/ctabledata.h>

#include "document.h"
#include "prettywriter.h"
#include "filestream.h"
#include "stringbuffer.h"
#include "writer.h"

#include"ibsdstool.h"
#include "timehelper.h"
#include "kmdsdataapidef.h"
#include "isonsdsdataapi.h"
#include "sourcepublic.h"
#include "jsonhelper.h"
#include "isonutil.h"
#include "sbeinterface.h"

//#include <Windows.h>
#define	STOCK_APL_SELL_BID_SIZE  10 //����/�����鳤��
using namespace ison::base;
using namespace std;
using namespace rapidjson;
extern Socket  *g_socket;//ȫ�־�̬Socket�����ڽ���KMDS�ӿ�PUSH������
extern Context ctx;
std::mutex g_Mutex;
KMDSUSERAPIHANDLE pclient;
extern int g_nlog_level;
extern int transaction_count;
extern int level2_count ;
extern std::vector<std::string> code_filter;//��Ʊ����
extern bool filter_code_flag;
map<std::string, HQ_CODETABLE>  g_codetablemap;//ȫ�־�̬���������ڴ洢
extern Socket  *g_socket;//ȫ�־�̬Socket�����ڽ���KMDS�ӿ�PUSH������
extern int g_nlog_level;
extern int g_tdf_time_sn;
DateAndTime nowdate = GetDateAndTime();

void  OnKmdsMsg(void* pmsgdesc, void* pmsgopts, int ncmd, MSGDATAHANDLE pmsg)
{
	LOG_IF(INFO, g_nlog_level>5) << "KMDS callback in ";
	g_Mutex.lock();//�Ի���������
	if (pmsg == NULL)   { return; }
	switch (ncmd)
	{
	case CKMDS_HQ_CODETABLE://֤ȯ����� ���͸�����Ϣ
	{ 
		//����֤ȯ�����  ����sn
		unsigned int sn;
		SnInit(sn);//��ʼ��sn
		transaction_count = level2_count= sn;
		HQ_CODETABLE_HEAD codetablehead;
		HQ_CODETABLE codetable;
		TABLEDATAHANDLE ptable = NULL;
		MSGDATAHANDLE   prow = NULL;
		CMsgData_GetData_Int32(pmsg, "TOT_RECORD", &codetablehead.TOT_RECORD);//��¼����	
		CMsgData_GetData_Int32(pmsg, "MKT_TP_CODE", &codetablehead.MKT_TP_CODE);//�����г�
		CMsgData_PutData_UInt32(pmsg, "CODETABLE_VER", codetablehead.CODETABLE_VER);
		DateAndTime updatecodetabledate = GetDateAndTime();
		printf("���´����-�г�:%d, ���������:%d, ���������:%d\n", codetablehead.MKT_TP_CODE, codetablehead.TOT_RECORD, updatecodetabledate.date);
		LOG(INFO) << "���´����-�г�:" << codetablehead.MKT_TP_CODE << "���������:" << codetablehead.TOT_RECORD;
		CMsgData_GetData_TableData(pmsg, "SEQ_HQ_CODETABLE", &ptable);
		if (ptable)
		{
			int  nrowcount = 0;
			CTableData_GetCount(ptable, (unsigned int *)&nrowcount); //�ý������¼��
			for (int i = 0; i < nrowcount; ++i)
			{
				CTableData_GetRow(ptable, i, &prow);
				if (prow)
				{
					rapidjson::Document document;
					document.SetObject();
					rapidjson::Document::AllocatorType& allocator = document.GetAllocator();//һ��һ�����ݷ�
					CMsgData_GetData_Int32(prow, "MKT_TP_CODE", &codetable.MKT_TP_CODE);// �����г�
					CMsgData_GetData_String(prow, "SCR_CODE", (unsigned char *)&codetable.SCR_CODE, sizeof(codetable.SCR_CODE));
					CMsgData_GetData_String(prow, "SCR_NM", (unsigned char *)&codetable.SCR_NM, sizeof(codetable.SCR_NM));
					CMsgData_GetData_String(prow, "STK_TYPE", (unsigned char *)&codetable.STK_TYPE, sizeof(codetable.STK_TYPE));//���
					CMsgData_GetData_Int64(prow, "YSTD_CLS_QTN_PRICE", &codetable.YSTD_CLS_QTN_PRICE);//���
					CMsgData_GetData_Int64(prow, "OPN_QTN_PRICE", &codetable.OPN_QTN_PRICE);//���¼۸�
					CMsgData_GetData_Int64(prow, "RISE_LMT_PRICE", &codetable.RISE_LMT_PRICE);//�ɽ���
					CMsgData_GetData_Int64(prow, "FALL_LMT_PRICE", &codetable.FALL_LMT_PRICE);//�ɽ���
					CMsgData_GetData_Int64(prow, "YSTD_DELTA", &codetable.YSTD_DELTA);
					CMsgData_GetData_Int64(prow, "YSTD_MKT_MAKE_POS_TOT_NUM", &codetable.YSTD_MKT_MAKE_POS_TOT_NUM);
					CMsgData_GetData_Int64(prow, "TDY_SETL_PRICE", &codetable.TDY_SETL_PRICE);
					CMsgData_GetData_Int64(prow, "YSTD_SETL_PRICE", &codetable.YSTD_SETL_PRICE);//�ɽ���
					CMsgData_GetData_UInt32(prow, "CODETABLE_VER", &codetable.CODETABLE_VER);
					CMsgData_GetData_Int64(prow, "ID_NUM_L1", &codetable.ID_NUM_L1);
					CMsgData_GetData_Int64(prow, "ID_NUM_L2", &codetable.ID_NUM_L2);
					g_codetablemap[codetable.SCR_CODE] = codetable;//�Ѵ����洢��map�У����µĿ��Ը���
					char send_buffer[bufferlength];
					memset(send_buffer, bufferlength, 0);
					baseline::SDS_Code  sds_code_table= {};
					baseline::MessageHeader hdr;
					hdr.wrap(send_buffer, 0, messageHeaderVersion, bufferlength)
						.blockLength(baseline::SDS_Code::sbeBlockLength())
						.templateId(baseline::SDS_Code::sbeTemplateId())
						.schemaId(baseline::SDS_Code::sbeSchemaId())
						.version(baseline::SDS_Code::sbeSchemaVersion());
					//��data����encode
					sds_code_table.wrapForEncode(send_buffer, hdr.size(), bufferlength)
						.seqno((int)ISON_MSG_ID::CodeTableMSG)
						.putCode(codetable.SCR_CODE)
						.putENName("")
						.putCNName(codetable.SCR_NM)
						.type(codetable.STK_TYPE);
					std::string scodetable = Document2String(document);	//Ttansfer to string 
					int res = MessageSend(itostring(SYSMSG).c_str(), scodetable.c_str(), g_socket);
				}
			}

			CTableData_Release(ptable);
			ptable = NULL;
		}
	};
		break;

	case CKMDS_HQ_STK_PART_SNPST:  // 6010 ��Ʊ��������	����
	{
		level2_count++;
		HQ_PUB_STOCK stockpub;
		TABLEDATAHANDLE ptable = NULL;
		MSGDATAHANDLE   prow = NULL;
		//CMsgData_GetData_Int32(pmsg, "TOT_RECORD", &hqpubhead.TOT_RECORD);//��¼����
		//CMsgData_GetData_Int32(pmsg, "MKT_TP_CODE", &hqpubhead.MKT_TP_CODE);//�����г�
		CMsgData_GetData_TableData(pmsg, "SEQ_HQ_SNAPSHOT", &ptable);   //�ռ�������������
		if (ptable)
		{
			int  nrowcount = 0;
			CTableData_GetCount(ptable, (unsigned int *)&nrowcount); //�ý������¼��
			for (int i = 0; i < nrowcount; ++i)//ÿ��nrowcount��Ϊ1  һ��һ������
			{
				rapidjson::Value object(rapidjson::kObjectType);  //ûһ�������¼Ϊһ������
				CTableData_GetRow(ptable, i, &prow);
				if (prow)
				{
					DateAndTime level2_source_time = GetDateAndTime();
					HQ_CODETABLE stockinfo;
					//ʵʱ����_��Ʊ
					//CMsgData_GetData_Int32(prow, "DT", &stockpub.DT);//����
					stockpub.DT = nowdate.date;
					CMsgData_GetData_Int32(prow, "TM", &stockpub.TM);// ����ʱ��
					CMsgData_GetData_Int32(prow, "MKT_TP_CODE", &stockpub.MKT_TP_CODE);// �����г�
					CMsgData_GetData_String(prow, "SCR_CODE", (unsigned char *)&stockpub.SCR_CODE, sizeof(stockpub.SCR_CODE));//֤ȯ�ֶ�
					CMsgData_GetData_Int32(prow, "HIGH_PRICE", &stockpub.HIGH_PRICE);//���
					CMsgData_GetData_Int32(prow, "LWS_PRICE", &stockpub.LWS_PRICE);//���
					CMsgData_GetData_Int32(prow, "LTST_PRICE", &stockpub.LTST_PRICE);//���¼۸�
					//����/�� ������
					for (int i = 0; i < STOCK_APL_SELL_BID_SIZE; i++)
					{
						
						CMsgData_GetData_Int32(prow, (char*)(concatenate("APL_SELL_PRC_", i + 1).c_str()), &stockpub.APL_SELL_PRC[i]);
						CMsgData_GetData_Int32(prow, (char*)(concatenate("APL_SELL_AMT_", i + 1).c_str()), &stockpub.APL_SELL_AMT[i]);
						CMsgData_GetData_Int32(prow, (char*)(concatenate("APL_BID_PRC_", i + 1).c_str()), &stockpub.APL_BID_PRC[i]);
						CMsgData_GetData_Int32(prow, (char*)(concatenate("APL_BID_AMT_", i + 1).c_str()), &stockpub.APL_BID_AMT[i]);
					}
					CMsgData_GetData_Int64(prow, "BGN_TOT_NUM", &stockpub.BGN_TOT_NUM);//�ɽ���
					CMsgData_GetData_Int64(prow, "BGN_TOT_AMT", &stockpub.BGN_TOT_AMT);//�ɽ���
					CMsgData_GetData_Int32(prow, "BGN_CNT", &stockpub.BGN_CNT);//�ɽ�����
					CMsgData_GetData_Int64(prow, "ENTRT_BUY_TOT_NUM", &stockpub.ENTRT_BUY_TOT_NUM);//ί����������
					CMsgData_GetData_Int64(prow, "ENTRT_SELL_TOT_NUM", &stockpub.ENTRT_SELL_TOT_NUM);//ί����������
					CMsgData_GetData_Int32(prow, "WGHT_AVG_ENTRT_BUY_PRC", &stockpub.WGHT_AVG_ENTRT_BUY_PRC);//��Ȩƽ��ί��۸�
					CMsgData_GetData_Int32(prow, "WGHT_AVG_ENTRT_SELL_PRC", &stockpub.WGHT_AVG_ENTRT_SELL_PRC);// ��Ȩƽ��ί���۸�
					LOG_IF(INFO, g_nlog_level > 5) << "SDS get from KMDS transcation :" << "SDS get from KMDS transcation:" << "Code:" << stockpub.SCR_CODE << "." << stockpub.MKT_TP_CODE << "Time:" << stockpub.TM << "Price:" << stockpub.LTST_PRICE << "Volume:" << stockpub.BGN_TOT_NUM;
					LOG_IF(INFO, (level2_count % 10000) == 0) << "SDS get from KMDS transcation * :" << "SDS get from KMDS transcation:" << "Code:" << stockpub.SCR_CODE << "." << stockpub.MKT_TP_CODE << "Time:" << stockpub.TM << "Price:" << stockpub.LTST_PRICE << "Volume:" << stockpub.BGN_TOT_NUM;
					string code = string(stockpub.SCR_CODE);
					//���	 
					bool codeflag_level2 = 1;//Ĭ�϶���Ҫ
					if (filter_code_flag)//��Ҫ����
						codeflag_level2 = CodeIsFilter(code.c_str(), code_filter);
					if (codeflag_level2)
					{			
						string maket;

						switch (stockpub.MKT_TP_CODE)
						{
						case 1:///�Ͻ���
						{ maket = "SH"; break; };
						case 2:
						{maket = "SZ"; break; };
						default:LOG(INFO) << "market:" << stockpub.MKT_TP_CODE;
						}
						code += "." + maket;
						try{ stockinfo = g_codetablemap.at(string(stockpub.SCR_CODE)); }
						catch (std::exception)
						{
							break;
						}

						level2_count++;
						char send_buffer[bufferlength];
						memset(send_buffer, bufferlength, 0);
						baseline::SDS_Level2 sds_level2 ;
						baseline::MessageHeader hdr ;
						//�ȶ�ͷ����encode
						hdr.wrap(send_buffer, 0, messageHeaderVersion, bufferlength)
							.blockLength(baseline::SDS_Level2::sbeBlockLength())
							.templateId(baseline::SDS_Level2::sbeTemplateId())
							.schemaId(baseline::SDS_Level2::sbeSchemaId())
							.version(baseline::SDS_Level2::sbeSchemaVersion());
						//��data����encode
						sds_level2.wrapForEncode(send_buffer, hdr.size(), bufferlength);
						for (int k = 0; k < 10; k++)
						{
							sds_level2.askPrice(k, stockpub.APL_SELL_PRC[k]);
							sds_level2.askVol(k, stockpub.APL_SELL_AMT[k]);
							sds_level2.bidPrice(k,stockpub.APL_BID_PRC[k]);
							sds_level2.bidVol(k, stockpub.APL_BID_AMT[k]);
						}
						sds_level2.seqno((int)ISON_MSG_ID::Level2)
							.sDSTime(level2_source_time.time)
							.sn(level2_count)
							.date(stockpub.DT)
							.time(stockpub.TM)
							.preClose((int)stockinfo.YSTD_CLS_QTN_PRICE)
							.open((int)stockinfo.OPN_QTN_PRICE)
							.high(stockpub.HIGH_PRICE)
							.low(stockpub.LWS_PRICE)
							.price(stockpub.LTST_PRICE)
							.volume(stockpub.BGN_TOT_NUM)
							.turnover(stockpub.BGN_TOT_AMT)
							.totalBidVol(stockpub.ENTRT_BUY_TOT_NUM)
							.totalAskVol(stockpub.ENTRT_SELL_TOT_NUM)
							.numTrades(stockpub.BGN_CNT)
							.avgBidPrice(stockpub.WGHT_AVG_ENTRT_BUY_PRC)
							.avgAskPrice(stockpub.WGHT_AVG_ENTRT_SELL_PRC)
							.highLimited((int)stockinfo.RISE_LMT_PRICE)
							.lowLimited((int)stockinfo.FALL_LMT_PRICE)
							.putCode(code.c_str());
						for (int i = 0; i < 10; i++)
						{
							//printf("kmds->bid%2d:%9d num%2d:%9d | ask%2d:%9d num%2d:%9d\n", i, stockpub.APL_BID_PRC[i], i, stockpub.APL_BID_AMT[i], i, stockpub.APL_SELL_PRC[i], i, stockpub.APL_SELL_AMT[i]);
							//printf("level2->bid%2d:%9d num%2d:%9d | ask%2d:%9d num%2d:%9d\n", i, sds_level2.bidPrice(i), i, sds_level2.bidVol(i), i, sds_level2.askPrice(i), i, sds_level2.askVol(i));
						}
						int len = hdr.size() + sds_level2.size();
						string send_message(send_buffer, len);
						MessageSend(itostring(Level2).c_str(), send_message.c_str(), len, g_socket);
						//LOG_IF(INFO,g_nlog_level>5)<<"SDS  publish" << level2.c_str();
						//LOG_IF(INFO, (level2_count % 10000) == 0) << "SDS publish  Level2 *:"<<level2.c_str() ;
					}
				}
			}
				
			}
			CTableData_Release(ptable);
			ptable = NULL;
		};

		break;
	case CKMDS_HQ_PER_STRK_BGN: //�ռ���ʳɽ�
	{
		HQ_PUB_SELLINTIME sellintime;
		TABLEDATAHANDLE ptable = NULL;
		MSGDATAHANDLE   prow = NULL;
		//CMsgData_GetData_Int32(pmsg, "TOT_RECORD", &hqpubhead.TOT_RECORD);//��¼����
		//CMsgData_GetData_Int32(pmsg, "MKT_TP_CODE", &hqpubhead.MKT_TP_CODE);//�����г�
		CMsgData_GetData_TableData(pmsg, "SEQ_HQ_STRK_BGN", &ptable);//��ȡ��ʳɽ�����
		if (ptable)
		{
			int  nrowcount = 0;
			CTableData_GetCount(ptable, (unsigned int *)&nrowcount); //�ý������¼��
			for (int i = 0; i < nrowcount; ++i)
			{
				transaction_count++;
				CTableData_GetRow(ptable, i, &prow);
				if (prow)
				{
					CMsgData_GetData_Int32(prow, "MKT_TP_CODE", &sellintime.MKT_TP_CODE);// �����г�
					CMsgData_GetData_String(prow, "SCR_CODE", (unsigned char *)&sellintime.SCR_CODE, sizeof(sellintime.SCR_CODE));
					CMsgData_GetData_Int32(prow, "TM", &sellintime.TM);
					sellintime.DT = nowdate.date;
					CMsgData_GetData_Int64(prow, "BGN_ID", &sellintime.BGN_ID);
					CMsgData_GetData_Int32(prow, "TRDD_PRC", &sellintime.TRDD_PRC);//�ɽ��۸�
					CMsgData_GetData_Int64(prow, "BGN_QTY", &sellintime.BGN_QTY);
					CMsgData_GetData_Int64(prow, "BGN_AMT", &sellintime.BGN_AMT);//�ɽ����
					CMsgData_GetData_String(prow, "BGN_FLG", (unsigned char *)&sellintime.BGN_FLG, sizeof(sellintime.BGN_FLG));//�ɽ����
					CMsgData_GetData_String(prow, "NSR_TXN_TP_CODE", (unsigned char *)&sellintime.NSR_TXN_TP_CODE, sizeof(sellintime.NSR_TXN_TP_CODE));
					LOG_IF(INFO, g_nlog_level > 5) << "SDS get from KMDS transcation:" << "Code:" << sellintime.SCR_CODE<<"."<<sellintime.MKT_TP_CODE<<"Time:"<<sellintime.TM<<"Index:"<<sellintime.BGN_ID<<"Price:"<<sellintime.TRDD_PRC<<"Volume:"<<sellintime.BGN_QTY<<"Turnover:"<<sellintime.BGN_AMT<<"BSFlag"<<sellintime.BGN_FLG<<"NSR_TXN:sellintime.NSR_TXN_TP_CODE";
					LOG_IF(INFO, (level2_count % 10000) == 0) << "SDS get from KMDS transcation * :" << "SDS get from KMDS transcation:" << "Code:" << sellintime.SCR_CODE << "." << sellintime.MKT_TP_CODE << "Time:" << sellintime.TM << "Index:" << sellintime.BGN_ID << "Price:" << sellintime.TRDD_PRC << "Volume:" << sellintime.BGN_QTY << "Turnover:" << sellintime.BGN_AMT << "BSFlag" << sellintime.BGN_FLG << "NSR_TXN:sellintime.NSR_TXN_TP_CODE";
					//���

					string market;
					string code = (string)sellintime.SCR_CODE;
	
					
					bool codeflag_trans = 1;//Ĭ�϶���Ҫ
					if (filter_code_flag)//��Ҫ����
						codeflag_trans = CodeIsFilter(code.c_str(), code_filter);
					if (codeflag_trans)
					{
						
						//����stockcode
						switch (sellintime.MKT_TP_CODE)
						{
						case 1:///�Ͻ���
						{ market = "SH"; break; };
						case 2:
						{market = "SZ"; break; };
						default:LOG(INFO) << "market:" << sellintime.MKT_TP_CODE;
						}
						code += "." + market;

						DateAndTime trans_source_time = GetDateAndTime();
						char send_buffer[bufferlength];
						memset(send_buffer, bufferlength, 0);
						baseline::SDS_Transaction sds_trans ;
						baseline::MessageHeader hdr;
						int data_length = 1024;
						//�ȶ�ͷ����encode
						hdr.wrap(send_buffer, 0, messageHeaderVersion, bufferlength)
							.blockLength(baseline::SDS_Transaction::sbeBlockLength())
							.templateId(baseline::SDS_Transaction::sbeTemplateId())
							.schemaId(baseline::SDS_Transaction::sbeSchemaId())
							.version(baseline::SDS_Transaction::sbeSchemaVersion());
						//��data����encode
						sds_trans.wrapForEncode(send_buffer, hdr.size(), bufferlength)
							.sDSTime(trans_source_time.time)
							.sn(transaction_count)
							.seqno((int)ISON_MSG_ID::Transaction)////��Ϣ��� 84
							.date(sellintime.DT)//��Ȼ��
							.time(sellintime.TM)//ʱ��(HHMMSSmmm)
							.index((int)sellintime.BGN_ID)//�ɽ����
							.price(sellintime.TRDD_PRC)//�ɽ��۸�
							.volume((int)sellintime.BGN_QTY)//�ɽ�����
							.turnover((int)sellintime.BGN_AMT)//�ɽ����
							.bSFlag(NULL)//��������
							.orderKind(NULL)//�ɽ����
							.functionCode(NULL)//�ɽ�����
							.askOrder(NULL)//������ί�����
							.bidOrder(NULL)//����ί�����
							.putCode(code.c_str());
						int len = hdr.size() + sds_trans.size();
						string send_message(send_buffer, len);
						MessageSend(itostring(Transaction).c_str(), send_message.c_str(), len, g_socket);		
				}

				}
			}

				CTableData_Release(ptable);
				ptable = NULL;
			}
		};

		break;
	default:
	{
		if (pmsgdesc)
		{
			//printf("Receive Subject:%s\n", pmsgdesc);
		}
	}
		break;

	}
		g_Mutex.unlock(); //�Ի���������

	}

int  KMDSCodeTableReq(int mktcode, map<std::string, HQ_CODETABLE>& g_codetablemap)
{ 
	LOG_IF(INFO,g_nlog_level >5) << "Market" << mktcode << "'s CodeTable  bengin!";
	rapidjson::Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	MSGDATAHANDLE preqdata = CMsgData_Create();
	MSGDATAHANDLE pans = NULL;
	CMsgData_PutData_Int32(preqdata, "MKT_TP_CODE", mktcode);//��������
	int nret = KmdsUserApi_GetHqData(pclient, CKMDS_HQ_CODETABLE, preqdata, &pans);
	//����
	if (nret != 0)    //0��ʾ�ɹ�,���ʧ��
	{
		printf("KMDS���������ʧ�ܣ�%d,%s", KmdsUserApi_GetErrCode(pclient), KmdsUserApi_GetErrMsg(pclient));
	}
	else{
		if (pans)
		{
			//...
			//ҵ�������
			//��ȡ�����, DATA0, [DATA1,DATA2,...]
			TABLEDATAHANDLE  ptable = NULL;
			MSGDATAHANDLE    prow = NULL;
			int              nrowcount = 0;
			int				 i = 0;
			CMsgData_GetData_TableData(pans, "DATA0", &ptable);
			if (ptable)
			{
				DateAndTime codetabledate = GetDateAndTime();
				CTableData_GetCount(ptable, (unsigned int *)&nrowcount); //�ý������¼��
				printf("�г�:%d, ���������:%d, ���������:%d\n", mktcode, nrowcount, codetabledate.date);
				for (i = 0; i < nrowcount; ++i)
				{
					CTableData_GetRow(ptable, i, &prow);
					if (prow)
					{
						HQ_CODETABLE codetable;
						//���֤ȯ�����
						CMsgData_GetData_Int32(prow, "MKT_TP_CODE", &codetable.MKT_TP_CODE);// �����г�
						CMsgData_GetData_String(prow, "SCR_CODE", (unsigned char *)&codetable.SCR_CODE, sizeof(codetable.SCR_CODE));
						CMsgData_GetData_String(prow, "SCR_NM", (unsigned char *)&codetable.SCR_NM, sizeof(codetable.SCR_NM));
						CMsgData_GetData_Int32(prow, "STK_TYPE", &codetable.STK_TYPE);//���
						CMsgData_GetData_Int64(prow, "YSTD_CLS_QTN_PRICE", &codetable.YSTD_CLS_QTN_PRICE);//���
						CMsgData_GetData_Int64(prow, "OPN_QTN_PRICE", &codetable.OPN_QTN_PRICE);//���¼۸�
						CMsgData_GetData_Int64(prow, "RISE_LMT_PRICE", &codetable.RISE_LMT_PRICE);//�ɽ���
						CMsgData_GetData_Int64(prow, "FALL_LMT_PRICE", &codetable.FALL_LMT_PRICE);//�ɽ���
						CMsgData_GetData_Int64(prow, "YSTD_DELTA", &codetable.YSTD_DELTA);
						CMsgData_GetData_Int64(prow, "YSTD_MKT_MAKE_POS_TOT_NUM", &codetable.YSTD_MKT_MAKE_POS_TOT_NUM);
						CMsgData_GetData_Int64(prow, "TDY_SETL_PRICE", &codetable.TDY_SETL_PRICE);
						CMsgData_GetData_Int64(prow, "YSTD_SETL_PRICE", &codetable.YSTD_SETL_PRICE);//�ɽ���
						CMsgData_GetData_UInt32(prow, "CODETABLE_VER", &codetable.CODETABLE_VER);
						CMsgData_GetData_Int64(prow, "ID_NUM_L1", &codetable.ID_NUM_L1);
						CMsgData_GetData_Int64(prow, "ID_NUM_L2", &codetable.ID_NUM_L2);
						g_codetablemap[codetable.SCR_CODE] = codetable;//
						//�����json��ʽ
						char send_buffer[bufferlength];
						memset(send_buffer, bufferlength, 0);
						baseline::SDS_Code  sds_code_table = {};
						baseline::MessageHeader hdr;
						hdr.wrap(send_buffer, 0, messageHeaderVersion, bufferlength)
							.blockLength(baseline::SDS_Code::sbeBlockLength())
							.templateId(baseline::SDS_Code::sbeTemplateId())
							.schemaId(baseline::SDS_Code::sbeSchemaId())
							.version(baseline::SDS_Code::sbeSchemaVersion());
						//��data����encode
						sds_code_table.wrapForEncode(send_buffer, hdr.size(), bufferlength)
							.seqno((int)ISON_MSG_ID::CodeTableMSG)
							.putCode(codetable.SCR_CODE)
							.putENName("")
							.putCNName(codetable.SCR_NM)
							.type(codetable.STK_TYPE);
						int len = hdr.size() + sds_code_table.size();
						string send_message(send_buffer, len);
						MessageSend(itostring(SYSMSG).c_str(), send_message.c_str(), len, g_socket);
					}
				}
				CTableData_Release(ptable);
				ptable = NULL;

			}
			else
			{//������
			}

			CMsgData_Release(pans);
			pans = NULL;

			if (preqdata)
			{
				CMsgData_Release(preqdata);
			}

		}
	
	}
	LOG_IF(INFO,g_nlog_level >5) << "Market" << mktcode << "'s CodeTable  end!";
	return 0;
}

int KmdsSubscribe(const char * topic)
{
int failflag = KmdsUserApi_Subscribe(pclient, topic);
if (failflag != 0)
LOG(WARNING) << "KMDS Subscribe" << topic << " failure!" << "Message:[" << KmdsUserApi_GetErrCode(pclient) << "]" << KmdsUserApi_GetErrMsg(pclient);
else
LOG(INFO) << " KMDS Subscribe"<< topic<<"success";
return failflag;
}

int GetDataFromKMDS(){
	LOG(INFO) << "SDS source <<<< KMDS ";
	//��ȡ�����ļ�
	const char* user_name = "test";
	const char* password= "888888";
	string submarket = "SH;SZ";
	std::vector<std::string> configkey = {"UserName", "Password","SubMarket"};
	map<std::string, std::string> configmap;//��������ļ��е�����
	int ret=ReadConfig("./sourcekmds.json", configkey, configmap);//��ȡ�����ļ�
	LOG_IF(WARNING, ret!=0) << "Read configfile fail";
	user_name = configmap["UserName"].c_str();
	password = configmap["Password"].c_str();
	submarket = configmap["SubMarket"];
	std::vector<std::string> market_spit;
	split(submarket, ";", &market_spit);
	pclient = KmdsUserApi_Create();//Create the KMDSUserApi 
	if (pclient == NULL)
	{
		return -1;
		LOG(ERROR) << "KMDSUserApi create fail\n ";
	}
	LOG_IF(INFO, g_nlog_level  > 5) << "KMDSUserApi create successfully\n";

	int nret = KmdsUserApi_Init(pclient);//KmdsUserApi Init    
	if (nret != 0)//KmdsUserApi init fail!
	{
		KmdsUserApi_UnInit(pclient);
		KmdsUserApi_Destory(pclient);
		LOG(WARNING) << KmdsUserApi_GetErrCode(pclient) << ":" << KmdsUserApi_GetErrMsg(pclient);
		return nret;
	}
	LOG_IF(INFO, g_nlog_level  > 5) << "KmdsUserApi_Login ...";
	nret = KmdsUserApi_Login(pclient, user_name, password);//KmdsUserApi Login 
	if (nret == 0)
	{
		printf("Welcome to ison sdssource!\n");
		LOG_IF(INFO, g_nlog_level  > 5) << "Sds Source login  from KMDS successfully";
	}
	else
	{
		printf("Login failure:%s\n", KmdsUserApi_GetErrMsg(pclient));
		LOG(WARNING) << "Sds Source login  from KMDS fail" << KmdsUserApi_GetErrMsg(pclient);
		KmdsUserApi_UnInit(pclient);
		KmdsUserApi_Destory(pclient);
		return nret;// KMDSUserApi login  fail��exit��  
	}
	
	//���ûص�
	int failflag = 1;
	//�ȵ�����֤ȯ�����
	for (int i = 0; i < market_spit.size();i++)
	{
		int market = 0;
		if (market_spit[i] == "SH") market = 1;
		else if(market_spit[i] == "SZ") market = 2;
		else market = 0;	
	switch (market)
	{
	case 1:
	   {
		KMDSCodeTableReq(1, g_codetablemap);//�����г�1��֤ȯ�����;
		//�����Ϻ�����������
		KmdsSubscribe("0.SBA2.1.1.*.*");//leve2
		KmdsSubscribe("0.RPT2.1.1.*.*");//Transcation
		 break;
	};
	case 2:
		{
			KMDSCodeTableReq(2, g_codetablemap);//�����г�2��֤ȯ�����;
			//�������ڵ���������
			KmdsSubscribe("0.SBA2.2.1.*.*");//leve2
			KmdsSubscribe("0.RPT2.2.1.*.*");//Transcation
			break;
	};
	default: break;

	}
	} 
	nret=KmdsUserApi_SetKmdsMsgCB(pclient, OnKmdsMsg);//KMDSUserApi login  successfully��set the callback function of 
	LOG_IF(ERROR, nret != 0) << "KMDS callback fail!" ;
	//������Դ������ļ�������
	char  ch;
	while (true)
	{
		//scanf_s("%c", &ch);
		//if (ch == 'e')  { break; }
		getchar();
	}
	return 0;
}
