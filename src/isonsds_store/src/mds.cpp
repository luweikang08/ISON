#include "mds.h"
#include "filehelper.h"
#include "mdsd.h"
#include "isonsdsdataapi.h"
#include "ibsdstool.h"
#include"isonutil.h"
#include <ostream>
#include<iostream>
#ifdef _WIN32
#define pathformat "%s\\%s\\"
#else
#define pathformat "%s/%s/"
#endif

Mds::Mds()
{
	//先对所有文件的头进行初始化
	MdsHeader mds_header;
	mds_header.Version;

}

Mds::~Mds()
{
}
void MdsDir(const char* market_id, const char* stock_id, int data_type, std::string &outdir, string &outsuffix)
{
	char path[512];
	const char *data_type_;
	const char *filesuffix;
	switch (data_type)
	{
	case MSD_Level2:
		data_type_ = "level2";
		filesuffix = "lel2";
		break;
	case MSD_Transaction:
		data_type_ = "transaction";
		filesuffix = "trns";
		break;
	case MSD_OrderQueue:
		data_type_ = "orderqueue";
		filesuffix = "ordq";
		break;
	default:break;
	}
	sprintf(path, pathformat, market_id, data_type_);///sh/transaction/600446.trns,
	outdir = path;
	outsuffix = filesuffix;

};
void  DateTimeKey(int date, int time,string &outkey)
{
	char key[18];
	memset(key, '0', sizeof(key));
	std::string sdate = itostring(date);
	std::string stime = itostring(time);
	StrInsertChar(sdate, 0, key);
	if (time<100000000)	
		StrInsertChar(stime, 9, key);
	else
		StrInsertChar(stime, 8, key);
	key[17] = '\0';
	outkey = key;
}



int Mds::Put(const char* market_id, const char* stock_id, int data_type, int date, int time, void* data, size_t len){

	//根据const char* market_id, const char* stock_id, int data_type创建文件及索引文件
	std::string spath;
	std::string filesuffix;
	MdsDir(market_id, stock_id, data_type, spath, filesuffix);
	CreatDir(spath.c_str());
	//创建文件名
	std::string mds_file = spath + stock_id +"."+ filesuffix;
	//创建mds类
	/*
	std::ofstream ofs(mds_file, std::ios::out | std::ios::binary);
	if (!ofs)
	{
		std::cerr << "Can't open " << mds_file<< "!" << std::endl;
		exit(1);
	}*/
	Mdsd *mdsd=new Mdsd();
	mdsd->Open(mds_file.c_str());
	int indx=mdsd->Append(data, len);//返回数据序号
	//根据日期时间形成关键字
	//char key[17];
	std::string keytemp;
	DateTimeKey(date, time, keytemp);
	Mdsi*mdsi = new Mdsi();
	string mds_file_idx = mds_file + ".idx";
	mdsi->Open(mds_file_idx.c_str());///sh/transaction/600446.trns.idx
	Key mdskey;
	const char* temp= keytemp.c_str();
	sprintf(mdskey.key, "%s", temp);
	//strcpy(mdskey.key, temp);
	//StrInsertChar(keytemp, 0, mdskey.key);
	//mdsi->Insert(mdskey, indx);//插入索引
	mdsi->Insert(mdskey,indx);//插入索引
	//delete(mdsi);
	//delete(mdsd);
	return 0;
};
int Mds::Get(const char* market_id, const char* stock_id, int data_type, int date, int time, void* data, size_t len){
	//先定位文件及索引文件
	std::string spath;
	std::string filesuffix;
	MdsDir(market_id, stock_id, data_type, spath, filesuffix);
	std::string mds_file = spath + stock_id + "." + filesuffix;//文件
	string mds_file_idx = mds_file + ".idx";//索引文件
	//从索引文件找到对应的序号
	//char key[17];
	std::string keytemp;
	DateTimeKey(date, time, keytemp);
	Mdsi *mdsi = new Mdsi();
	Key mdskey;
	const char* temp = keytemp.c_str();
	sprintf(mdskey.key, "%s", temp);
	//strcpy(mdskey.key, temp);
	int idx=0;
	mdsi->Open(mds_file_idx.c_str());///sh/transaction/600446.trns.idx
	mdsi->Lookup(mdskey,&idx);//查到序号
	//根据序号取出内容
	Mdsd *mdsd = new Mdsd();
	//Mdsd::Get(int idx, void**buf, size_t* len);
	char buf[1024];
	mdsd->Get(idx,(void**)&buf, &len);
	//mdsd->Get(idx, &data, len);//取出内容
	return 0;
};
/*
int Get(const char* market_id, const char* stock_id, int data_type, int begin_date, int begin_time, int end_date, int end_time, void* data, size_t len){





}*/