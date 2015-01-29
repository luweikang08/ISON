#include "sourcepublic.h"

int MessageSend(const char * topic, const char * data, ison::base::Socket  *socket){
	ison::base::Message  message;
	message.AddStr(topic);//主题
	message.AddStr(data);//发送的内容
	int res = message.Send(socket->self());//推送消息发送到Socket中
	return res;
};
int MessageSend(const char * topic, const char * data, int & len,ison::base::Socket  *socket){
	ison::base::Message  message;
	message.AddStr(topic);//主题
	message.AddMem(data,len);//发送的内容
	int res = message.Send(socket->self());//推送消息发送到Socket中
	return res;
};

bool CodeIsFilter(const char * code, std::vector<std::string> &code_filter){
	if (!code_filter.empty())//只需要过滤的数据，其余的不要
	{
		for (unsigned int i = 0; i < code_filter.size(); i++)
		{
			if (atoi(code) == atoi(code_filter[i].c_str()))
				return 1;//需要的		
		}
	}
	else{//过滤列表为空，全部需要，不过滤
		return 1;
	}
	return 0;//列表中没有的，不发送
};