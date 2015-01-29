#include "sourcepublic.h"

int MessageSend(const char * topic, const char * data, ison::base::Socket  *socket){
	ison::base::Message  message;
	message.AddStr(topic);//����
	message.AddStr(data);//���͵�����
	int res = message.Send(socket->self());//������Ϣ���͵�Socket��
	return res;
};
int MessageSend(const char * topic, const char * data, int & len,ison::base::Socket  *socket){
	ison::base::Message  message;
	message.AddStr(topic);//����
	message.AddMem(data,len);//���͵�����
	int res = message.Send(socket->self());//������Ϣ���͵�Socket��
	return res;
};

bool CodeIsFilter(const char * code, std::vector<std::string> &code_filter){
	if (!code_filter.empty())//ֻ��Ҫ���˵����ݣ�����Ĳ�Ҫ
	{
		for (unsigned int i = 0; i < code_filter.size(); i++)
		{
			if (atoi(code) == atoi(code_filter[i].c_str()))
				return 1;//��Ҫ��		
		}
	}
	else{//�����б�Ϊ�գ�ȫ����Ҫ��������
		return 1;
	}
	return 0;//�б���û�еģ�������
};