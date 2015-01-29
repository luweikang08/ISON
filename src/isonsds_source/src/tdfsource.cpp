#include "TDFAPI.h"
#include "TDFAPIStruct.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <wchar.h>
#include <iconv.h>
#include <map>
#include<fstream>
#include <iostream>
#include <string>
#include<vector>

#include "document.h"
#include "prettywriter.h"
#include "filestream.h"
#include "stringbuffer.h"
#include "writer.h"
#include "ibsdstool.h"

#include "isonbase.h"
#include "context.h"
#include "event.h"
#include "ibprotocol.h"
#include "socket.h"
#include"message.h"
#include"iblog.h"
#include"isonsdsdataapi.h"
#include "filehelper.h"
#include"jsonhelper.h"
#include"timehelper.h"
#include"sourcepublic.h"
#include "sbeinterface.h"


#ifdef _WIN32
#define strcpy strcpy_s
#endif

using namespace ison::base;
extern Socket  *g_socket;//全局静态Socket，用于接受KMDS接口PUSH的数据
extern int g_nlog_level;
extern int g_tdf_time_sn;
map<std::string, TDF_CODE>  g_mcodetable_pdf;
extern std::vector<std::string> code_filter;//股票过滤
extern bool filter_code_flag ;

//extern int  ReadConfig(const char* filename, std::vector<std::string> configkey, map<std::string, std::string> &configmap);
extern int transaction_count ;
extern int level2_count ;
extern int oder_count ;
extern int oder_queue_count ;

#define MIN(x, y) ((x)>(y)?(y):(x))

void RecvData(THANDLE hTdf, TDF_MSG* pMsgHead);

void RecvSys(THANDLE hTdf, TDF_MSG* pSysMsg);
void GetDataFromTDF();

#define ELEM_COUNT(arr) (sizeof(arr)/sizeof(arr[0]))
#define SAFE_STR(str) ((str)?(str):"")
#define SAFE_CHAR(ch) ((ch) ? (ch) : ' ')
 int code_convert(char *from_charset, char *to_charset, char *inbuf, size_t* inlen, char *outbuf, size_t* outlen)
{
	iconv_t cd;
	//int rc;
	char **pin = &inbuf;
	char **pout = &outbuf;
	cd = iconv_open(to_charset, from_charset);
	if (cd == 0)
		return -1;
	memset(outbuf, 0, *outlen);
	if (iconv(cd, pin, (size_t*)inlen, pout, (size_t*)outlen) == -1)
		return -1;
	iconv_close(cd);
	return 0;
}


// const char * data_type[] = { itostring(Level2).c_str(), stranscation.c_str(), soder_queue.c_str(), soder.c_str(), scode_table.c_str() };
#define GETRECORD(pBase, TYPE, nIndex) ((TYPE*)((char*)(pBase) + sizeof(TYPE)*(nIndex)))


