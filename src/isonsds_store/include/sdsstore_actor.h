
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
class MemoryStoreActor :public Actor{//�������鱣�����ڴ����Թ�ϵͳ��Ƶ����ѯ
public:
	MemoryStoreActor(std::string id_);
	int OnStart(ActorContext&);
	int OnEvent(Event& e);
};

class PersistentStoreActor :public Actor{//�ѵ�������־û��洢������ʱ���������ݵ���ISON-DAC
public:
	PersistentStoreActor(std::string id_);
	int OnStart(ActorContext&);
	int OnEvent(Event& e);

};
class QueryServiceActor :public Actor{//�ѵ�������־û��洢������ʱ���������ݵ���ISON-DAC
public:
	QueryServiceActor(std::string id_);
	int OnStart(ActorContext&);
	int OnEvent(Event& e);

};
class TransStoreActor :public Actor{//�ѵ�������־û��洢������ʱ���������ݵ���ISON-DAC
public:
	TransStoreActor(std::string id_);
	int OnStart(ActorContext&);
	int OnEvent(Event& e);

};