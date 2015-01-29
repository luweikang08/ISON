
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
#include "libckmdsapi/ckmdsdef.h"
using namespace ison::base;
//This Actor publish HQ include sellintime and level2
class HQMulticastActor :public Actor{
public:
	HQMulticastActor(std::string id_);
	int OnStart(ActorContext&);
	int OnEvent(Event& e);

};

//This Actor publish Stock infomation£¬which is subscrible by client
class StockPubActor :public Actor{
public:
	StockPubActor(std::string id_);
	int OnStart(ActorContext&);
	int OnEvent(Event& e);

};
