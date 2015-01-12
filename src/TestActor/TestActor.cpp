//
//#include "ibsdstool.h"
#include <map>
#include<mutex>
//#include "isonutil.h"
#include"testactor.h"
//#include"timehelper.h"

#define g_str_subid "sub1";

extern int g_nlog_level;
extern int gHQSource;
int transaction_count = 0;
int level2_count = 0;
int oder_count = 0;
int oder_queue_count = 0; 
int misslevel2 = 0;
int misstrans = 0;
int missordque = 0;
int timecount = 0;
extern int windowmin;
std::mutex mutex_read_code;
std::map< std::string, std::string> mcode_table;//stockid an stock name
std::map< std::string, std::string> mlevel2;//stock the last level2
std::vector<ThreeTime> level2_timemin10;
std::vector<ThreeTime> level2_timemin5;
//std::vector<ThreeTime> level2_timemin5;
std::vector<ThreeTime> trans_timemin10;
std::vector<ThreeTime> trans_timemin5;
std::vector<ThreeTime> ordque_timemin10;
std::vector<ThreeTime> ordque_timemin5;

ProfileData level2subture_pre5 = {235959000,0,0,0};
ProfileData level2subture_now5 = { 235959000, 0, 0, 0 };
ProfileData level2subpub_pre5 = { 235959000, 0, 0, 0 };
ProfileData level2subpub_now5 = { 235959000, 0, 0, 0 };
ProfileData level2turepub_pre5 = { 235959000, 0, 0, 0 };//
ProfileData level2turepub_now5 = { 235959000, 0, 0, 0 };//

ProfileData transsubture_pre5 = { 235959000, 0, 0, 0 };
ProfileData transsubture_now5 = { 235959000, 0, 0, 0 };
ProfileData transsubpub_pre5 = { 235959000, 0, 0, 0 };
ProfileData transsubpub_now5 = { 235959000, 0, 0, 0 };
ProfileData transturepub_pre5 = { 235959000, 0, 0, 0 };//
ProfileData transturepub_now5 = { 235959000, 0, 0, 0 };//

ProfileData orderqueuesubture_pre5 = { 235959000, 0, 0, 0 };
ProfileData orderqueuesubture_now5 = { 235959000, 0, 0, 0 };
ProfileData orderqueuesubpub_pre5 = { 235959000, 0, 0, 0 };//
ProfileData orderqueuesubpub_now5 = { 235959000, 0, 0, 0 };//

ProfileData orderqueueturepub_pre5 = { 235959000, 0, 0, 0 };//
ProfileData orderqueueturepub_now5 = { 235959000, 0, 0, 0 };//

