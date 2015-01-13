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

#include "math.h"
#include <fstream>
#include <time.h>
#include "windows.h"

#include "jsonhelper.h"
#include "timehelper.h"
#include "klinehelper.h"
#include "readconfig.h"
#include "stringhelper.h"
#include "SDS_Level2.hpp"
#include "MessageHeader.hpp"

#define g_num_DataStoreCntMax          30
#define g_num_KlineOneMinuteCntMax     242 //9:30:00.000-11:29:59.999  13:00:00.000-14:59:59.999,
#define g_num_KlineFiveMinuteCntMax    48
#define g_num_KlineTenMinuteCntMax     24
#define g_num_KlineFifteenMinutCntMax  16
#define g_num_KlineThirtyMinutCntMax   8
#define g_num_KlineOneHourCntMax       4

#define  KLINE_VERSION      "V0.00.01"
#define  KLINE_RELEASEDATE  "2015/01/12"
#define  KLINE_LOGLEVEL     "1"
#define  KLINE_SUBTOPIC     "1012,1112"
#define  KLINE_SUBENDPOINT  "tcp://192.168.15.200:2010"
#define  KLINE_PUBTOPIC     "2013,2113"
#define  KLINE_PUBENDPOINT  "tcp://192.168.15.200:2012"

#define CONFIG_KEY_VECTOR_DEFAULT \
{ "version", "releasedate", "loglevel", "subtopic", "subendpoint", "pubtopic", "pubendpoint" }
#define JSON_FILE_DEFAULT "config.json"

#endif