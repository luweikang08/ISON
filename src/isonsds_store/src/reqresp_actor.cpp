#include "reqresp_actor.h"
#include"memorydata.h"
#include<vector>
#include"ibsdstool.h"
#include"jsonhelper.h"
;
extern int g_nlog_level;
extern MemoryData< SDS_Level2 > g_level2;
extern MemoryData< SDS_Transaction > g_transaction;
extern MemoryData<SDS_Order> g_order;
extern MemoryData<SDS_OderQueen> g_order_queen;
int parase(std::string message, SubscribeMarketDataReq &maket_data_req);

/*
int Level2Pub::OnStart(ActorContext &){
	const char * leve2 = itostring(Level2).c_str();
	Subscribe("subfromsource", leve2);
	return 0;
}
int Level2Pub::OnEvent(Event &e){
	//SubscribeMarketData（）
	//{Seqno：“”，DateType：“”，BeginDate：“”，BeginTime：“”，EndDate：“”，EndTime：“”，CodeTable：[]}
	std::string message;
	if (e.type() == kMessage)
	{
		message = e.message();
		
			



		return 0;


	}
}







*/


SubMarketDataResp::SubMarketDataResp(std::string id_) :Actor(id_){}//查询行情请求
int SubMarketDataResp::OnEvent(Event  &e){
	/*if (e.type() == kMessage){
		std::string message = e.message();
		    SubscribeMarketDataReq market_data_req = {0};
			parase(message, market_data_req);
			int a = market_data_req.begin_time;
			DateAndTime date_time = { 0, 0 };
			std::vector<std::string> table_code = market_data_req.code_table;//查询的股票列表
			//datatype进行位运算 
			int data_type = market_data_req.date_type & 0x00FF;//按位与 确定低八位的数值
			//最高位为0  为1表示信号id
			int flag[8] = { 0 };//flag[0]is the bit8 ,flag[7]is bit0
			for (int i = 0; i < 8; i++)
				flag[i] = 0x0080 & (data_type << i);//取各位的值 
			if (flag[0] != 0)return -1;//表示信号id

			
			date_time = GetDateAndTime();
			if (market_data_req.begin_date == 0 && market_data_req.begin_time == 0 &&
				market_data_req.end_date == 0 && market_data_req.end_time == 0)//表示订阅
			{
				if (flag[1] == 0x0080)//Push  OderQueue
				{
					 for (auto iter = table_code.begin(); iter != table_code.end(); ++iter)
					 {
						 std::vector<SDS_Order> vsds_oder;//查询到一支股票的就发出去一次
						 g_order.SelectMessage(*iter, market_data_req.begin_time, market_data_req.end_time, vsds_oder);
						 const char * code = (*iter).c_str();
		
						// QueryReply(vsds_oder, code, itostring(Order).c_str());
						 e.Reply(itostring(Order).c_str());
					 }
				}    
					
				
				if (flag[2] == 0x0080)//Push  Oder
				{
					for (auto iter = table_code.begin(); iter != table_code.end(); ++iter)
					{
						std::vector<SDS_Order> vsds_oder;//查询到一支股票的就发出去一次
						g_order.SelectMessage(*iter, market_data_req.begin_time, market_data_req.end_time, vsds_oder);
						const char * code = (*iter).c_str()
						//QueryReply(vsds_oder, code, itostring(Order).c_str());
						//e.Reply(itostring(Order));
					}
				}
	    
				if (flag[3] == 0x0080)//Push  SellInTime
				{
					for (auto iter = table_code.begin(); iter != table_code.end(); ++iter)
					{
						std::vector<SDS_Order> vsds_oder;//查询到一支股票的就发出去一次
						g_order.SelectMessage(*iter, market_data_req.begin_time, market_data_req.end_time, vsds_oder);
						const char * code = (*iter).c_str();
	
						//QueryReply(vsds_oder, code, itostring(Order).c_str());
						e.Reply(itostring(Order).c_str());
					}






				}

				if (flag[1] == 0x0080)//Push  level2
				{









				}
					






			}
			else if ((market_data_req.begin_date == 0 || date_time.date - market_data_req.begin_date == 0) &&
				((market_data_req.end_date == 0 || market_data_req.end_date == -1) || date_time.date - market_data_req.end_date == 0))//Request  today data!   1
			{
				if (flag[1] == 0x0080){
				    /* //根据查询条件 形成正则表达式
					const char * message = "{\"Table\":\"Order.kch\",\"Regex\":\" \"}";//
					e.Forward("con1","MemoryStore",message);*/
				//	for (auto it = table_code.begin(); it != table_code.end(); it++)
						//g_order.SelectMessage(*it, market_data_req.begin_time, market_data_req.end_time, result);



		
				//};//bit7 is Order  
				/*if (flag[2] == 0x0080){
					//for (auto it = table_code.begin(); it != table_code.end(); it++)
					//	g_order_queen.SelectMessage(*it, market_data_req.begin_time, market_data_req.end_time, result);

				};//bit6 is OrderQueue
				if (flag[3] == 0x0080){
					//for (auto it = table_code.begin(); it != table_code.end(); it++)
					//	g_transaction.SelectMessage(*it, market_data_req.begin_time, market_data_req.end_time, result); 

				
				};//bi5 is TransAction

				if (flag[4] == 0x0080 || flag[6] == 0x0080 || flag[7] == 0x0080){//行情统一交给网关处理
					//for (auto it = table_code.begin(); it != table_code.end(); it++)
						//g_level2.SelectMessage(*it, market_data_req.begin_time, market_data_req.end_time, result); 
				}
					
			//	for (auto it = result.begin(); it != result.end(); it++)
				//	e.Reply((*it).c_str());
					/*
				}//if (flag[5] == 1){ };//保留         
				else if (flag[6] == 0x0080)//bit2 is basic HQ
				{	                                                     		}
				else if (flag[7] == 0x0080)//bit1 is Price
				{		}*/
				
				
			//查询内存数据库
			//}
			/*else if (date_time.date - market_data_req.begin_date > 0)//Request the passed data!
			{  
				if (date_time.date - market_data_req.end_date > 0)//all passed      2
				{
					//查询历史数据库

				  
				}
				else                                         //3  先查询内存数据库在查询历史数据库
				{
					//1、查询历史数据库




					//2、查询内存数据库



				}


				;
			}
			else if(date_time.date - market_data_req.begin_date <0)
			{
				printf("Input params error,I can't know the future ");
				return -2;
			};//Request is  error*/


	

			//}
			
			
		return 0;
	}
	Variance::Variance(std::string id_) : Actor(id_){}
	int Variance::OnStart(ActorContext &){
		return 0;

	}
	int Variance::OnEvent(Event &e){

		//先读取时间差数据
		//排序
		//最大、最小
		//
		int n = 10, sum = 0;
		for (int i = 0; i < n; i++)  
		{sum +=i;  }
		float avg = sum / n; float v = 0;
		for (int i = 0; i < n; i++)
		{
			v += (i - avg)*(i - avg);
		}
		v = v / n;
			


			return 0;
	}