int Level2TimeDealay::OnStart(ActorContext&){
	Subscribe("sub1", "83");
	return 0;
};
int TransactionDealay::OnStart(ActorContext&){
	Subscribe("sub1", "84");
	return 0;
};
int OrderqueueDealay::OnStart(ActorContext&){
	Subscribe("sub1", "86");
	return 0;
};
//int PutThreetime(std::string  &smss, ThreeTime  &threetime,int  &sn)
//{
//	DateAndTime recv4tdftime = GetDateAndTime();
//
//	recv4tdftime.time;
//	
//	//������ SDSsource�յ���ʱ���sub���ܵ���ʱ��
//	rapidjson::Document documentprase;
//	documentprase.Parse<0>(smss.c_str());
//	const char * IntParams[] = { "TDFTime", "Time","Sn" };//8
//	//const char * IntParams[] = {"Time" };
//	if (documentprase.HasParseError())  //������������ʱ������ʾ  
//	{
//		printf("GetParseError %s\n", documentprase.GetParseError());
//		return -1;//����ʧ��
//	}
//	else if (documentprase.IsObject())
//	{
//		for (int i = 0; i < sizeof(IntParams) / sizeof(IntParams[0]); i++)
//		{
//			if (!documentprase.HasMember(IntParams[i]) || !documentprase[IntParams[i]].IsInt())
//				return -1;
//		}
//	
//		threetime.pub_time = documentprase["TDFTime"].GetInt();//�Ӻ�����pub��ʱ��
//		//LOG(INFO) << "pubTDFTime" << threetime.pub_time;
//		threetime.true_time = documentprase["Time"].GetInt();//�ɽ���ʱ��
//		//LOG(INFO) << "trueTime" << threetime.true_time;
//		threetime.sub_time = recv4tdftime.time;             //sub�õ���ʱ��
//		printf("ture:%dpubtime:%dsubtime%d\n", threetime.true_time, threetime.pub_time, threetime.sub_time);
//		//LOG(INFO) << "subtime" << recv4tdftime.time;
//		sn = documentprase["Sn"].GetInt();//���		    
//	};
//	return 0;
//}/*
//int Cucalate(std::vector<int> &data,int &min,int &max, float &avg, float  &variance)
//{
//	int len = data.size();
//   //������
//	for (int  j = 0; j<len -1; j++)
//		for (int i = 0; i<len - 1 - j; i++)
//		{
//		if (data[i]>data[i + 1])//����Ԫ�ش�С����������
//		{
//			int temp = data[i];
//			data[i] = data[i+1];
//			data[i+1] = temp;
//		}
//		}
//	min = data[0];
//	max = data[len - 1];
//	int sum = 0;
//	for (int i = 0; i < len; i++)
//	{
//		sum += i;}
//	avg = float(sum) / (float)len;
//	printf("sum:%d/len:%d=%f",sum,len,avg);
//
//	for (int i = 0; i < len; i++)
//	{
//		variance += (i - avg)*(i - avg);
//	}
//	variance = variance / len;
//	return 0;
//}*/
/*
int CaculateMA(std::vector<int> &min5, std::vector<int> &min10,float &avg,int &min,int &max,int &lencount){

	int len = min10.size();
	min = min10[0];
	min = min10[0];
	for (int i= 0; i<len - 1; i++)
	{ 
		if (min10[i] < min)
			min = min10[i] < min;
		if (min10[i] > max)
	     max = min10[i];
	}
	int lenmin5 = min5.size();
	if (len == 0 || lenmin5 == 0) return -1;
	int summin5 = 0;
	for (int i = 0; i < lenmin5; i++)
		summin5 += min5[i];
	int summin10 = 0;
	for (int i = 0; i < len; i++)
		summin10 += min10[i];
	avg = float(summin10 + summin5) / (lenmin5 + len);
	lencount = len;
	return 0;
}
*/
void SumTimeDelay(ProfileData &threetime,int &delaytime )
{
	if (delaytime < 0)
{
		LOG(INFO) <<"sub-ture time <0";//С��0���������ӳٷ�Χ��
		//printf("sub-ture time <0");//С��0���������ӳٷ�Χ��
}
else{
	threetime.size++;
	if (delaytime < threetime.min)
		threetime.min = delaytime;
	if (delaytime > threetime.max)
	{ 
		threetime.max = delaytime;
		//�������ֵ�ǵļ�¼����
		LOG(INFO) << "T:" << transaction_count << "L:"<<level2_count << "OQ:"<<oder_queue_count;
	}
	    threetime.sum += delaytime;
}
}

