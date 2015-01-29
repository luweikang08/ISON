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
	//SubscribeMarketData����
	//{Seqno��������DateType��������BeginDate��������BeginTime��������EndDate��������EndTime��������CodeTable��[]}
	std::string message;
	if (e.type() == kMessage)
	{
		message = e.message();
		
			



		return 0;


	}
}







*/


SubMarketDataResp::SubMarketDataResp(std::string id_) :Actor(id_){}//��ѯ��������
int SubMarketDataResp::OnEvent(Event  &e){
	/*if (e.type() == kMessage){
		std::string message = e.message();
		    SubscribeMarketDataReq market_data_req = {0};
			parase(message, market_data_req);
			int a = market_data_req.begin_time;
			DateAndTime date_time = { 0, 0 };
			std::vector<std::string> table_code = market_data_req.code_table;//��ѯ�Ĺ�Ʊ�б�
			//datatype����λ���� 
			int data_type = market_data_req.date_type & 0x00FF;//��λ�� ȷ���Ͱ�λ����ֵ
			//���λΪ0  Ϊ1��ʾ�ź�id
			int flag[8] = { 0 };//flag[0]is the bit8 ,flag[7]is bit0
			for (int i = 0; i < 8; i++)
				flag[i] = 0x0080 & (data_type << i);//ȡ��λ��ֵ 
			if (flag[0] != 0)return -1;//��ʾ�ź�id

			
			date_time = GetDateAndTime();
			if (market_data_req.begin_date == 0 && market_data_req.begin_time == 0 &&
				market_data_req.end_date == 0 && market_data_req.end_time == 0)//��ʾ����
			{
				if (flag[1] == 0x0080)//Push  OderQueue
				{
					 for (auto iter = table_code.begin(); iter != table_code.end(); ++iter)
					 {
						 std::vector<SDS_Order> vsds_oder;//��ѯ��һ֧��Ʊ�ľͷ���ȥһ��
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
						std::vector<SDS_Order> vsds_oder;//��ѯ��һ֧��Ʊ�ľͷ���ȥһ��
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
						std::vector<SDS_Order> vsds_oder;//��ѯ��һ֧��Ʊ�ľͷ���ȥһ��
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
				    /* //���ݲ�ѯ���� �γ�������ʽ
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

				if (flag[4] == 0x0080 || flag[6] == 0x0080 || flag[7] == 0x0080){//����ͳһ�������ش���
					//for (auto it = table_code.begin(); it != table_code.end(); it++)
						//g_level2.SelectMessage(*it, market_data_req.begin_time, market_data_req.end_time, result); 
				}
					
			//	for (auto it = result.begin(); it != result.end(); it++)
				//	e.Reply((*it).c_str());
					/*
				}//if (flag[5] == 1){ };//����         
				else if (flag[6] == 0x0080)//bit2 is basic HQ
				{	                                                     		}
				else if (flag[7] == 0x0080)//bit1 is Price
				{		}*/
				
				
			//��ѯ�ڴ����ݿ�
			//}
			/*else if (date_time.date - market_data_req.begin_date > 0)//Request the passed data!
			{  
				if (date_time.date - market_data_req.end_date > 0)//all passed      2
				{
					//��ѯ��ʷ���ݿ�

				  
				}
				else                                         //3  �Ȳ�ѯ�ڴ����ݿ��ڲ�ѯ��ʷ���ݿ�
				{
					//1����ѯ��ʷ���ݿ�




					//2����ѯ�ڴ����ݿ�



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

		//�ȶ�ȡʱ�������
		//����
		//�����С
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