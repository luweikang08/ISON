#ifndef __ISON_SDS_KDJ_H__
#define __ISON_SDS_KDJ_H__

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

#include "jsonhelper.h"
#include "timehelper.h"
#include "kdjhelper.h"
#include "readconfig.h"
#include "stringhelper.h"

#define  KDJ_VERSION      "V0.00.01"
#define  KDJ_DATE         "2015/01/07"
#define  KDJ_SUBTOPIC     "1002"
#define  KDJ_SUBENDPOINT  "tcp://192.168.15.200:2010"
#define  KDJ_PUBTOPIC     "6001"
#define  KDJ_PUBENDPOINT  "tcp://127.0.0.1:2010"

#define CONFIG_KEY_VECTOR_DEFAULT { "version", "subtopic", "subendpoint", "pubtopic", "pubendpoint" }
#define JSON_FILE_DEFAULT "config.json"

#endif