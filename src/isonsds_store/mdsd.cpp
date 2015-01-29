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
	//��ͷд���ļ�
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
{ //���ļ��а�ͷ������
	std::ifstream ifs(path_,std::ios::in|std::ios::binary);
	if (!ifs)
	{
		std::cerr << "Can't open " << path_ << "!" << std::endl;
		exit(1);
	}
	char recvBuf[256];
	ifs.read(recvBuf, sizeof(MdsHeader));
	//ת���ɶ���
	MdsHeader *mds_header = (MdsHeader*)recvBuf;
	return *mds_header;
}

int Mdsd::Append(void * data, size_t len)//׷������
{
	std::ofstream ofs(path_, std::ios::out | std::ios::binary);
if (!ofs)
{
	std::cerr   << "Can't open "<<path_ <<"!" << std::endl;
	exit(1);
}
//���T1��¼���͵ĳ���
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
//������׷�ӵ��ļ�ĩβ

ofs.write((const char*)data, len);
ofs.seekp(sizeof(MdsHeader), std::ios::end);
//�����ǰ���ļ�����
int idx= ofs.tellp()/len;//�õ���¼�����
ofs.close();
return idx;
}
/*
//��ȡ����
int Mdsd::Get(int idx, void**buf, size_t* len){

	std::ifstream ifs(path_, std::ios::in | std::ios::binary);
	if (!ifs)
	{
		std::cerr << "Can't open " << path_ << "!" << std::endl;
		//exit(1);
		return -1;
	}
	//����idx��λ���ļ�λ��
	ifs.seekg(sizeof(MdsHeader)+(idx*(int)len));
	ifs.read((char*)&buf, *len);//
	return 0;
}*/
//��ȡ����
int Mdsd::Get(int idx, void**buf, size_t* len){
	int count = 1;
	this->Get(idx, &count, buf, len);
	return 0;
}

//������ȡ�������
int Mdsd::Get(int idx, int* count, void** buf, size_t* len){
	std::ifstream ifs(path_, std::ios::in | std::ios::binary);
	if (!ifs)
	{
		std::cerr << "Can't open " << path_ << "!" << std::endl;
		//exit(1);
		return -1;
	}
	//����idx��λ���ļ�λ��
	ifs.seekg(sizeof(MdsHeader) + (idx*(int)len));
	ifs.read((char*)&buf, (*len)*(*count));//
	return 0;
}

int Mdsi::Open(const char * path)
{
	this->path_ = path;
	return 0;
}

int Mdsi::Insert(Key &key,int value)//�Ѽ�ֵ��Ӧ����Ӧ�����
{

	bpt::bplus_tree  btree(path_,false);
	btree.insert((bpt::key_t)key.key,value);
	//�޸��ļ�ͷ����ĳЩ��Ϣ

	return 0;
};
int Mdsi::Lookup(Key &key,int *value)
{
	bpt::bplus_tree  btree(path_, false);
	btree.search((bpt::key_t)key.key, value);
	return 0;
};