int  Level2TimeDealay::OnEvent(Event& e){//���ڼ�¼level2��ʱ��
	std::string smss;
	if (e.type() == kPublish) {//�˲���ʵ�����͹���
		level2_count++;
		smss = e.message();
		printf("%s", smss);
	//	LOG_IF(INFO, g_nlog_level>5) << "SDS@Atrede recv:" << smss.c_str();
	//	ThreeTime threetime_level;
	//	int snlevel2;
	//	PutThreetime(smss, threetime_level, snlevel2);//����������ʱ��
	//	if (level2_count != snlevel2)      //���ݶ�ʧ�ʼ���
	//		misslevel2++;
	//	//sub-ture �ӳٴ���
	//	int subturelevel2 = threetime_level.sub_time - threetime_level.true_time;
	////	printf("sub:%dture:%ddelay:%d%\n", threetime_level.sub_time, threetime_level.true_time, subturelevel2);
	//	printf("sub:%dture:%ddelay:%d%\n", threetime_level.sub_time, threetime_level.true_time, subturelevel2);
	//	SumTimeDelay(level2subture_now5, subturelevel2);
	//	//pub��sub��ʱ��	
	//	int subpublevel2 = threetime_level.sub_time - threetime_level.pub_time;
	//	//printf("sub:%dpub:%ddelay:%d%\n", threetime_level.sub_time, threetime_level.pub_time, subpublevel2);
	//	SumTimeDelay(level2subpub_now5, subpublevel2);
	//	//pub��ture��ʱ��
	//	int turepublevel2 = threetime_level.pub_time - threetime_level.true_time;
	//	//printf("sub:%dpub:%ddelay:%d%\n", threetime_level.sub_time, threetime_level.pub_time, subpublevel2);
	//	SumTimeDelay(level2turepub_now5, turepublevel2);
	}
	return 0;
}
int TransactionDealay::OnEvent(Event& e){//���ڼ�¼level2��ʱ��
	std::string smss;
	if (e.type() == kPublish) {//�˲���ʵ�����͹���
		transaction_count++;
		smss = e.message();
		printf("%s", smss);
		//LOG_IF(INFO, g_nlog_level>5) << "SDS@Atrede recv:" << smss.c_str();
		//ThreeTime threetime_trans;
		//int sntrans;
		//PutThreetime(smss, threetime_trans, sntrans);
		//if (transaction_count!= sntrans)      //���ݶ�ʧ�ʼ���
		//	misstrans++;
		////sub-ture �ӳٴ���
		//int subturetrans = threetime_trans.sub_time - threetime_trans.true_time;
		//SumTimeDelay(transsubture_now5, subturetrans);
		////pub��sub��ʱ��
		//int subpubtrans = threetime_trans.sub_time - threetime_trans.pub_time;
		//SumTimeDelay(transsubpub_now5, subpubtrans);
		////ture��pub��ʱ��
		//int turepubtrans = threetime_trans.pub_time - threetime_trans.true_time;
		//SumTimeDelay(transturepub_now5, turepubtrans);

	}
	return 0;
}
int  OrderqueueDealay::OnEvent(Event& e){//���ڼ�¼level2��ʱ��
	std::string smss;
	if (e.type() == kPublish) {//�˲���ʵ�����͹���
	oder_queue_count++;
		smss = e.message();
		printf("%s", smss);
		//LOG_IF(INFO, g_nlog_level>5) << "SDS@Atrede recv:" << smss.c_str();
		//ThreeTime threetime_orderqueue;
		//int snorderqueue;
		//PutThreetime(smss, threetime_orderqueue, snorderqueue);
		//if (oder_queue_count!= snorderqueue)      //���ݶ�ʧ�ʼ���
		//	missordque++;
		////sub-ture �ӳٴ���
		//int subtureordque = threetime_orderqueue.sub_time - threetime_orderqueue.true_time;
		//SumTimeDelay(orderqueuesubture_now5, subtureordque);
		////pub��sub��ʱ��
		//int subpubordque = threetime_orderqueue.sub_time - threetime_orderqueue.pub_time;
		//SumTimeDelay(orderqueuesubpub_now5, subpubordque);
	}
	return 0;
}
int CaculateDealay::OnStart(ActorContext&){
	AddTimer(windowmin * 60* 1000);//ÿ15
	//AddTimer(10 * 1000);//ÿ15���Ӽ���һ��
	return 0;
};

