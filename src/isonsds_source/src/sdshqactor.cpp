
#include "interfacetrans.h"
#include "ibserverdatadef.h"
#include "ibsdstool.h"
#include <map>
#include "isonsdsdataapi.h"

extern int g_nlog_level;
extern int g_HQSource;
std::multimap< std::string, int> stock_signal;//���ڴ洢topic�Լ����������stockid
int counthq=0;
int level2 = 0;
int sellintime = 0;
HQMulticastActor::HQMulticastActor(std::string id_) :Actor(id_)
{	
}
int HQMulticastActor::OnStart(ActorContext&) {
	Subscribe("sub1", "alldata");//����
	return 0;
}
int  HQMulticastActor::OnEvent(Event& e) {
	std::string smss;
	if (e.type() == kPublish) {//�˲���ʵ�����͹���
		smss = e.message();//����KMDS-Socket ���͹�����������Ϣ��Ŀǰ���͵����Ͻ���������Ĺ�Ʊ����
		const char * sscrcode = "";//SCR_CODE	֤ȯ�ֶ���	String
		const char * szttopic = "";
		const	 char* cmss = smss.c_str();
		rapidjson::Document documentprase;
		documentprase.Parse<0>(cmss);
		if (documentprase.HasParseError())  //������������ʱ������ʾ  
		{
			printf("GetParseError %s\n", documentprase.GetParseError());
			return -1;//����ʧ��
		}
		else if (documentprase.IsObject()) //��level2ת����ibserver��Ҫ�ĸ�ʽ
		{
			if (documentprase.HasMember("Seqno")) //
			{
				if (!documentprase.HasMember("Seqno") || !documentprase["Seqno"].IsInt()) 	
				return -2;//û��Seqno�ֶλ���"Seqno"�ֶ����Ͳ���
				Publish("pub1", itostring(documentprase["Seqno"].GetInt()).c_str(), cmss);//����������Ϊ���ⷨ����ȥ
				 LOG(INFO) << "PUB smessage" << cmss  ;
			}
	}
	
	}

	return 0;
} 



/*
StockPubActor::StockPubActor(std::string id_) :Actor(id_)
{
}
int StockPubActor::OnStart(ActorContext&) {
	Subscribe("sub1", "alldata");//����
	return 0;
}
int  StockPubActor::OnEvent(Event& e){
	std::string smss;
	
	if (e.type() == kPublish) {//�˲���ʵ�����͹���
		smss = e.message();//����KMDS-Socket ���͹�����������Ϣ��Ŀǰ���͵����Ͻ���������Ĺ�Ʊ����
		const char * sscrcode = "";//SCR_CODE	֤ȯ�ֶ���	String
		const char * szttopic = "";
		//printf("SocketDAta��%s", smss.c_str());
		const	 char* cmss = smss.c_str();//����{"TM":102128000, "MKT_TP_CODE" : 1, "SCR_CODE" : "990843".........}	
		rapidjson::Document documentprase;
		documentprase.Parse<0>(cmss);
		//�����͹�������Ϣ���н���
		if (documentprase.HasParseError())  //������������ʱ������ʾ  
		{
			printf("GetParseError %s\n", documentprase.GetParseError());
			return -1;//����ʧ��
		}
		else if (documentprase.IsObject())
		{
			if (!documentprase.HasMember("body") || !documentprase["body"].IsObject())
				return  -1;
			rapidjson::Value& body = documentprase["body"];
			
			if (!body.HasMember("SCR_CODE") || !body["SCR_CODE"].IsString())
				return  -1;
				sscrcode = body["SCR_CODE"].GetString();//���Ը��ݹ�Ʊ���������Ķ�ֻ��Ʊ	
		}

		//stock_signal.find(sscrcode);

		std::string serch_iterm = sscrcode;//���ݹ�Ʊ�����������ź�
		/*
		for (auto beg = stock_signal.lower_bound(serch_iterm), end = stock_signal.upper_bound(serch_iterm); beg != end; ++beg)
		{
			char signaltopic[100] = {};
			itoa(beg->second, signaltopic, 10);//������ת����char
			Publish("pub1", signaltopic, smss.c_str());//Publish���ͻ��ˣ��˴��Ǹ��ݹ�Ʊ���ҵ���Ӧ���ź���Ϊ�������ⷢ����ȥ
		}
		*/

/*
	}
	else {//�˲���ʵ�ֶ��Ĺ���
		if (e.type() == kMessage) {
			//std::cout << proto->message() << std::endl;
			smss = e.message();//��ȡ����˵�json��������
			rapidjson::Document documentprase;
			const	 char* cmss = smss.c_str();
			printf("s%", cmss);
			e.Reply(cmss);
			//�����ͻ���json��ʽ������
			documentprase.Parse<0>(cmss);
			int nsubscribletype = 0;//�������ķ�ʽ 1
			int failflag = 0;//���ĳɹ����ı�־λ 0 �ɹ���1 ʧ��

			if (documentprase.HasParseError())  //������������ʱ������ʾ  
			{
				printf("GetParseError %s\n", documentprase.GetParseError());
			}
			else if (documentprase.IsObject())   //�洢signal �Լ�signal��Ӧ���ĵĹ�Ʊ
			{

				if (documentprase.HasMember("signal")&documentprase.HasMember("stock"))//
				{
					const rapidjson::Value &signal = documentprase["signal"];
					int nsignal = signal.GetInt();
					const rapidjson::Value &stock = documentprase["stock"];//�õ�һ������
					if (stock.IsArray())
					{
						for (rapidjson::SizeType i = 0; i < stock.Size(); ++i) //����������飬��һ�лᱨ��  
						{
							const rapidjson::Value &stocktemp = stock[i];///�õ���i��stock
							std::string sstock = stocktemp.GetString();
							stock_signal.insert({ sstock, nsignal });
						}
					}
				}

			}
			e.Reply("subscribe success!");
		}
	}
	return 0;

}

*/




