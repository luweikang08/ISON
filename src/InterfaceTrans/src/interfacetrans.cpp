#include "interfacetrans.h"
#include "sbeinterface.h"
#include "jsonhelper.h"
#include "ibsdstool.h"
extern int g_pub_topic ;
extern int g_count;
extern int g_log_level;
int Sbe2JsonLevel2::OnStart(ison::base::ActorContext &){
	int ret=Subscribe("sub", itostring(Level2));
	LOG_IF(INFO,ret==0) << "Sub topic :" << itostring(Level2);
	return 0;
}
int Sbe2JsonLevel2::OnEvent(Event &e){
	std::string sbe_mess;
	int ret;
	if (e.type() == kPublish){
		g_count++;
		sbe_mess = e.message();
		baseline::SDS_Level2 sds_level2;
		std::string json_mess;
		SBE2Json(sbe_mess, sds_level2, json_mess);
		switch (g_pub_topic)
		{
		case 1:
			ret=Publish("pub", itostring(Level2), json_mess); break;//以数据类别作为topic
			ret=Publish("pub", sds_level2.code(), json_mess); break;//以stockid作为topic
		case 2:
			ret=Publish("pub", itostring(Level2), json_mess); break;//以数据类别作为topic		
		case 3:
			ret=Publish("pub", sds_level2.code(), json_mess); break;//以stockid作为topic
		default:ret=Publish("pub", itostring(Level2), json_mess); break;//以数据类别作为topic	
		}	
		LOG_IF(INFO, g_count % 30000 == 0) << json_mess;
		LOG_IF(INFO, g_log_level<5) << json_mess;
	}
	return ret;
}
int Sbe2JsonTrans::OnStart(ison::base::ActorContext &){
	int ret = Subscribe("sub", itostring(Transaction));
	LOG_IF(INFO, ret == 0) << "Sub topic :" << itostring(Transaction);
	return 0;
}

int Sbe2JsonTrans::OnEvent(Event &e){

	std::string sbe_mess;
	int ret;
	if (e.type() == kPublish){
		g_count++;
		sbe_mess = e.message();
		baseline::SDS_Transaction sds_trans;
		std::string json_mess;
		SBE2Json(sbe_mess, sds_trans, json_mess);
		switch (g_pub_topic)
		{
		case 1:
			ret=Publish("pub", itostring(Transaction), json_mess); break;//以数据类别作为topic
			ret=Publish("pub", sds_trans.code(), json_mess); break;//以stockid作为topic
		case 2:
			ret=Publish("pub", itostring(Transaction), json_mess); break;//以数据类别作为topic		
		case 3:
			ret=Publish("pub", sds_trans.code(), json_mess); break;//以stockid作为topic
		default:
			ret=Publish("pub", itostring(Transaction), json_mess); break;//以数据类别作为topic	
		}
		LOG_IF(INFO, g_count % 30000 == 0) << json_mess;
		LOG_IF(INFO, g_log_level<5) << json_mess;
	}
	return ret;
}
int Sbe2JsonQueue::OnStart(ison::base::ActorContext &){
	int ret = Subscribe("sub", itostring(OrderQueue));
	LOG_IF(INFO, ret == 0) << "Sub topic :" << itostring(OrderQueue);
	return 0;
}
int Sbe2JsonQueue::OnEvent(Event &e){

	std::string sbe_mess;//Recv data
	int ret = 0;
	if (e.type() == kPublish){
		g_count++;
		sbe_mess = e.message();
		baseline::SDS_OderQueen sds_queue;
		std::string json_mess;
		SBE2Json(sbe_mess, sds_queue, json_mess);
	
		switch (g_pub_topic)
		{
		case 1:
			ret=Publish("pub", itostring(OrderQueue), json_mess); break;//pub with datatype topic
			ret=Publish("pub", sds_queue.code(), json_mess); break;//pub with stockid topic
		case 2:
			ret=Publish("pub", itostring(OrderQueue), json_mess); break;//以数据类别作为topic		
		case 3:
			ret=Publish("pub", sds_queue.code(), json_mess); break;//以stockid作为topic
		default:
			ret=Publish("pub", itostring(OrderQueue), json_mess); break;//以数据类别作为topic	
		}
		LOG_IF(INFO, g_count % 30000 == 0) << json_mess;
		LOG_IF(INFO, g_log_level<5) << json_mess;
	}
	return ret;
}