void RecvData(THANDLE hTdf, TDF_MSG* pMsgHead)
{   
	LOG_IF(INFO, g_nlog_level>5) << "TDF callback in ";
	if (!pMsgHead->pData)
	{
		assert(0);
		return;
	}

	unsigned int nItemCount = pMsgHead->pAppHead->nItemCount;
	unsigned int nItemSize = pMsgHead->pAppHead->nItemSize;
	LOG_IF(INFO, g_nlog_level >6) << "nItemCount:" << nItemCount << "ItemSize:" << nItemSize;
	if (!nItemCount)
	{
		assert(0);
		return;
	}

	switch (pMsgHead->nDataType)
	{
	case MSG_DATA_MARKET:
	{   
		//assert(nItemSize == sizeof(TDF_MARKET_DATA));
		//DumpScreenMarket((TDF_MARKET_DATA*)pMsgHead->pData, nItemCount);
		//TDF_MARKET_DATA* pLastMarket = GETRECORD(pMsgHead->pData, TDF_MARKET_DATA, nItemCount - 1);
		TDF_MARKET_DATA *pData = (TDF_MARKET_DATA *)(pMsgHead->pData);
		for (unsigned int i = 0; i < nItemCount; i++)
		{
			const TDF_MARKET_DATA & pLastMarket = pData[i];
			std::string code = pLastMarket.szCode;
			bool codeflag_trans = 1;//默认都需要
			if (filter_code_flag)//需要过滤
				codeflag_trans = CodeIsFilter(code.c_str(), code_filter);
			if (codeflag_trans)
			{
				level2_count++;
				DateAndTime level2_source_time = GetDateAndTime();

				char send_buffer[bufferlength];
				memset(send_buffer, bufferlength, 0);
				baseline::SDS_Level2 sds_level2 = {};
				baseline::MessageHeader hdr;
				//先对头进行encode
				hdr.wrap(send_buffer, 0, messageHeaderVersion, bufferlength)
					.blockLength(baseline::SDS_Level2::sbeBlockLength())
					.templateId(baseline::SDS_Level2::sbeTemplateId())
					.schemaId(baseline::SDS_Level2::sbeSchemaId())
					.version(baseline::SDS_Level2::sbeSchemaVersion());
				//对data进行encode
				sds_level2.wrapForEncode(send_buffer, hdr.size(), bufferlength);
				for (int k = 0, size = 10; k < size; k++)
				{
					sds_level2.askPrice(k, pLastMarket.nAskPrice[k]);
					sds_level2.askVol(k, pLastMarket.nAskVol[k]);
					sds_level2.bidPrice(k, pLastMarket.nBidPrice[k]);
					sds_level2.bidVol(k, pLastMarket.nBidVol[k]);
				}

				sds_level2.seqno((int)ISON_MSG_ID::Level2)
					.sDSTime(level2_source_time.time)
					.sn(level2_count)
					.date(pLastMarket.nActionDay)
					.time(pLastMarket.nTime)
					.preClose(pLastMarket.nPreClose)
					.open(pLastMarket.nOpen)
					.high(pLastMarket.nHigh)
					.low(pLastMarket.nLow)
					.price(pLastMarket.nMatch)
					.volume(pLastMarket.iVolume)
					.turnover(pLastMarket.iTurnover)
					.totalBidVol(pLastMarket.nTotalBidVol)
					.totalAskVol(pLastMarket.nTotalAskVol)
					.numTrades(pLastMarket.nNumTrades)
					.avgBidPrice(pLastMarket.nWeightedAvgBidPrice)
					.avgAskPrice(pLastMarket.nWeightedAvgAskPrice)
					.lowLimited(pLastMarket.nLowLimited)
					.highLimited(pLastMarket.nHighLimited)
					.putCode(pLastMarket.szWindCode);
				int len = hdr.size() + sds_level2.size();
				string send_message(send_buffer, len);
				MessageSend(itostring(Level2).c_str(), send_message.c_str(), len, g_socket);
			}
		}
		
	}
		break;
	
	case MSG_DATA_TRANSACTION://逐笔成交
	{  
		TDF_TRANSACTION *pData = (TDF_TRANSACTION *)(pMsgHead->pData);
		for (unsigned int i = 0; i < nItemCount; i++)
		{  
			transaction_count++;
			const TDF_TRANSACTION & pLastTransaction = pData[i];
			std::string code = pLastTransaction.szCode;
			bool codeflag_trans = 1;//默认都需要
			if (filter_code_flag)//需要过滤
			codeflag_trans = CodeIsFilter(code.c_str(),code_filter);
			if (codeflag_trans)
			{
				    DateAndTime trans_source_time = GetDateAndTime();
					char send_buffer[bufferlength];
					memset(send_buffer, bufferlength, 0);
					baseline::SDS_Transaction sds_trans = {};
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
						.date(pLastTransaction.nActionDay)//自然日
						.time(pLastTransaction.nTime)
						.index(pLastTransaction.nIndex)//成交编号
						.price(pLastTransaction.nPrice)//成交价格
						.volume(pLastTransaction.nVolume)//成交数量
						.turnover(pLastTransaction.nTurnover)//成交金额
						.bSFlag(pLastTransaction.nBSFlag)//买卖方向
						.orderKind(pLastTransaction.chOrderKind)//成交类别
						.functionCode(pLastTransaction.chFunctionCode)//成交代码
						.askOrder(pLastTransaction.nAskOrder)//叫卖方委托序号
						.bidOrder(pLastTransaction.nBidOrder)//叫买方委托序号
						.putCode(pLastTransaction.szWindCode);
						int len = hdr.size() + sds_trans.size();
					    string send_message(send_buffer, len);
					    MessageSend(itostring(Transaction).c_str(), send_message.c_str(), len, g_socket);
			}
		}
	}
		break;
	case MSG_DATA_ORDERQUEUE://委托队列
	{   
		TDF_ORDER_QUEUE *pData = (TDF_ORDER_QUEUE *)(pMsgHead->pData);
		for (unsigned int i = 0; i < nItemCount; i++)
		{
			oder_count++;
			const TDF_ORDER_QUEUE & pLastOrderQueue = pData[i];
			std::string code = pLastOrderQueue.szCode;
			bool codeflag_queue = 1;//默认都需要
			if (filter_code_flag)//需要过滤
				codeflag_queue = CodeIsFilter(code.c_str(),code_filter);
			if (codeflag_queue)
			{
				DateAndTime queue_source_time = GetDateAndTime();
				char send_buffer[bufferlength];
				memset(send_buffer, bufferlength, 0);
				baseline::SDS_OderQueen sds_queue = {};
				baseline::MessageHeader hdr;
				int items= pLastOrderQueue.nABItems;
				//先对头进行encode
				hdr.wrap(send_buffer, 0, messageHeaderVersion, bufferlength)
					.blockLength(baseline::SDS_OderQueen::sbeBlockLength())
					.templateId(baseline::SDS_OderQueen::sbeTemplateId())
					.schemaId(baseline::SDS_OderQueen::sbeSchemaId())
					.version(baseline::SDS_OderQueen::sbeSchemaVersion());
				//对data进行encode
				sds_queue.wrapForEncode(send_buffer, hdr.size(), bufferlength)
					.seqno((int)ISON_MSG_ID::OrderQueue)
					.sDSTime(queue_source_time.time)
					.sn(oder_count)
					.date(pLastOrderQueue.nActionDay)
					.time(pLastOrderQueue.nTime)
					.side(pLastOrderQueue.nSide)
					.price(pLastOrderQueue.nPrice)
					.orders(pLastOrderQueue.nOrders)
					.aBItems(items)
					.putCode(pLastOrderQueue.szWindCode);
				for (int k = 0; k < items; k++)
					sds_queue.aBVolume(i, pLastOrderQueue.nABVolume[i]);
				int len = hdr.size() + sds_queue.size();
				string send_message(send_buffer, len);
				MessageSend(itostring(OrderQueue).c_str(), send_message.c_str(), len, g_socket);
			}
		}

	};
		break;
		/*
	case MSG_DATA_ORDER://逐笔委托
	{
		TDF_ORDER *pData = (TDF_ORDER *)(pMsgHead->pData);
		for (unsigned int i = 0; i < nItemCount; i++)
		{
			rapidjson::Document document;
			const TDF_ORDER & plastOrder = pData[i];
			std::string code = plastOrder.szCode;
			bool codeflag_oder = 1;//默认都需要
			if (filter_code_flag)//需要过滤
				codeflag_oder = CodeIsFilter(code.c_str(),code_filter);
			if (codeflag_oder)
			{
				DateAndTime queue_source_time = GetDateAndTime();
				char send_buffer[bufferlength];
				memset(send_buffer, bufferlength, 0);
				baseline::SDS sds_queue = {};
				baseline::MessageHeader hdr;
				//先对头进行encode
				hdr.wrap(send_buffer, 0, messageHeaderVersion, bufferlength)
					.blockLength(baseline::SDS_OderQueen::sbeBlockLength())
					.templateId(baseline::SDS_OderQueen::sbeTemplateId())
					.schemaId(baseline::SDS_OderQueen::sbeSchemaId())
					.version(baseline::SDS_OderQueen::sbeSchemaVersion());
				//对data进行encode
				sds_queue.wrapForEncode(send_buffer, hdr.size(), bufferlength)
				document.AddMember("Seqno", (int)ISON_MSG_ID::Order, allocator);//消息编号 85
				rapidjson::Value swindcrcode;
				swindcrcode.SetString(plast_order.szWindCode, strlen(plast_order.szWindCode), allocator);
				document.AddMember("Code", swindcrcode, allocator);
				document.AddMember("Date", plast_order.nActionDay, allocator); //自然日
				document.AddMember("Time", plast_order.nTime, allocator);//时间(HHMMSSmmm)
				document.AddMember("Order", plast_order.nOrder, allocator);//委托号
				document.AddMember("Price", plast_order.nPrice, allocator);//委托价格
				document.AddMember("Volume", plast_order.nVolume, allocator);//委托数量
				document.AddMember("OrderKind", plast_order.chOrderKind, allocator);//委托类别
				document.AddMember("FunctionCode", plast_order.chFunctionCode, allocator);//委托代码
				std::string source_order = Document2String(document);	//Ttansfer to string 
				int res = MessageSend(itostring(Order).c_str(), source_order.c_str(), g_socket);		//Send to Socket
				LOG_IF(INFO, g_nlog_level>5) << "TDF send " << source_order.c_str() << "to Scoket";
				LOG_IF(INFO, (level2_count % 10000) == 0) << "TDF send Level2 [coun:"
					<< level2_count << "]" << source_order.c_str() << "\n";
			}
		}
	}
		break;*/
	default:
		break;
	}
		
}

