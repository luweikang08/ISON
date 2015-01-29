#include "mdsd.h"
#include <fstream>
#include<iostream>
#include"isonsdsdataapi.h"
#include"bpt.h"

int Mdsd::Open(const char*path){//set the open path 
	this->path_ = path;
	return 0;
}
int Mdsd::Init(MdsHeader& mds_header){
	//this->mds_header_= mds_header;
	//把头写入文件
	char recvBuf[256];
	memset(recvBuf, 0, 256);
	memcpy(recvBuf, (char *)&mds_header, sizeof(mds_header));
	std::string header(recvBuf, sizeof(MdsHeader));
	std::ofstream ofs(path_, std::ios::out | std::ios::binary);
	if (!ofs)
	{
		std::cerr << "Can't open " << path_ << "!" << std::endl;
		exit(1);
	}
	ofs.write(header.c_str(), sizeof(MdsHeader));
	ofs.close();

	return 0;
};
const MdsHeader& Mdsd::GetHeader()
{ //从文件中把头读出来
	std::ifstream ifs(path_,std::ios::in|std::ios::binary);
	if (!ifs)
	{
		std::cerr << "Can't open " << path_ << "!" << std::endl;
		exit(1);
	}
	char recvBuf[256];
	ifs.read(recvBuf, sizeof(MdsHeader));
	//转化成对象
	MdsHeader *mds_header = (MdsHeader*)recvBuf;
	return *mds_header;
}

int Mdsd::Append(void * data, size_t len)//追加数据
{
	std::ofstream ofs(path_, std::ios::out | std::ios::binary);
if (!ofs)
{
	std::cerr   << "Can't open "<<path_ <<"!" << std::endl;
	exit(1);
}
//求出T1记录类型的长度
/*
int data_length;
switch (mds_header_.Data_type)
{
case MSD_Level2:
	data_length = sizeof(SDS_Level2);
case MSD_Transaction:
	data_length = sizeof(SDS_Transaction);
case MSD_OrderQueue:
	data_length = sizeof(SDS_OderQueen);

default:break;
}*/
//把内容追加到文件末尾

ofs.write((const char*)data, len);
ofs.seekp(sizeof(MdsHeader), std::ios::end);
//求出当前主文件长度
int idx= ofs.tellp()/len;//得到记录的序号
ofs.close();
return idx;
}
/*
//读取数据
int Mdsd::Get(int idx, void**buf, size_t* len){

	std::ifstream ifs(path_, std::ios::in | std::ios::binary);
	if (!ifs)
	{
		std::cerr << "Can't open " << path_ << "!" << std::endl;
		//exit(1);
		return -1;
	}
	//根据idx定位到文件位置
	ifs.seekg(sizeof(MdsHeader)+(idx*(int)len));
	ifs.read((char*)&buf, *len);//
	return 0;
}*/
//读取数据
int Mdsd::Get(int idx, void**buf, size_t* len){
	int count = 1;
	this->Get(idx, &count, buf, len);
	return 0;
}

//连续读取多个数据
int Mdsd::Get(int idx, int* count, void** buf, size_t* len){
	std::ifstream ifs(path_, std::ios::in | std::ios::binary);
	if (!ifs)
	{
		std::cerr << "Can't open " << path_ << "!" << std::endl;
		//exit(1);
		return -1;
	}
	//根据idx定位到文件位置
	ifs.seekg(sizeof(MdsHeader) + (idx*(int)len));
	ifs.read((char*)&buf, (*len)*(*count));//
	return 0;
}

int Mdsi::Open(const char * path)
{
	this->path_ = path;
	return 0;
}

int Mdsi::Insert(Key &key,int value)//把键值对应到相应的序号
{

	bpt::bplus_tree  btree(path_,false);
	btree.insert((bpt::key_t)key.key,value);
	//修改文件头，的某些信息

	return 0;
};
int Mdsi::Lookup(Key &key,int *value)
{
	bpt::bplus_tree  btree(path_, false);
	btree.search((bpt::key_t)key.key, value);
	return 0;
};