int CaculateDealay::OnEvent(Event &e){
	
	//Level2 ���ܼ���
		//�ȼ���now��pre
	//1
	 //float  level2subtureavg = float(level2subture_now5.sum + level2subture_pre5.sum) /
		//(level2subture_now5.size + level2subture_pre5.size);
		////���now��ֵ��pre������now����
		//level2subture_pre5 = level2subture_now5;
		//char bufferlevel2_st[512];
		//memset(bufferlevel2_st, 0, 512);
		//sprintf(bufferlevel2_st, "[min]:%d[max]:%d[count]:%d[avg]:%f and [miss]:%d",
		//	level2subture_now5.min, level2subture_now5.max, level2subture_now5.size, level2subtureavg, misslevel2);
		//LOG(INFO) << "level2 delay sub - ture ****" << bufferlevel2_st;
		//level2subture_now5.reset();//����now
		////2
		//float  level2subpubavg = float(level2subpub_now5.sum + level2subpub_pre5.sum) /
		//	(level2subpub_now5.size + level2subpub_pre5.size);
		//level2subpub_pre5 = level2subpub_now5;
		//char bufferlevel2_sp[512];
		//memset(bufferlevel2_sp, 0, 512);
		//sprintf(bufferlevel2_sp, "[min]:%d[max]:%d[count]:%d[avg]:%f and [miss]:%d",
		//	level2subpub_now5.min, level2subpub_now5.max, level2subpub_now5.size, level2subpubavg, misslevel2);
		//LOG(INFO) << "level2 delay sub - pub ****" << bufferlevel2_sp;
		//level2subpub_now5.reset();
		////misslevel2 = 0;//��ʧ���ݵĸ�����0
		////3   
		//float  level2turepubavg = float(level2turepub_now5.sum + level2turepub_pre5.sum) /
		//	(level2turepub_now5.size + level2turepub_pre5.size);
		//level2turepub_pre5 = level2turepub_now5;
		//char bufferlevel2_tp[512];
		//memset(bufferlevel2_tp, 0, 512);
		//sprintf(bufferlevel2_tp, "[min]:%d[max]:%d[count]:%d[avg]:%f and [miss]:%d",
		//	level2turepub_now5.min, level2turepub_now5.max, level2turepub_now5.size, level2turepubavg, misslevel2);
		//LOG(INFO) << "level2 delay ture- pub ****" << bufferlevel2_tp;
		//level2turepub_now5.reset();
		//misslevel2 = 0;//��ʧ���ݵĸ�����0



		////Transaction���ܼ���

		//float  transsubtureavg = float(transsubture_now5.sum + transsubture_pre5.sum) /
		//	(transsubture_now5.size + transsubture_pre5.size);
		////���now��ֵ��pre������now����
		//transsubture_pre5 = transsubture_now5;
		//char buffertrans_st[512];
		//memset(buffertrans_st, 0, 512);
		//sprintf(buffertrans_st, "[min]:%d[max]:%d[count]:%d[avg]:%f and [miss]:%d",
		//	transsubture_now5.min, transsubture_now5.max, transsubture_now5.size, transsubtureavg, misstrans);
		//LOG(INFO) << "trans delay sub - ture ****" << buffertrans_st;
		//transsubture_now5.reset();//����now

		//float  transsubpubavg = float(transsubpub_now5.sum + transsubpub_pre5.sum) /
		//	(transsubpub_now5.size + transsubpub_pre5.size);
		//transsubpub_pre5 = transsubpub_now5;
		//char buffertrans_sp[512];
		//memset(buffertrans_sp, 0, 512);
		//sprintf(buffertrans_sp, "[min]:%d[max]:%d[count]:%d[avg]:%f and [miss]:%d",
		//	transsubpub_now5.min, transsubpub_now5.max, transsubpub_now5.size, transsubpubavg, misstrans);
		//LOG(INFO) << "trans delay sub - pub ****" << buffertrans_sp;
		//transsubpub_now5.reset();
		////misstrans = 0;//��ʧ���ݵĸ�����0

		//float  transturepubavg = float(transturepub_now5.sum + transturepub_pre5.sum) /
		//	(transturepub_now5.size + transturepub_pre5.size);
		//transturepub_pre5 = transturepub_now5;
		//char buffertrans_tp[512];
		//memset(buffertrans_tp, 0, 512);
		//sprintf(buffertrans_tp, "[min]:%d[max]:%d[count]:%d[avg]:%f and [miss]:%d",
		//	transturepub_now5.min, transturepub_now5.max, transturepub_now5.size, transturepubavg, misstrans);
		//LOG(INFO) << "trans delay ture - pub ****" << buffertrans_tp;
		//transsubpub_now5.reset();
		//misstrans = 0;//��ʧ���ݵĸ�����0



		////OrderQueue���ܼ���

		//float  orderqueuesubtureavg = float(orderqueuesubture_now5.sum + orderqueuesubture_pre5.sum) /
		//	(orderqueuesubture_now5.size + orderqueuesubture_pre5.size);
		////���now��ֵ��pre������now����
		//orderqueuesubture_pre5 = orderqueuesubture_now5;
		//char bufferorderqueue_st[512];
		//memset(bufferorderqueue_st, 0, 512);
		//sprintf(bufferorderqueue_st, "[min]:%d[max]:%d[count]:%d[avg]:%f and [miss]:%d",
		//	orderqueuesubture_now5.min, orderqueuesubture_now5.max, orderqueuesubture_now5.size, orderqueuesubtureavg, missordque);
		//LOG(INFO) << "orderqueue delay sub - ture ****" << bufferorderqueue_st;
		//orderqueuesubture_now5.reset();//����now

		//float  orderqueuesubpubavg = float(orderqueuesubpub_now5.sum + orderqueuesubpub_pre5.sum) /
		//	(orderqueuesubpub_now5.size + orderqueuesubpub_pre5.size);
		//orderqueuesubpub_pre5 = orderqueuesubpub_now5;
		//char bufferorderqueue_sp[512];
		//memset(bufferorderqueue_sp, 0, 512);
		//sprintf(bufferorderqueue_sp, "[min]:%d[max]:%d[count]:%d[avg]:%f and [miss]:%d",
		//	orderqueuesubpub_now5.min, orderqueuesubpub_now5.max, orderqueuesubpub_now5.size, orderqueuesubpubavg, missordque);
		//LOG(INFO) << "orderqueue delay sub - pub ****" << bufferorderqueue_sp;
		//orderqueuesubpub_now5.reset();
		//missordque = 0;//��ʧ���ݵĸ�����0

	return 0;
}


