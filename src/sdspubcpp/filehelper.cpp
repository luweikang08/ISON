#include"filehelper.h"
#ifdef _WIN32
#include <direct.h>
#include <io.h>
#elif _LINUX
#include <stdarg.h>
#include <sys/stat.h>
#include <unistd.h>
#include<string.h>
#endif

#ifdef _WIN32
#define ACCESS _access
#define MKDIR(a) _mkdir((a))
#define strncpy strncpy_s
#elif _LINUX
#define ACCESS access
#define MKDIR(a) mkdir((a),0755)
#endif

int CreatDir(const char *pszDir)
{
	int i = 0;
	int iRet;
	int iLen = strlen(pszDir);

	//在末尾加/
	char dir_path[100];

	if (pszDir[iLen - 1] != '\\' && pszDir[iLen - 1] != '/')
	{
		dir_path[iLen] = '/';
		dir_path[iLen + 1] = '\0';
	}
	else dir_path[iLen] = '\0';
	strncpy(dir_path, pszDir, iLen);

	int len = strlen(dir_path);
	for (i = 0; i<len; i++)
	{
		if (dir_path[i] == '/' && i > 0)
		{
			dir_path[i] = '\0';//一级一级建目录
			if (ACCESS(dir_path, 0) < 0)
			{
				if (MKDIR(dir_path) < 0)
				{
					return -1;
				}
			}
			dir_path[i] = '/';
		}
	}

	return 0;
}

int WriteData2file(const char* data, const char* filename, std::string dir, std::ios_base::openmode mode){
	CreatDir(dir.c_str());
	std::ofstream out(dir + filename, mode);  //打开文件并以追加的形式向文件中写，不存在创建，存在打开

	if (out){
		out << data << std::endl;
	}
	out.close();
	return 0;
}

int WriteData2file(std::string data, const char* filename, std::string dir ){

	CreatDir(dir.c_str());
	std::ofstream out(dir+filename);  //打开文件并以追加的形式向文件中写，不存在创建，存在打开
	
	if (out){
		out << data << std::endl;
		//out.write(data, strlen(data));
	}	
	out.close();
	return 0;
}






int TransferFile(const char * goal, const char * source,std::string goal_dir,std::string source_dir)//转移内存数据库文件和持久化文件
{    
	std::string source_path = source_dir + "/" + source;
	std::string goal_path = goal_dir+ "/" + goal;
	std::ifstream infile(source_path);//打开1.txt    
	std::ofstream out(goal_path, std::ofstream::app);
	std::string temp;
	if (!infile)
	{
		printf("open file %s fail ", source_path);
		return -1;
	}  //不能打开文件，报错   
	else
	{
		if (!out)
		{
			printf("open file %s fail ", goal_path);
			return -1;
		}
		while (getline(infile, temp))
		{
			out << temp << std::endl;
		}
		return -1;
	}

}

int ReadFilterCode(std::vector<std::string> &code_filter, std::string code_path)
{
	std::ifstream infile(code_path);
	if (!infile)
	{
		return -1;
	}
	std::string temp;
	while (getline(infile, temp))
	{
		code_filter.push_back(temp);
	}
	return 0;
}