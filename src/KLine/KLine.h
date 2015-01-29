#ifndef __ISON_SDS_K_LINE_H__
#define __ISON_SDS_K_LINE_H__

#ifndef SBE_FORMAT
#define SBE_FORMAT
#else
#define JSON_FORMAT
#endif

#include <iostream>
#include "isonbase.h"
#include "iblog.h"
#include "context.h"
#include "stage.h"
#include "actor.h"
#include "event.h"
#include "ibprotocol.h"

#include <fstream>
#include <time.h>
#include "windows.h"

#include "timehelper.h"
#include "readconfig.h"
#include "stringhelper.h"
#include "KLineBase.h"

#include "isonsdsdataapi.h"
//#include "data_struct.h"

const char*  SUBID = "sub1";
const char*  PUBID = "pub1";
const string SUBTOPIC_SZ = itostring(ISON_SDS2TGW::TSZ_TRANSACTION);
const string SUBTOPIC_SH = itostring(ISON_SDS2TGW::TSH_TRANSACTION);

const char* g_clog_dir = "KLINELog";
const char* JSON_FILE_DEFAULT = "config.json";

const vector<string> VECTOR_CONFIGKEY = { "version", "releasedate", "loglevel", "subendpoint_kline", "pubendpoint_kline" };
const vector<string> VECTOR_CONFIGVALUE_DEFAULT = { "V0.00.03", "2015/01/28", "1", "tcp://192.168.15.200:2010", "tcp://192.168.15.200:2031" };

#endif