int Level2TDFDealay::OnStart(ActorContext&){
	Subscribe("sub1", "83");
	return 0;
};

int Level2TDFDealay::OnEvent(Event& e){//���ڼ�¼level2��ʱ��
	std::string smss;
	if (e.type() == kPublish) {//�˲���ʵ�����͹���
		level2_count++;
		smss = e.message();
		printf("%s", smss);
		//LOG_IF(INFO, g_nlog_level>5) << "SDS@Atrede recv:" << smss.c_str();
		//ThreeTime threetime_level;
		//int snlevel2;
		//PutThreetime(smss, threetime_level, snlevel2);//����������ʱ��
		//if (level2_count != snlevel2)      //���ݶ�ʧ�ʼ���
		//	misslevel2++;
		//level2turepub_now5.size++;
		////sub-ture �ӳٴ���
		//int pubturelevel2 = threetime_level.pub_time - threetime_level.true_time;
		//
		//if (pubturelevel2 < level2turepub_now5.min)
		//	level2turepub_now5.min = pubturelevel2;
		//if (pubturelevel2 > level2turepub_now5.max)
		//{
		//	level2turepub_now5.max = pubturelevel2;
		//	//�������ֵ�ǵļ�¼����
		//	LOG(INFO) << smss.c_str();
		//}
		//level2turepub_now5.sum += pubturelevel2;
	}
	return 0;
}




int TransTDFDealay::OnStart(ActorContext&){
	Subscribe("sub1", "84");
	return 0;
};
int TransTDFDealay::OnEvent(Event& e){//���ڼ�¼level2��ʱ��
	std::string smss;
	if (e.type() == kPublish) {//�˲���ʵ�����͹���
	 transaction_count++;
		smss = e.message();
		//printf("%s", smss);
		//LOG_IF(INFO, g_nlog_level>5) << "SDS@Atrede recv:" << smss.c_str();
		//ThreeTime threetime_trans;
		//int snlevel2;
		//PutThreetime(smss, threetime_trans, snlevel2);//����������ʱ��
		//if (transaction_count != snlevel2)      //���ݶ�ʧ�ʼ���
		//	misstrans++;
		//transturepub_now5.size++;
		////sub-ture �ӳٴ���
		//int pubturtrans= threetime_trans.pub_time - threetime_trans.true_time;

		//if (pubturtrans< transturepub_now5.min)
		//	transturepub_now5.min = pubturtrans;
		//if (pubturtrans> transturepub_now5.max)
		//{
		//	transturepub_now5.max = pubturtrans;
		//	//�������ֵ�ǵļ�¼����
		//	LOG(INFO) << smss.c_str();
		//}
		//transturepub_now5.sum += pubturtrans;
	}
	return 0;
}
int OrderQueueTDFDealay::OnStart(ActorContext&){
	Subscribe("sub1", "86");
	return 0;
};
int OrderQueueTDFDealay::OnEvent(Event& e){//���ڼ�¼level2��ʱ��
	std::string smss;
	if (e.type() == kPublish) {//�˲���ʵ�����͹���
		oder_queue_count++;
		smss = e.message();
		//printf("%s", smss);
		//LOG_IF(INFO, g_nlog_level>5) << "SDS@Atrede recv:" << smss.c_str();
		//ThreeTime threetime_ordqueue;
		//int snlevel2;
		//PutThreetime(smss, threetime_ordqueue, snlevel2);//����������ʱ��
		//if (oder_queue_count != snlevel2)      //���ݶ�ʧ�ʼ���
		//	missordque++;
		////sub-ture �ӳٴ���
		//orderqueueturepub_now5.size++;
		//int pubtureordque = threetime_ordqueue.pub_time - threetime_ordqueue.true_time;

		//if (pubtureordque < orderqueueturepub_now5.min)
		//	orderqueueturepub_now5.min = pubtureordque;
		//if (pubtureordque > orderqueueturepub_now5.max)
		//{
		//	orderqueueturepub_now5.max = pubtureordque;
		//	//�������ֵ�ǵļ�¼����
		//	LOG(INFO) << smss.c_str();
		//}
		//orderqueueturepub_now5.sum += pubtureordque;
	}
	return 0;
}

