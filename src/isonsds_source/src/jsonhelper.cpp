
#include <iostream> 
#include<fstream>
#include"iblog.h"
#include<vector>
#include<string>
#include<map>

#include"ibsdstool.h"
#include "document.h"
#include "isonsdsdataapi.h"


//Read the config file
int ReadConfig(const char* filename, std::vector <std::string> configkey,std::map<std::string,std::string> &configmap){
	std::ifstream is;
	is.open(filename);
	if (is){
		std::cout << "Read config file "<<filename<< " sucessfully!" << std::endl;
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
			for (auto key:configkey)
			{ 
				//if (!documentprase.HasMember(key.c_str()) || documentprase[key.c_str()].IsString()) 
					//std::cout << "Parase config file fail :" << key<<"is not string or exist!"<<std::endl;
				if (!documentprase.HasMember(key.c_str()))
				{
					std::cout << "Parase config file fail :" << key << " is not  exist!" << std::endl;
					return -1;
					
				}
				if (documentprase[key.c_str()].IsString())
					configmap[key] = documentprase[key.c_str()].GetString();
				else if (documentprase[key.c_str()].IsInt())
				{  //转化成string
					int temp = documentprase[key.c_str()].GetInt();
					configmap[key]= itostring(temp);//要转化回去的话用atoi
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
//Package the Basic HQ

//Package the Level2

