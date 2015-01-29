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
#define	STOCK_APL_SELL_BID_SIZE  10 //申卖/买数组长度
using namespace ison::base;
using namespace std;
using namespace rapidjson;
extern Socket  *g_socket;//全局静态Socket，用于接受KMDS接口PUSH的数据
extern Context ctx;
std::mutex g_Mutex;
KMDSUSERAPIHANDLE pclient;
extern int g_nlog_level;
extern int transaction_count;
extern int level2_count ;
extern std::vector<std::string> code_filter;//股票过滤
extern bool filter_code_flag;
map<std::string, HQ_CODETABLE>  g_codetablemap;//全局静态变量，用于存储
extern Socket  *g_socket;//全局静态Socket，用于接受KMDS接口PUSH的数据
extern int g_nlog_level;
extern int g_tdf_time_sn;
DateAndTime nowdate = GetDateAndTime();

void  OnKmdsMsg(void* pmsgdesc, void* pmsgopts, int ncmd, MSGDATAHANDLE pmsg)
{
	LOG_IF(INFO, g_nlog_level>5) << "KMDS callback in ";
	g_Mutex.lock();//对互斥量加锁
	if (pmsg == NULL)   { return; }
	switch (ncmd)
	{
	case CKMDS_HQ_CODETABLE://证券代码表 推送更新消息
	{ 
		//更新证券代码表  更新sn
		unsigned int sn;
		SnInit(sn);//初始化sn
		transaction_count = level2_count= sn;
		HQ_CODETABLE_HEAD codetablehead;
		HQ_CODETABLE codetable;
		TABLEDATAHANDLE ptable = NULL;
		MSGDATAHANDLE   prow = NULL;
		CMsgData_GetData_Int32(pmsg, "TOT_RECORD", &codetablehead.TOT_RECORD);//记录条数	
		CMsgData_GetData_Int32(pmsg, "MKT_TP_CODE", &codetablehead.MKT_TP_CODE);//交易市场
		CMsgData_PutData_UInt32(pmsg, "CODETABLE_VER", codetablehead.CODETABLE_VER);
		DateAndTime updatecodetabledate = GetDateAndTime();
		printf("更新代码表-市场:%d, 代码表项数:%d, 代码表日期:%d\n", codetablehead.MKT_TP_CODE, codetablehead.TOT_RECORD, updatecodetabledate.date);
		LOG(INFO) << "更新代码表-市场:" << codetablehead.MKT_TP_CODE << "代码表项数:" << codetablehead.TOT_RECORD;
		CMsgData_GetData_TableData(pmsg, "SEQ_HQ_CODETABLE", &ptable);
		if (ptable)
		{
			int  nrowcount = 0;
			CTableData_GetCount(ptable, (unsigned int *)&nrowcount); //该结果集记录数
			for (int i = 0; i < nrowcount; ++i)
			{
				CTableData_GetRow(ptable, i, &prow);
				if (prow)
				{
					rapidjson::Document document;
					document.SetObject();
					rapidjson::Document::AllocatorType& allocator = document.GetAllocator();//一条一条数据发
					CMsgData_GetData_Int32(prow, "MKT_TP_CODE", &codetable.MKT_TP_CODE);// 交易市场
					CMsgData_GetData_String(prow, "SCR_CODE", (unsigned char *)&codetable.SCR_CODE, sizeof(codetable.SCR_CODE));
					CMsgData_GetData_String(prow, "SCR_NM", (unsigned char *)&codetable.SCR_NM, sizeof(codetable.SCR_NM));
					CMsgData_GetData_String(prow, "STK_TYPE", (unsigned char *)&codetable.STK_TYPE, sizeof(codetable.STK_TYPE));//最高
					CMsgData_GetData_Int64(prow, "YSTD_CLS_QTN_PRICE", &codetable.YSTD_CLS_QTN_PRICE);//最低
					CMsgData_GetData_Int64(prow, "OPN_QTN_PRICE", &codetable.OPN_QTN_PRICE);//最新价格
					CMsgData_GetData_Int64(prow, "RISE_LMT_PRICE", &codetable.RISE_LMT_PRICE);//成交量
					CMsgData_GetData_Int64(prow, "FALL_LMT_PRICE", &codetable.FALL_LMT_PRICE);//成交额
					CMsgData_GetData_Int64(prow, "YSTD_DELTA", &codetable.YSTD_DELTA);
					CMsgData_GetData_Int64(prow, "YSTD_MKT_MAKE_POS_TOT_NUM", &codetable.YSTD_MKT_MAKE_POS_TOT_NUM);
					CMsgData_GetData_Int64(prow, "TDY_SETL_PRICE", &codetable.TDY_SETL_PRICE);
					CMsgData_GetData_Int64(prow, "YSTD_SETL_PRICE", &codetable.YSTD_SETL_PRICE);//成交额
					CMsgData_GetData_UInt32(prow, "CODETABLE_VER", &codetable.CODETABLE_VER);
					CMsgData_GetData_Int64(prow, "ID_NUM_L1", &codetable.ID_NUM_L1);
					CMsgData_GetData_Int64(prow, "ID_NUM_L2", &codetable.ID_NUM_L2);
					g_codetablemap[codetable.SCR_CODE] = codetable;//把代码表存储在map中，更新的可以覆盖
					char send_buffer[bufferlength];
					memset(send_buffer, bufferlength, 0);
					baseline::SDS_Code  sds_code_table= {};
					baseline::MessageHeader hdr;
					hdr.wrap(send_buffer, 0, messageHeaderVersion, bufferlength)
						.blockLength(baseline::SDS_Code::sbeBlockLength())
						.templateId(baseline::SDS_Code::sbeTemplateId())
						.schemaId(baseline::SDS_Code::sbeSchemaId())
						.version(baseline::SDS_Code::sbeSchemaVersion());
					//对data进行encode
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

	case CKMDS_HQ_STK_PART_SNPST:  // 6010 股票行情数据	推送
	{
		level2_count++;
		HQ_PUB_STOCK stockpub;
		TABLEDATAHANDLE ptable = NULL;
		MSGDATAHANDLE   prow = NULL;
		//CMsgData_GetData_Int32(pmsg, "TOT_RECORD", &hqpubhead.TOT_RECORD);//记录条数
		//CMsgData_GetData_Int32(pmsg, "MKT_TP_CODE", &hqpubhead.MKT_TP_CODE);//交易市场
		CMsgData_GetData_TableData(pmsg, "SEQ_HQ_SNAPSHOT", &ptable);   //日间行情推送数据
		if (ptable)
		{
			int  nrowcount = 0;
			CTableData_GetCount(ptable, (unsigned int *)&nrowcount); //该结果集记录数
			for (int i = 0; i < nrowcount; ++i)//每次nrowcount都为1  一条一条推送
			{
				rapidjson::Value object(rapidjson::kObjectType);  //没一条行情记录为一个对象
				CTableData_GetRow(ptable, i, &prow);
				if (prow)
				{
					DateAndTime level2_source_time = GetDateAndTime();
					HQ_CODETABLE stockinfo;
					//实时推送_股票
					//CMsgData_GetData_Int32(prow, "DT", &stockpub.DT);//日期
					stockpub.DT = nowdate.date;
					CMsgData_GetData_Int32(prow, "TM", &stockpub.TM);// 行情时间
					CMsgData_GetData_Int32(prow, "MKT_TP_CODE", &stockpub.MKT_TP_CODE);// 交易市场
					CMsgData_GetData_String(prow, "SCR_CODE", (unsigned char *)&stockpub.SCR_CODE, sizeof(stockpub.SCR_CODE));//证券字段
					CMsgData_GetData_Int32(prow, "HIGH_PRICE", &stockpub.HIGH_PRICE);//最高
					CMsgData_GetData_Int32(prow, "LWS_PRICE", &stockpub.LWS_PRICE);//最低
					CMsgData_GetData_Int32(prow, "LTST_PRICE", &stockpub.LTST_PRICE);//最新价格
					//申买/卖 价与量
					for (int i = 0; i < STOCK_APL_SELL_BID_SIZE; i++)
					{
						
						CMsgData_GetData_Int32(prow, (char*)(concatenate("APL_SELL_PRC_", i + 1).c_str()), &stockpub.APL_SELL_PRC[i]);
						CMsgData_GetData_Int32(prow, (char*)(concatenate("APL_SELL_AMT_", i + 1).c_str()), &stockpub.APL_SELL_AMT[i]);
						CMsgData_GetData_Int32(prow, (char*)(concatenate("APL_BID_PRC_", i + 1).c_str()), &stockpub.APL_BID_PRC[i]);
						CMsgData_GetData_Int32(prow, (char*)(concatenate("APL_BID_AMT_", i + 1).c_str()), &stockpub.APL_BID_AMT[i]);
					}
					CMsgData_GetData_Int64(prow, "BGN_TOT_NUM", &stockpub.BGN_TOT_NUM);//成交量
					CMsgData_GetData_Int64(prow, "BGN_TOT_AMT", &stockpub.BGN_TOT_AMT);//成交额
					CMsgData_GetData_Int32(prow, "BGN_CNT", &stockpub.BGN_CNT);//成交笔数
					CMsgData_GetData_Int64(prow, "ENTRT_BUY_TOT_NUM", &stockpub.ENTRT_BUY_TOT_NUM);//委托买入总量
					CMsgData_GetData_Int64(prow, "ENTRT_SELL_TOT_NUM", &stockpub.ENTRT_SELL_TOT_NUM);//委托卖出总量
					CMsgData_GetData_Int32(prow, "WGHT_AVG_ENTRT_BUY_PRC", &stockpub.WGHT_AVG_ENTRT_BUY_PRC);//加权平均委买价格
					CMsgData_GetData_Int32(prow, "WGHT_AVG_ENTRT_SELL_PRC", &stockpub.WGHT_AVG_ENTRT_SELL_PRC);// 加权平均委卖价格
					LOG_IF(INFO, g_nlog_level > 5) << "SDS get from KMDS transcation :" << "SDS get from KMDS transcation:" << "Code:" << stockpub.SCR_CODE << "." << stockpub.MKT_TP_CODE << "Time:" << stockpub.TM << "Price:" << stockpub.LTST_PRICE << "Volume:" << stockpub.BGN_TOT_NUM;
					LOG_IF(INFO, (level2_count % 10000) == 0) << "SDS get from KMDS transcation * :" << "SDS get from KMDS transcation:" << "Code:" << stockpub.SCR_CODE << "." << stockpub.MKT_TP_CODE << "Time:" << stockpub.TM << "Price:" << stockpub.LTST_PRICE << "Volume:" << stockpub.BGN_TOT_NUM;
					string code = string(stockpub.SCR_CODE);
					//打包	 
					bool codeflag_level2 = 1;//默认都需要
					if (filter_code_flag)//需要过滤
						codeflag_level2 = CodeIsFilter(code.c_str(), code_filter);
					if (codeflag_level2)
					{			
						string maket;

						switch (stockpub.MKT_TP_CODE)
						{
						case 1:///上交所
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
						//先对头进行encode
						hdr.wrap(send_buffer, 0, messageHeaderVersion, bufferlength)
							.blockLength(baseline::SDS_Level2::sbeBlockLength())
							.templateId(baseline::SDS_Level2::sbeTemplateId())
							.schemaId(baseline::SDS_Level2::sbeSchemaId())
							.version(baseline::SDS_Level2::sbeSchemaVersion());
						//对data进行encode
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
	case CKMDS_HQ_PER_STRK_BGN: //日间逐笔成交
	{
		HQ_PUB_SELLINTIME sellintime;
		TABLEDATAHANDLE ptable = NULL;
		MSGDATAHANDLE   prow = NULL;
		//CMsgData_GetData_Int32(pmsg, "TOT_RECORD", &hqpubhead.TOT_RECORD);//记录条数
		//CMsgData_GetData_Int32(pmsg, "MKT_TP_CODE", &hqpubhead.MKT_TP_CODE);//交易市场
		CMsgData_GetData_TableData(pmsg, "SEQ_HQ_STRK_BGN", &ptable);//获取逐笔成交数据
		if (ptable)
		{
			int  nrowcount = 0;
			CTableData_GetCount(ptable, (unsigned int *)&nrowcount); //该结果集记录数
			for (int i = 0; i < nrowcount; ++i)
			{
				transaction_count++;
				CTableData_GetRow(ptable, i, &prow);
				if (prow)
				{
					CMsgData_GetData_Int32(prow, "MKT_TP_CODE", &sellintime.MKT_TP_CODE);// 交易市场
					CMsgData_GetData_String(prow, "SCR_CODE", (unsigned char *)&sellintime.SCR_CODE, sizeof(sellintime.SCR_CODE));
					CMsgData_GetData_Int32(prow, "TM", &sellintime.TM);
					sellintime.DT = nowdate.date;
					CMsgData_GetData_Int64(prow, "BGN_ID", &sellintime.BGN_ID);
					CMsgData_GetData_Int32(prow, "TRDD_PRC", &sellintime.TRDD_PRC);//成交价格
					CMsgData_GetData_Int64(prow, "BGN_QTY", &sellintime.BGN_QTY);
					CMsgData_GetData_Int64(prow, "BGN_AMT", &sellintime.BGN_AMT);//成交金额
					CMsgData_GetData_String(prow, "BGN_FLG", (unsigned char *)&sellintime.BGN_FLG, sizeof(sellintime.BGN_FLG));//成交类别
					CMsgData_GetData_String(prow, "NSR_TXN_TP_CODE", (unsigned char *)&sellintime.NSR_TXN_TP_CODE, sizeof(sellintime.NSR_TXN_TP_CODE));
					LOG_IF(INFO, g_nlog_level > 5) << "SDS get from KMDS transcation:" << "Code:" << sellintime.SCR_CODE<<"."<<sellintime.MKT_TP_CODE<<"Time:"<<sellintime.TM<<"Index:"<<sellintime.BGN_ID<<"Price:"<<sellintime.TRDD_PRC<<"Volume:"<<sellintime.BGN_QTY<<"Turnover:"<<sellintime.BGN_AMT<<"BSFlag"<<sellintime.BGN_FLG<<"NSR_TXN:sellintime.NSR_TXN_TP_CODE";
					LOG_IF(INFO, (level2_count % 10000) == 0) << "SDS get from KMDS transcation * :" << "SDS get from KMDS transcation:" << "Code:" << sellintime.SCR_CODE << "." << sellintime.MKT_TP_CODE << "Time:" << sellintime.TM << "Index:" << sellintime.BGN_ID << "Price:" << sellintime.TRDD_PRC << "Volume:" << sellintime.BGN_QTY << "Turnover:" << sellintime.BGN_AMT << "BSFlag" << sellintime.BGN_FLG << "NSR_TXN:sellintime.NSR_TXN_TP_CODE";
					//打包

					string market;
					string code = (string)sellintime.SCR_CODE;
	
					
					bool codeflag_trans = 1;//默认都需要
					if (filter_code_flag)//需要过滤
						codeflag_trans = CodeIsFilter(code.c_str(), code_filter);
					if (codeflag_trans)
					{
						
						//整合stockcode
						switch (sellintime.MKT_TP_CODE)
						{
						case 1:///上交所
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
						//先对头进行encode
						hdr.wrap(send_buffer, 0, messageHeaderVersion, bufferlength)
							.blockLength(baseline::SDS_Transaction::sbeBlockLength())
							.templateId(baseline::SDS_Transaction::sbeTemplateId())
							.schemaId(baseline::SDS_Transaction::sbeSchemaId())
							.version(baseline::SDS_Transaction::sbeSchemaVersion());
						//对data进行encode
						sds_trans.wrapForEncode(send_buffer, hdr.size(), bufferlength)
							.sDSTime(trans_source_time.time)
							.sn(transaction_count)
							.seqno((int)ISON_MSG_ID::Transaction)////消息编号 84
							.date(sellintime.DT)//自然日
							.time(sellintime.TM)//时间(HHMMSSmmm)
							.index((int)sellintime.BGN_ID)//成交编号
							.price(sellintime.TRDD_PRC)//成交价格
							.volume((int)sellintime.BGN_QTY)//成交数量
							.turnover((int)sellintime.BGN_AMT)//成交金额
							.bSFlag(NULL)//买卖方向
							.orderKind(NULL)//成交类别
							.functionCode(NULL)//成交代码
							.askOrder(NULL)//叫卖方委托序号
							.bidOrder(NULL)//叫买方委托序号
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
		g_Mutex.unlock(); //对互斥量解锁

	}

int  KMDSCodeTableReq(int mktcode, map<std::string, HQ_CODETABLE>& g_codetablemap)
{ 
	LOG_IF(INFO,g_nlog_level >5) << "Market" << mktcode << "'s CodeTable  bengin!";
	rapidjson::Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	MSGDATAHANDLE preqdata = CMsgData_Create();
	MSGDATAHANDLE pans = NULL;
	CMsgData_PutData_Int32(preqdata, "MKT_TP_CODE", mktcode);//请求代码表
	int nret = KmdsUserApi_GetHqData(pclient, CKMDS_HQ_CODETABLE, preqdata, &pans);
	//处理
	if (nret != 0)    //0表示成功,如果失败
	{
		printf("KMDS出获得数据失败：%d,%s", KmdsUserApi_GetErrCode(pclient), KmdsUserApi_GetErrMsg(pclient));
	}
	else{
		if (pans)
		{
			//...
			//业务处理代码
			//获取结果集, DATA0, [DATA1,DATA2,...]
			TABLEDATAHANDLE  ptable = NULL;
			MSGDATAHANDLE    prow = NULL;
			int              nrowcount = 0;
			int				 i = 0;
			CMsgData_GetData_TableData(pans, "DATA0", &ptable);
			if (ptable)
			{
				DateAndTime codetabledate = GetDateAndTime();
				CTableData_GetCount(ptable, (unsigned int *)&nrowcount); //该结果集记录数
				printf("市场:%d, 代码表项数:%d, 代码表日期:%d\n", mktcode, nrowcount, codetabledate.date);
				for (i = 0; i < nrowcount; ++i)
				{
					CTableData_GetRow(ptable, i, &prow);
					if (prow)
					{
						HQ_CODETABLE codetable;
						//获得证券代码表
						CMsgData_GetData_Int32(prow, "MKT_TP_CODE", &codetable.MKT_TP_CODE);// 交易市场
						CMsgData_GetData_String(prow, "SCR_CODE", (unsigned char *)&codetable.SCR_CODE, sizeof(codetable.SCR_CODE));
						CMsgData_GetData_String(prow, "SCR_NM", (unsigned char *)&codetable.SCR_NM, sizeof(codetable.SCR_NM));
						CMsgData_GetData_Int32(prow, "STK_TYPE", &codetable.STK_TYPE);//最高
						CMsgData_GetData_Int64(prow, "YSTD_CLS_QTN_PRICE", &codetable.YSTD_CLS_QTN_PRICE);//最低
						CMsgData_GetData_Int64(prow, "OPN_QTN_PRICE", &codetable.OPN_QTN_PRICE);//最新价格
						CMsgData_GetData_Int64(prow, "RISE_LMT_PRICE", &codetable.RISE_LMT_PRICE);//成交量
						CMsgData_GetData_Int64(prow, "FALL_LMT_PRICE", &codetable.FALL_LMT_PRICE);//成交额
						CMsgData_GetData_Int64(prow, "YSTD_DELTA", &codetable.YSTD_DELTA);
						CMsgData_GetData_Int64(prow, "YSTD_MKT_MAKE_POS_TOT_NUM", &codetable.YSTD_MKT_MAKE_POS_TOT_NUM);
						CMsgData_GetData_Int64(prow, "TDY_SETL_PRICE", &codetable.TDY_SETL_PRICE);
						CMsgData_GetData_Int64(prow, "YSTD_SETL_PRICE", &codetable.YSTD_SETL_PRICE);//成交额
						CMsgData_GetData_UInt32(prow, "CODETABLE_VER", &codetable.CODETABLE_VER);
						CMsgData_GetData_Int64(prow, "ID_NUM_L1", &codetable.ID_NUM_L1);
						CMsgData_GetData_Int64(prow, "ID_NUM_L2", &codetable.ID_NUM_L2);
						g_codetablemap[codetable.SCR_CODE] = codetable;//
						//打包成json格式
						char send_buffer[bufferlength];
						memset(send_buffer, bufferlength, 0);
						baseline::SDS_Code  sds_code_table = {};
						baseline::MessageHeader hdr;
						hdr.wrap(send_buffer, 0, messageHeaderVersion, bufferlength)
							.blockLength(baseline::SDS_Code::sbeBlockLength())
							.templateId(baseline::SDS_Code::sbeTemplateId())
							.schemaId(baseline::SDS_Code::sbeSchemaId())
							.version(baseline::SDS_Code::sbeSchemaVersion());
						//对data进行encode
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
			{//空数据
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
	//读取配置文件
	const char* user_name = "test";
	const char* password= "888888";
	string submarket = "SH;SZ";
	std::vector<std::string> configkey = {"UserName", "Password","SubMarket"};
	map<std::string, std::string> configmap;//存放配置文件中的数据
	int ret=ReadConfig("./sourcekmds.json", configkey, configmap);//读取配置文件
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
		return nret;// KMDSUserApi login  fail，exit！  
	}
	
	//设置回调
	int failflag = 1;
	//先得请求证券代码表
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
		KMDSCodeTableReq(1, g_codetablemap);//请求市场1的证券代码表;
		//订阅上海的所有数据
		KmdsSubscribe("0.SBA2.1.1.*.*");//leve2
		KmdsSubscribe("0.RPT2.1.1.*.*");//Transcation
		 break;
	};
	case 2:
		{
			KMDSCodeTableReq(2, g_codetablemap);//请求市场2的证券代码表;
			//订阅深圳的所有数据
			KmdsSubscribe("0.SBA2.2.1.*.*");//leve2
			KmdsSubscribe("0.RPT2.2.1.*.*");//Transcation
			break;
	};
	default: break;

	}
	} 
	nret=KmdsUserApi_SetKmdsMsgCB(pclient, OnKmdsMsg);//KMDSUserApi login  successfully，set the callback function of 
	LOG_IF(ERROR, nret != 0) << "KMDS callback fail!" ;
	//这个可以从配置文件中设置
	char  ch;
	while (true)
	{
		//scanf_s("%c", &ch);
		//if (ch == 'e')  { break; }
		getchar();
	}
	return 0;
}