int CaculateTDFDealay::OnStart(ActorContext&){
	//AddTimer(windowmin * 1 * 1000);//ÿ15
	//AddTimer(10 * 1000);//ÿ15���Ӽ���һ��
	printf("start");
	return 0;
};

int CaculateTDFDealay::OnEvent(Event &e){
	//level2���ӳ�
	//1��pub��ture��ʱ��
	//float  level2turepubavg = float(level2turepub_now5.sum + level2turepub_pre5.sum) /
	//	(level2turepub_now5.size + level2turepub_pre5.size);
	//level2turepub_pre5 = level2turepub_now5;
	//char bufferlevel2_tp[512];
	//memset(bufferlevel2_tp, 0, 512);
	//sprintf(bufferlevel2_tp, "[min]:%d[max]:%d[count]:%d[avg]:%f and [miss]:%d",
	//	level2turepub_now5.min, level2turepub_now5.max, level2turepub_now5.size, level2turepubavg, misslevel2);
	//LOG(INFO) << "level2 delay ture- pub ****" << bufferlevel2_tp;
	//level2turepub_now5.reset();
	//misslevel2 = 0;//��ʧ���ݵĸ�����0



	////Transaction���ܼ���
	////1��pub��ture��ʱ��

	//float  transturepubavg = float(transturepub_now5.sum + transturepub_pre5.sum) /
	//	(transturepub_now5.size + transturepub_pre5.size);
	//transturepub_pre5 = transturepub_now5;
	//char buffertrans_tp[512];
	//memset(buffertrans_tp, 0, 512);
	//sprintf(buffertrans_tp, "[min]:%d[max]:%d[count]:%d[avg]:%f and [miss]:%d",
	//	transturepub_now5.min, transturepub_now5.max, transturepub_now5.size, transturepubavg, misstrans);
	//LOG(INFO) << "trans delay ture - pub ****" << buffertrans_tp;
	//transsubpub_now5.reset();
	//misstrans = 0;//��ʧ���ݵĸ�����0

	////OrderQueue���ܼ���
	////1��pub��ture��ʱ��
	//float  orderqueueturepubavg = float(orderqueueturepub_now5.sum + orderqueueturepub_pre5.sum) /
	//	(orderqueueturepub_now5.size + orderqueueturepub_pre5.size);
	//orderqueueturepub_pre5 = orderqueueturepub_now5;
	//char bufferorderqueue_tp[512];
	//memset(bufferorderqueue_tp, 0, 512);
	//sprintf(bufferorderqueue_tp, "[min]:%d[max]:%d[count]:%d[avg]:%f and [miss]:%d",
	//	orderqueueturepub_now5.min, orderqueueturepub_now5.max, orderqueueturepub_now5.size, orderqueueturepubavg, missordque);
	//LOG(INFO) << "orderqueue delay ture - pub ****" << bufferorderqueue_tp;
	//orderqueuesubpub_now5.reset();
	//missordque= 0;//��ʧ���ݵĸ�����0
	//printf("%s", e.message());
	return 0;
}