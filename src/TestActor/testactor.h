#ifndef ISON_SDS_Test_H_
#define ISON_SDS_Test_H_
#include "isonbase.h"
#include "iblog.h"
#include "context.h"
#include "stage.h"
#include "actor.h"
#include "event.h"
#include "ibprotocol.h"
#include "socket.h"
#include"message.h"
//
//#include "jsonhelper.h"
//#include"ibsdstool.h"
using namespace ison::base;
struct ThreeTime{
	int pub_time;
	int true_time;
	int sub_time;
};

struct ProfileData{
	int min;
	int max;
	long long sum;
	int size;
	void reset(){
		min = 235959000;
		max = 0;
		sum = 0;
		size = 0;
	}

};
class Level2TimeDealay :public Actor{
public:
	Level2TimeDealay(const std::string id_) :Actor(id_){};
	int OnStart(ActorContext&);
	int OnEvent(Event& e);
};
class TransactionDealay :public Actor{
public:
	TransactionDealay(const std::string id_) :Actor(id_){};
	int OnStart(ActorContext&);
	int OnEvent(Event& e);
};
class OrderqueueDealay :public Actor{
public:
	OrderqueueDealay(const std::string id_) :Actor(id_){};
	int OnStart(ActorContext&);
	int OnEvent(Event& e);
};

class CaculateDealay :public Actor{
public:
	CaculateDealay (const std::string id_) :Actor(id_){};
	int OnStart(ActorContext&);
	int OnEvent(Event& e);
};
//“‘œ¬µƒ≤‚ ‘
class Level2TDFDealay :public Actor{
public:
	Level2TDFDealay(const std::string id_) :Actor(id_){};
	int OnStart(ActorContext&);
	int OnEvent(Event& e);
};

class TransTDFDealay :public Actor{
public:
	TransTDFDealay(const std::string id_) :Actor(id_){};
	int OnStart(ActorContext&);
	int OnEvent(Event& e);
};
class OrderQueueTDFDealay :public Actor{
public:
	OrderQueueTDFDealay(const std::string id_) :Actor(id_){};
	int OnStart(ActorContext&);
	int OnEvent(Event& e);
};
class CaculateTDFDealay :public Actor{
public:
	CaculateTDFDealay(const std::string id_) :Actor(id_){};
	int OnStart(ActorContext&);
	int OnEvent(Event& e);
};

#endif