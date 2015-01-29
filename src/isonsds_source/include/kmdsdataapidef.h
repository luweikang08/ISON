#ifndef ISON_SDS_KMDS_DATA_API_H_
#define ISON_SDS_KMDS_DATA_API_H_
#include <sstream>
#define STOCK_APL_SELL_BID_SIZE 10
struct HQ_CODETABLE_HEAD //实时推送_头
{
	int           TOT_RECORD;//记录条数	
	int		      MKT_TP_CODE;//交易市场	
	unsigned int  CODETABLE_VER;//市场版本号
};
struct HQ_PUB_HEAD//实时推送_头
{
	int     TOT_RECORD;//记录条数	
	int		MKT_TP_CODE;//交易市场	
};
struct HQ_CODETABLE
{
	int MKT_TP_CODE;//	市场类型代码
	char SCR_CODE[10];//	证券代码
	char SCR_NM[128];//证券名称	String
	int STK_TYPE;	//证券类别	INT32
	long long YSTD_CLS_QTN_PRICE;// 昨收盘价	
	long long OPN_QTN_PRICE;	//开盘价
	long long RISE_LMT_PRICE;	//涨停价
	long long FALL_LMT_PRICE;	//跌停价
	long long YSTD_DELTA;	//昨虚实度
	long long YSTD_MKT_MAKE_POS_TOT_NUM;	//昨持仓
	long long TDY_SETL_PRICE;//	今日结算
	long long YSTD_SETL_PRICE;// 昨结算
	unsigned int CODETABLE_VER;//	版本号
	long long ID_NUM_L1;// 普通行情本日编号
	long long ID_NUM_L2;// L2行情本日编号
	

};

struct HQ_PUB_INDEX//实时推送_指数
{
	int      TM;  //行情时间		HHMMSSmmm
	int      MKT_TP_CODE;//交易市场	
	char     SCR_CODE[10]; //证券字段名
	long long  SEQ_ID;	  //快照序号	
	int	     HIGH_PRICE; // 最高价	
	int	     LWS_PRICE;  //  最低价	
	int	     LTST_PRICE;//  最新价	
	long long	 BGN_TOT_NUM;// 成交总量	INT64
	long long	 BGN_TOT_AMT;//  成交总金额
	long long	 PUBLISH_TM1;//  一级发布时间	
	long long  PUBLISH_TM2;// 二级发布时间	
	long long	 ERROR_MARK;//  错误字段域	
};
struct HQ_PUB_SELLINTIME//实时推送_逐笔成交
{
	int     MKT_TP_CODE; // 交易市场	INT32
	char    SCR_CODE[10];	 // 证券字段名	String
	int 	TM;  // 成交时间	INT32
	int     DT;//成交日期 
	long long	BGN_ID;//	  成交编号	INT64
	int	     TRDD_PRC;	//  成交价格	INT32
	long long BGN_QTY;	  //成交数量	INT64
	long long BGN_AMT; //成交金额	INT64
	char	BGN_FLG[56];	 // 成交类别	String
	char	NSR_TXN_TP_CODE[64];//  指令交易类型	String
};
struct HQ_PUB_STOCK
{
	int TM;//行情时间	
	int DT;
	int MKT_TP_CODE;//  交易市
	char SCR_CODE[10];// 证券字段名	
	long long SEQ_ID ; // 快照序号	
	int PART_SNPST_ST_TP_CODE;//分笔快照状态类型代码	
	int HIGH_PRICE;// 最高价	
	int LWS_PRICE;// 最低价	
	int LTST_PRICE;// 最新价

	int APL_SELL_PRC [10];//  申卖价	
	int APL_SELL_AMT [10];//申卖量
	int APL_BID_PRC [10];//  申买价	
	int APL_BID_AMT [10];//申买量

	int BGN_CNT;//  成交笔数
	long long BGN_TOT_NUM;//  成交总量	
	long long BGN_TOT_AMT; // 成交总金额	
	long long ENTRT_BUY_TOT_NUM; //	  委托买入总量	
	long long ENTRT_SELL_TOT_NUM;//	  委托卖出总量	
	int WGHT_AVG_ENTRT_BUY_PRC; // 加权平均委买价格	
	int WGHT_AVG_ENTRT_SELL_PRC; //加权平均委卖价格	
//	int IOPV_NET_VAL_VALT;//  IOPV净值估值	
	//int MTUR_YLD;// 到期收益率	
	//int PE_RATIO_1;//  市盈率1	
	//int PE_RATIO_2;// 市盈率2	
	//long long PUBLISH_TM1;//  一级发布时间	
	//long long PUBLISH_TM2;// 二级发布时间	
	//char PRE_FIX[64];//	  证券信息前缀	
	//int ERROR_MARK; // 错误字段域		
	

};
#endif