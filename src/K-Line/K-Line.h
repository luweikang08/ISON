#ifndef __ISON_SDS_K_LINE_H__
#define __ISON_SDS_K_LINE_H__

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

#define g_num_DataStoreCntMax          30
#define g_num_KlineOneMinuteCntMax     242 //9:30:00.000-11:29:59.999  13:00:00.000-14:59:59.999,
#define g_num_KlineFiveMinuteCntMax    48
#define g_num_KlineTenMinuteCntMax     24
#define g_num_KlineFifteenMinutCntMax  16
#define g_num_KlineThirtyMinutCntMax   8
#define g_num_KlineOneHourCntMax       4

#endif