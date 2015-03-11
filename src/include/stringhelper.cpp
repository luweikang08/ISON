#include "stringhelper.h"

int GetAppName(const char* src, std::string& dest)
{
	size_t offset = 0;
	std::string srcStr;
	srcStr.append(src);
	if (srcStr.find('\\',0) !=std::string::npos)  //src is full path
	{
		offset = srcStr.find_last_of('\\', srcStr.length());
		if (offset == std::string::npos)
		{
			return -1;
		}
		else
		{
			dest.append(srcStr.substr(offset + 1, sizeof(srcStr) - offset));
			return 0;
		}
	}
	else if (srcStr.find('/', 0) != std::string::npos)  //src is full path
	{
		offset = srcStr.find_last_of('/', srcStr.length());
		if (offset == std::string::npos)
		{
			return -1;
		}
		else
		{
			dest.append(srcStr.substr(offset + 1, sizeof(srcStr) - offset));
			return 0;
		}
	}
	else     //src is filename
	{
		dest.append(src);
		return 0;
	}
}
