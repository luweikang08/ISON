#include "PubSbe.h"

using namespace std;
using namespace ison::base;
using namespace rapidjson;
using namespace baseline;

const char* g_str_pubid = "pub1";
const char* g_str_pubendpoint = "tcp://127.0.0.1:1212";

SDS_Transaction g_TranData;

class ProcActor : public Actor {
public:
	ProcActor(const std::string& id) : Actor(id) {}
	int OnStart(ActorContext&)
	{
		AddTimer(1000);
		cout << "test start ok" << endl;
		return 0;
	}
	int OnEvent(Event& e)
	{
		cout << ".";
		if (e.type() == kPublish)
		{
			string smss = e.message();
		}
		return 0;
	}
};

int main(int argc, char* argv[])
{
	Context ctx;
	Stage stage(ctx);

	

	stage.AddSubscriber(g_str_pubid, g_str_pubendpoint);
	ActorPtr ProcActor(new ProcActor("proc"));
	stage.AddActor(ProcActor);
	stage.Start();
	stage.Join();
	return 0;
}