#ifndef _UTIL_H_
#define _UTIL_H_

#include <string>
#include<stdio.h>
#include <vector>
using  std::string;
void split(const std::string& s, const std::string& delim, std::vector< std::string >* ret);
int StrInsertChar(string in, int offset, char *out);
#endif