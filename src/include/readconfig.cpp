#include "readconfig.h"
//old read json config file
int readjsonfile(const char* filename, std::vector <std::string> configkey, std::map<std::string, double> &configmap)
{
	std::ifstream is;
	is.open(filename);
	if (is){
		std::cout << "Read config file " << filename << " sucessfully!" << std::endl;
		std::string str((std::istreambuf_iterator<char>(is)),
			std::istreambuf_iterator<char>());
		rapidjson::Document documentprase;
		const char* ssms;
		ssms = str.c_str();
		documentprase.Parse<0>(ssms);
		if (documentprase.HasParseError())
		{
			return -1;
			printf("ParseError error:%s\n", documentprase.GetParseError());
		}
		else if (documentprase.IsObject())
		{
			for (auto key : configkey)
			{
				//if (!documentprase.HasMember(key.c_str()) || documentprase[key.c_str()].IsString()) 
				//std::cout << "Parase config file fail :" << key<<"is not string or exist!"<<std::endl;
				if (!documentprase.HasMember(key.c_str()))
				{
					std::cout << "Parase config file fail :" << key << " is not  exist!" << std::endl;
					return -1;

				}
				if (documentprase[key.c_str()].IsString())
				{
					std::string temp = documentprase[key.c_str()].GetString();
					configmap[key] = atoi(temp.c_str());
				}
				else if (documentprase[key.c_str()].IsInt())
				{
					configmap[key] = documentprase[key.c_str()].GetInt();
				}
				else {
					return -1;
					std::cout << "Parase config file fail :" << key << " type is not  string or int!" << std::endl;
				}
			}
		}
	}
	else { return -1; std::cout << "Read config file fail,so  use the default params!" << std::endl; }
	is.close();
	return 0;
}
/*
*read setting infomation from json config file
*filename : (in)config file name in json format,ex:XXX.json,
*masterkey : (in)key of destination block,typical is application name,ex:argv[0]
*configkey : (in)destination key of block belongs to masterkey
configmap : (out)map of result
*/
RF_RetCode readjsonfile(const char* filename, const char* masterkey, std::vector <std::string> configkey, std::map<std::string, std::string> &configmap)
{
	std::ifstream is;
	is.open(filename);
	if (is)
	{
		std::cout << "Read config file " << filename << " sucessfully!" << std::endl;
		std::string str((std::istreambuf_iterator<char>(is)),
			std::istreambuf_iterator<char>());
		rapidjson::Document documentprase;
		const char* ssms;
		ssms = str.c_str();
		documentprase.Parse<0>(ssms);
		if (documentprase.HasParseError())
		{
			printf("ParseError error:%s\n", documentprase.GetParseError());
			is.close();
			return RF_PARSE_ERROR;
		}
		else if (documentprase.IsObject())
		{
			cout << masterkey << endl;
			if (documentprase.HasMember(masterkey))
			{
				rapidjson::Value &body = documentprase[masterkey];
				if (body.IsObject())
				{
					for (auto key : configkey)
					{
						if (body[key.c_str()].IsString())
						{
							configmap[key] = body[key.c_str()].GetString();
						}
						if (body[key.c_str()].IsInt())
						{
							configmap[key] = itostring(body[key.c_str()].GetInt());
						}
					}
					is.close();
					return RF_SUCCESS;
				}
				else
				{
					is.close();
					return RF_PARSE_ERROR;
				}
			}
			else
			{
				is.close();
				return RF_KEY_NOT_EXIST;
			}
		}
	}
	else
	{
		std::cout << "Read config file fail,so  use the default params!" << std::endl;
		is.close();
		return RF_FILE_NOT_EXIST;
	}
	return RF_FAIL;
}