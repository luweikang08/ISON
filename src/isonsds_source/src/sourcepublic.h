#ifndef  ISON_BASE_SDS_SOURCE_PUBLIC_H_
#define    ISON_BASE_SDS_SOURCE_PUBLIC_H_
#include "socket.h"
#include "message.h"
#include <string.h>
#include <vector>
int MessageSend(const char * topic, const char * data, ison::base::Socket  *socket);
int MessageSend(const char * topic, const char * data, int & len, ison::base::Socket  *socket);
bool CodeIsFilter(const char * code, std::vector<std::string> &code_filter);
#endif