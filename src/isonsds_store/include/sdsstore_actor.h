
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
using namespace ison::base;
class MemoryStoreActor :public Actor{//当日行情保存在内存中以供系统的频繁查询
public:
	MemoryStoreActor(std::string id_);
	int OnStart(ActorContext&);
	int OnEvent(Event& e);
};

class PersistentStoreActor :public Actor{//把当日行情持久化存储。日终时将当日数据导入ISON-DAC
public:
	PersistentStoreActor(std::string id_);
	int OnStart(ActorContext&);
	int OnEvent(Event& e);

};
class QueryServiceActor :public Actor{//把当日行情持久化存储。日终时将当日数据导入ISON-DAC
public:
	QueryServiceActor(std::string id_);
	int OnStart(ActorContext&);
	int OnEvent(Event& e);

};
class TransStoreActor :public Actor{//把当日行情持久化存储。日终时将当日数据导入ISON-DAC
public:
	TransStoreActor(std::string id_);
	int OnStart(ActorContext&);
	int OnEvent(Event& e);

};