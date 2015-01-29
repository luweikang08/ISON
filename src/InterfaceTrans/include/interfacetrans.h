#ifndef ISON_SDS_INTERFACE_TRANS_H_
#define ISON_SDS_INTERFACE_TRANS_H_
#include "isonbase.h"
#include "iblog.h"
#include "context.h"
#include "stage.h"
#include "actor.h"
#include "event.h"
#include "ibprotocol.h"
#include "socket.h"
#include"message.h"

using namespace ison::base;
class Sbe2JsonLevel2 :public Actor{
public:
	Sbe2JsonLevel2(const std::string id_):Actor(id_){};
	int OnStart(ActorContext&);
	int OnEvent(Event& e);
};
class Sbe2JsonTrans :public Actor{
public:
	Sbe2JsonTrans(const std::string id_) :Actor(id_){};
	int OnStart(ActorContext&);
	int OnEvent(Event& e);
};
class Sbe2JsonQueue :public Actor{
public:
	Sbe2JsonQueue(const std::string id_) :Actor(id_){};
	int OnStart(ActorContext&);
	int OnEvent(Event& e);
};

#endif