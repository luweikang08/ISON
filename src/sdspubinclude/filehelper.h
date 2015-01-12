#ifndef __ISON_SDS_STORE_FILEHELPER_H__ 
#define __ISON_SDS_STORE_FILEHELPER_H__
#include <fstream>
#include<stdio.h>
#include<string>
#include<vector>

int WriteData2file(const char* data, const char* filename, std::string dir);
int TransferFile(const char * goal, const char * source, std::string goal_dir, std::string source_dir);
int CreatDir(const char *pszDir);
int ReadFilterCode(std::vector<std::string> &code_filter, std::string code_path);



#endif