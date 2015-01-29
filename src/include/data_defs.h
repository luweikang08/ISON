#ifndef __ISON_DATA_DEFS_H__
#define __ISON_DATA_DEFS_H__

#define MESSAGEHEADERVERSION 0     //baseline::MessageHeader version
#define BUFFELENGTH          512   //buffer length of store e.message()

enum ISON_TRADEPUBTOPIC{
	TSZ_LARGETRANMONITOR = 2030,    //SZ,large buy&sell
	TSZ_KLINEONEMINUTE = 2031,      //SZ,Kline,one minute

	TSH_LARGETRANMONITOR = 2130,	//SH,large buy&sell
	TSH_KLINEONEMINUTE = 2131,      //SH,Kline,one minute
};

#endif