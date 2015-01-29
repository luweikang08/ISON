#ifndef ISON_SDS_2ATRADE_H_
#define ISON_SDS_2ATRADE_H_
#include "isonbase.h"
#include "iblog.h"
#include "context.h"
#include "stage.h"
#include "actor.h"
#include "event.h"
#include "ibprotocol.h"
#include "socket.h"
#include"message.h"

#include "jsonhelper.h"
#include"ibsdstool.h"
using namespace ison::base;
class Level22Atrade :public Actor{
public:
	Level22Atrade(const std::string id_):Actor(id_){};
	int OnStart(ActorContext&);
	int OnEvent(Event& e);
};

class Level2Req :public Actor{
public:
	Level2Req(const std::string id_) :Actor(id_){};
	int OnStart(ActorContext&);
	int OnEvent(Event& e);
};
class SellInTime2Atrade:public Actor{
public:
	SellInTime2Atrade(const std::string id_):Actor(id_){};
	int OnStart(ActorContext&);
	int OnEvent(Event& e);
};

class ClosePriceReq :public Actor{
public:
	ClosePriceReq(const std::string id_) :Actor(id_){};
	int OnEvent(Event& e);
};


class SubCodeTable:public Actor{
public:
	SubCodeTable(const std::string id_) :Actor(id_){};
	int OnStart(ActorContext&);
	int OnEvent(Event& e);
};

//—” ±≤‚ ‘








#endif
