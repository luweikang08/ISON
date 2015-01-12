#include <iostream>
#include "isonbase.h"
#include "iblog.h"
#include "context.h"
#include "stage.h"
#include "actor.h"
#include "event.h"
#include "ibprotocol.h"

using namespace ison::base;

const std::string g_str_connid = "connect1";
const std::string g_str_endpoint = "tcp://127.0.0.1:5555";

class ReplyActor : public Actor {
public:
	ReplyActor() : Actor("aa") {}
	int OnStart(Event& e){
		//e.Forward("tcp://127.0.0.1:5555", "test", "request");
		std::cout << e.message() << std::endl;
		std::cout << "test start ok!" << std::endl;
		e.Reply("test start ok!");
		return 0;
	}
	int OnEvent(Event& e) {
		std::cout << e.message() << std::endl;
		std::cout << "test OnEvent ok!" << std::endl;
		e.Reply("ok!");
		return 0;
	}
};

class RequestActor : public Actor {
public:
	RequestActor() : Actor("Requs") {}
	int OnEvent(Event& e) {
		std::cout << e.message() << std::endl;
		e.Forward(g_str_connid, "test", "request");
		e.Reply("Requs ok!");
		return 0;
	}
	int OnStart(Event& e){
		e.Forward(g_str_connid, "test", "request start");
		std::cout << e.message() << std::endl;
		return 0;
	}
};

int main(int argc, char* argv[]) {
	//iblog_init(argv[0]);
	//iblog_v(10);
	Context ctx, ctx1;
	Stage stage(ctx), stage1(ctx1);
	stage.Bind("tcp://*:5555");
	stage1.Bind("tcp://*:5556");
	ActorPtr actor(new ReplyActor());
	ActorPtr actor1(new RequestActor());
	stage.AddActor(actor);
	stage1.AddActor(actor1);
	stage1.Connect(g_str_connid, g_str_endpoint);
	//stage.Connect("con1", "tcp://*:5556");
	stage1.Start();
	stage.Start();
	stage1.Join();
	stage.Join();
	return 0;
}