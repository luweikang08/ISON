#ifndef __ISON_DATA_DEFS_H__
#define __ISON_DATA_DEFS_H__

#define MESSAGEHEADERVERSION 0     //baseline::MessageHeader version
#define BUFFELENGTH          512   //buffer length of store e.message()

#define BUY_SIGNALID          1
#define SELL_SIGNALID         2
#define AUCTION_SIGNAID       3
#define RISE_SIGNALID         4
#define FALL_SIGNALID         5
#define HOLDHIGH_SIGNALID     6
#define OPENHIGH_SIGNALID     7
#define HOLDLOW_SIGNALID      8
#define OPENLOW_SIGNALID      9

#define MACD_SIGNALID     1
#define KDJ_SIGNALID      2
#define RSI_SIGNALID      3
#define BOLL_SIGNALID     4

enum ISON_TRADEPUBTOPIC{
	TSZ_SHORTELF = 2030,    //SZ,shortelf
	TSZ_KLINEONEMINUTE = 2031,      //SZ,Kline,one minute
	TSZ_POINDICATOR = 2032,      //SZ,POIndicator,one minute

	TSH_SHORTELF = 2130,	//SH,shortelf
	TSH_KLINEONEMINUTE = 2131,      //SH,Kline,one minute
	TSH_POINDICATOR = 2132,      //SH,POIndicator,one minute
};

#endif