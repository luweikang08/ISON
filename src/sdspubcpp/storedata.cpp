#include  <iostream>
#include  <string>
#include  <vector>
#include  <map>
#include  <fstream>
#include  "isonbase.h"
#include  "iblog.h"
#include  "context.h"
#include  "stage.h"
#include  "actor.h"
#include  "event.h"
#include  "ibprotocol.h"
#include  "socket.h"
#include  "message.h"
#include  "filehelper.h"
#include "ibsdstool.h"
#include "isonutil.h"

using namespace ison::base;
std::string g_source2store;
std::string g_topic;
class StorePlayDataActor :public Actor{
public:
	StorePlayDataActor(std::string id_) :Actor(id_){};
	int OnStart(ActorContext&)
	{
		std::vector<std::string> topic_spit;
		split(g_topic, ";", &topic_spit);
		for (int i = 0; i < topic_spit.size();i++)
		{
			Subscribe("subfromsource", topic_spit[i]);
		}
		return 0;
	}
	int OnEvent(Event& e)
	{
		std::string smss;
		if (e.type() == kPublish) {//此部分实现推送功能
			smss = e.message() + "\n";//接受KMDS-Socket 推送过来的所有消息，目前推送的是上交所和深交所的股票数据
			WriteData2file(smss, "playdata.txt", "./store");
		}
		return 0;
	}
	
};

int main(int argc, char* argv[]) {
	Context ctx;
	Stage persistent_stage(ctx);//持久化到文件中
	std::cout << "Please input  the source endpoint like 'tcp://127.0.0.1:2000':" << std::endl;
	std::cin >> g_source2store;
	std::cout << "Please input  the topic and separate  with  a semicolon like '1012;1013':" << std::endl;
	std::cin >> g_topic;
	persistent_stage.AddSubscriber("subfromsource", g_source2store);
	ActorPtr apersistent_store(new StorePlayDataActor("PersistentStore"));
	persistent_stage.AddActor(apersistent_store);
	persistent_stage.Start();
	persistent_stage.Join();
	return 0;
}

