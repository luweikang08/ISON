#ifndef __ISON_POINDICATOR_H__
#define __ISON_POINDICATOR_H__

#include <iostream>
#include "isonbase.h"
#include "iblog.h"
#include "context.h"
#include "stage.h"
#include "actor.h"
#include "event.h"
#include "ibprotocol.h"

#include "timehelper.h"
#include "isonsdsdataapi.h"
#include "readconfig.h"
#include "stringhelper.h"
#include "data_defs.h"

#include "ibsdstool.h"
#include "data_defs.h"

#include "MacdBase.h"
#include "KdjBase.h"
#include "RsiBase.h"
#include "BollBase.h"

const char* SUBID_OF_SUB = "sub_subactor";
const char* PUBID_OF_SUB = "pub_subactor";
const char* SUBID_OF_PROC = "sub_procactor";
const char* PUBID_OF_PROC = "pub_procactor";
const char* SUBID_OF_PUB = "sub_pubactor";
const char* PUBID_OF_PUB = "pub_pubactor";
const char* STR_EP_SUB = "inproc://subkline";  //tansform data from stage_sub to stage_proc
const char* STR_EP_PUB = "inproc://pubindicator";  //tansform data from stage_sub to stage_proc


const char* CLOG_DIR = "POILog";
const char* JSON_FILE_DEFAULT = "config.json";

const vector<string> VECTOR_CONFIGKEY = { "version", "releasedate", "loglevel", "subendpoint", "pubendpoint" };
const vector<string> VECTOR_CONFIGVALUE_DEFAULT = { "V0.00.01", "2015/01/26", "1", "tcp://192.168.15.200:2031", "tcp://192.168.15.200:2032" };


#endif