void RecvSys(THANDLE hTdf, TDF_MSG* pSysMsg)
{ 
	LOG_IF(INFO, g_nlog_level>5) << "RecvSys in ";
	LOG_IF(INFO, g_nlog_level>5) << pSysMsg->nDataType;
	if (!pSysMsg || !hTdf)
	{
		return;
	}

	switch (pSysMsg->nDataType)
	{
	case MSG_SYS_DISCONNECT_NETWORK:
	{
		printf("网络断开\n");
	}
		break;
	case MSG_SYS_CONNECT_RESULT:
	{
		TDF_CONNECT_RESULT* pConnResult = (TDF_CONNECT_RESULT*)pSysMsg->pData;
		if (pConnResult && pConnResult->nConnResult)
		{
			printf("连接 %s:%s user:%s, password:%s 成功!\n", pConnResult->szIp, pConnResult->szPort, pConnResult->szUser, pConnResult->szPwd);
		}
		else
		{
			printf("连接 %s:%s user:%s, password:%s 失败!\n", pConnResult->szIp, pConnResult->szPort, pConnResult->szUser, pConnResult->szPwd);
		}
	}
		break;
	case MSG_SYS_LOGIN_RESULT:
	{
		TDF_LOGIN_RESULT* pLoginResult = (TDF_LOGIN_RESULT*)pSysMsg->pData;

		//convert gb2312 to utf-8
		char utf_info[128];
		size_t len2 = 128;
		size_t len1 = strlen(pLoginResult->szInfo);
		code_convert("gb2312", "utf-8", pLoginResult->szInfo, &len1, utf_info, &len2);

		if (pLoginResult && pLoginResult->nLoginResult)
		{
			printf("登陆成功！info:%s, nMarkets:%d\n", utf_info, pLoginResult->nMarkets);
			for (int i = 0; i<pLoginResult->nMarkets; i++)
			{
				printf("market:%s, dyn_date:%d\n", pLoginResult->szMarket[i], pLoginResult->nDynDate[i]);
			}
		}
		else
		{
			printf("登陆失败，原因：%s\n", utf_info);
		}
	}
		break;
	case MSG_SYS_CODETABLE_RESULT://获取证券代码表
	{
		TDF_CODE_RESULT* pCodeResult = (TDF_CODE_RESULT*)pSysMsg->pData;
		if (pCodeResult)
		{
			printf("接收到代码表：info:%s, 市场个数:%d\n", pCodeResult->szInfo, pCodeResult->nMarkets);
			for (int i = 0; i<pCodeResult->nMarkets; i++)
			{
				printf("市场:%s, 代码表项数:%d, 代码表日期:%d\n", pCodeResult->szMarket[i], pCodeResult->nCodeCount[i], pCodeResult->nCodeDate[i]);
				//获取代码表
				TDF_CODE* pCodeTable;
				unsigned int nItems;
				TDF_GetCodeTable(hTdf, pCodeResult->szMarket[i], &pCodeTable, &nItems);//请求证券代码表
				for (unsigned int i = 0; i<nItems; i++)
				{   //Pacage the CodeTable 
					TDF_CODE& code = pCodeTable[i];
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
						.putCode(code.szWindCode)
						.putENName(code.szENName)
						.putCNName(code.szENName)
						.type(code.nType);
					int len = hdr.size() + sds_code_table.size();
					string send_message(send_buffer, len);
					MessageSend(itostring(SYSMSG).c_str(), send_message.c_str(), len, g_socket);
				}
				TDF_FreeArr(pCodeTable);
			}
		}
	}
		break;
	case MSG_SYS_QUOTATIONDATE_CHANGE:
	{
		TDF_QUOTATIONDATE_CHANGE* pChange = (TDF_QUOTATIONDATE_CHANGE*)pSysMsg->pData;
		if (pChange)
		{
			printf("收到行情日期变更通知，即将自动重连！交易所：%s, 原日期:%d, 新日期：%d\n", pChange->szMarket, pChange->nOldDate, pChange->nNewDate);
			LOG_IF(INFO, g_nlog_level >6) << "收到行情日期变更通知，即将自动重连！交易所" << pChange->szMarket << "原日期" << pChange->nOldDate << "新日期"<<pChange->nNewDate;
		}
	}
		break;
	case MSG_SYS_MARKET_CLOSE:
	{
		TDF_MARKET_CLOSE* pCloseInfo = (TDF_MARKET_CLOSE*)pSysMsg->pData;
		if (pCloseInfo)
		{ 
			LOG_IF(INFO, g_nlog_level >6) << "闭市消息:" << "market" << pCloseInfo->szMarket << "time" << pCloseInfo->nTime << "info" << pCloseInfo->chInfo;
			printf("闭市消息:market:%s, time:%d, info:%s\n", pCloseInfo->szMarket, pCloseInfo->nTime, pCloseInfo->chInfo);
			/*rapidjson::Document document;
			document.SetObject();
			rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
			document.AddMember("Seqno", (int)ISON_MSG_ID::SYSCloseMarket, allocator);
			document.AddMember("Market", pCloseInfo->szMarket,allocator);
			document.AddMember("Time", pCloseInfo->nTime, allocator);
			document.AddMember("Info", pCloseInfo->chInfo, allocator);
			std::string message= Document2String(document);
			MessageSend(itostring(SYSMSG).c_str(), message.c_str(), g_socket);
			LOG_IF(INFO, g_nlog_level > 4) << message.c_str();*/
		}
	}
		break;
	case MSG_SYS_HEART_BEAT:
	{
		printf("收到心跳消息\n");

	}
		break;
	default:
		//assert(0);
		break;
	}
}
const char* GetErrStr(TDF_ERR nErr)
{
	std::map<TDF_ERR, const char*> mapErrStr;
	mapErrStr.insert(std::make_pair(TDF_ERR_UNKOWN, "TDF_ERR_UNKOWN"));
	mapErrStr.insert(std::make_pair(TDF_ERR_INITIALIZE_FAILURE, "TDF_ERR_INITIALIZE_FAILURE"));
	mapErrStr.insert(std::make_pair(TDF_ERR_NETWORK_ERROR, "TDF_ERR_NETWORK_ERROR"));
	mapErrStr.insert(std::make_pair(TDF_ERR_INVALID_PARAMS, "TDF_ERR_INVALID_PARAMS"));
	mapErrStr.insert(std::make_pair(TDF_ERR_VERIFY_FAILURE, "TDF_ERR_VERIFY_FAILURE"));
	mapErrStr.insert(std::make_pair(TDF_ERR_NO_AUTHORIZED_MARKET, "TDF_ERR_NO_AUTHORIZED_MARKET"));
	mapErrStr.insert(std::make_pair(TDF_ERR_NO_CODE_TABLE, "TDF_ERR_NO_CODE_TABLE"));
	mapErrStr.insert(std::make_pair(TDF_ERR_SUCCESS, "TDF_ERR_SUCCESS"));
	if (mapErrStr.find(nErr) == mapErrStr.end())
	{
		return "TDF_ERR_UNKOWN";
	}
	else
	{
		return mapErrStr[nErr];
	}
}
void GetDataFromTDF(){
	std::vector<std::string> configkey = { "ServerIp", "ServerPort", "UserId", "Password", "ReconnectCount"
		, "ReconnectGap", "SubMarket", "SubScriptions", "RequestDate", "RequestTime" };
	map<std::string, std::string> configmap;//存放配置文件中的数据
	ReadConfig("./sourcetdf.json", configkey, configmap);//读取配置文件
	TDF_OPEN_SETTING settings = { 0 };
	strcpy(settings.szIp, configmap[configkey[0]].c_str());
	strcpy(settings.szPort, configmap[configkey[1]].c_str());
	strcpy(settings.szUser, configmap[configkey[2]].c_str());
	strcpy(settings.szPwd, configmap[configkey[3]].c_str());
	settings.nReconnectCount = atoi(configmap[configkey[4]].c_str());//连接断开时重连次数
	settings.nReconnectGap = atoi(configmap[configkey[5]].c_str()); //重连间隔
	settings.pfnMsgHandler = RecvData; //设置数据消息回调函数
	settings.pfnSysMsgNotify = RecvSys;//设置系统消息回调函数
	settings.nProtocol = 0;
	settings.szMarkets = configmap[configkey[6]].c_str(); //需要订阅的市场列表
	settings.szSubScriptions = configmap[configkey[7]].c_str(); //需要订阅的股票,为空则订阅全市场//异步函数
	settings.nDate = atoi(configmap[configkey[8]].c_str());//请求的日期，格式YYMMDD，为0则请求今墍
	settings.nTime = atoi(configmap[configkey[9]].c_str());// 请求的时间，格式HHMMSS，为0则请求实时行情，䶿xffffffff从头请求
	settings.nTypeFlags = DATA_TYPE_ALL; //请求的品种。DATA_TYPE_ALL请求所有品祍
	//设置环境变量
	TDF_ERR nErr = TDF_ERR_SUCCESS;//错误代码
	THANDLE hTDF = NULL;//TDF句柄
	hTDF = TDF_Open(&settings, &nErr);//打开链接
	if (hTDF == NULL)
		printf("TDF_Open return error: %s\n", GetErrStr(nErr));
	char  ch;
	while (true)
	{
		//scanf_s("%c", &ch);
		//if (ch == 'e')  { break; }
		getchar();
	}
	TDF_Close(hTDF);//关闭连接
}

