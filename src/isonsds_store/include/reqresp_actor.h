#include "isonbase.h"
#include "iblog.h"
#include "context.h"
#include "stage.h"
#include "actor.h"
#include "event.h"
#include "ibprotocol.h"
#include "socket.h"
#include"message.h"
#include <string>
#include "document.h"
#include "prettywriter.h"
#include "filestream.h"
#include "stringbuffer.h"
#include "writer.h"
#include "isonsdsdataapi.h"
#include "timehelper.h"
using namespace ison::base;
/*
class Level2Pub:public Actor{//Push  ����
public:
	Level2Pub(std::string id_) :Actor(id_){};
	int OnStart(ActorContext&){
        
	
	};
	int OnEvent(Event& e);
};
class TransActionPub :public Actor{//Push  ����
public:
	TransActionPub(std::string id_) :Actor(id_){};
	int OnStart(ActorContext&);
	int OnEvent(Event& e);
};
class OderPub :public Actor{
public:
	OderPub(std::string id_) :Actor(id_){};
	int OnStart(ActorContext&);
	int OnEvent(Event& e);

};
class OderQueuePub :public Actor{
public:
	OderQueuePub(std::string id_) :Actor(id_){};
	int OnStart(ActorContext&);
	int OnEvent(Event& e);
};


*/



class Variance :public Actor{//����   
public:
	Variance(std::string id_);
	int OnStart(ActorContext&);
	int OnEvent(Event& e);
};

class SubMarketDataResp :public Actor{//�ظ�����
public:
	SubMarketDataResp(std::string id_);
	//int OnStart(ActorContext&);
	int OnEvent(Event& e);
};