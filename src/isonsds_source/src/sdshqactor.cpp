
#include "interfacetrans.h"
#include "ibserverdatadef.h"
#include "ibsdstool.h"
#include <map>
#include "isonsdsdataapi.h"

extern int g_nlog_level;
extern int g_HQSource;
std::multimap< std::string, int> stock_signal;//用于存储topic以及对于请求的stockid
int counthq=0;
int level2 = 0;
int sellintime = 0;
HQMulticastActor::HQMulticastActor(std::string id_) :Actor(id_)
{	
}
int HQMulticastActor::OnStart(ActorContext&) {
	Subscribe("sub1", "alldata");//订阅
	return 0;
}
int  HQMulticastActor::OnEvent(Event& e) {
	std::string smss;
	if (e.type() == kPublish) {//此部分实现推送功能
		smss = e.message();//接受KMDS-Socket 推送过来的所有消息，目前推送的是上交所和深交所的股票数据
		const char * sscrcode = "";//SCR_CODE	证券字段名	String
		const char * szttopic = "";
		const	 char* cmss = smss.c_str();
		rapidjson::Document documentprase;
		documentprase.Parse<0>(cmss);
		if (documentprase.HasParseError())  //解析发生错误时错误提示  
		{
			printf("GetParseError %s\n", documentprase.GetParseError());
			return -1;//解析失败
		}
		else if (documentprase.IsObject()) //把level2转化成ibserver需要的格式
		{
			if (documentprase.HasMember("Seqno")) //
			{
				if (!documentprase.HasMember("Seqno") || !documentprase["Seqno"].IsInt()) 	
				return -2;//没有Seqno字段或者"Seqno"字段类型不对
				Publish("pub1", itostring(documentprase["Seqno"].GetInt()).c_str(), cmss);//以数据类型为主题法布出去
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
	Subscribe("sub1", "alldata");//订阅
	return 0;
}
int  StockPubActor::OnEvent(Event& e){
	std::string smss;
	
	if (e.type() == kPublish) {//此部分实现推送功能
		smss = e.message();//接受KMDS-Socket 推送过来的所有消息，目前推送的是上交所和深交所的股票数据
		const char * sscrcode = "";//SCR_CODE	证券字段名	String
		const char * szttopic = "";
		//printf("SocketDAta：%s", smss.c_str());
		const	 char* cmss = smss.c_str();//类似{"TM":102128000, "MKT_TP_CODE" : 1, "SCR_CODE" : "990843".........}	
		rapidjson::Document documentprase;
		documentprase.Parse<0>(cmss);
		//对推送过来的消息进行解析
		if (documentprase.HasParseError())  //解析发生错误时错误提示  
		{
			printf("GetParseError %s\n", documentprase.GetParseError());
			return -1;//解析失败
		}
		else if (documentprase.IsObject())
		{
			if (!documentprase.HasMember("body") || !documentprase["body"].IsObject())
				return  -1;
			rapidjson::Value& body = documentprase["body"];
			
			if (!body.HasMember("SCR_CODE") || !body["SCR_CODE"].IsString())
				return  -1;
				sscrcode = body["SCR_CODE"].GetString();//可以根据股票代码来订阅多只股票	
		}

		//stock_signal.find(sscrcode);

		std::string serch_iterm = sscrcode;//根据股票代码来查找信号
		/*
		for (auto beg = stock_signal.lower_bound(serch_iterm), end = stock_signal.upper_bound(serch_iterm); beg != end; ++beg)
		{
			char signaltopic[100] = {};
			itoa(beg->second, signaltopic, 10);//把数字转化成char
			Publish("pub1", signaltopic, smss.c_str());//Publish到客户端，此处是根据股票查找到对应的信号作为过滤主题发布出去
		}
		*/

/*
	}
	else {//此部分实现订阅功能
		if (e.type() == kMessage) {
			//std::cout << proto->message() << std::endl;
			smss = e.message();//获取请求端的json请求数据
			rapidjson::Document documentprase;
			const	 char* cmss = smss.c_str();
			printf("s%", cmss);
			e.Reply(cmss);
			//解析客户端json格式的请求
			documentprase.Parse<0>(cmss);
			int nsubscribletype = 0;//决定订阅方式 1
			int failflag = 0;//订阅成功与否的标志位 0 成功，1 失败

			if (documentprase.HasParseError())  //解析发生错误时错误提示  
			{
				printf("GetParseError %s\n", documentprase.GetParseError());
			}
			else if (documentprase.IsObject())   //存储signal 以及signal对应订阅的股票
			{

				if (documentprase.HasMember("signal")&documentprase.HasMember("stock"))//
				{
					const rapidjson::Value &signal = documentprase["signal"];
					int nsignal = signal.GetInt();
					const rapidjson::Value &stock = documentprase["stock"];//得到一个数组
					if (stock.IsArray())
					{
						for (rapidjson::SizeType i = 0; i < stock.Size(); ++i) //如果不是数组，这一行会报错  
						{
							const rapidjson::Value &stocktemp = stock[i];///得到第i个stock
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




