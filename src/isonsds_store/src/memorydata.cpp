
/*#include "memorydata.h"
#include<exception>

template <typename Tstruct>MemoryData<Tstruct>::MemoryData()
{

}

template <typename Tstruct> int MemoryData<Tstruct>::InsertMessage(std::string & key, MemoryMessage<Tstruct> & memory_message){
	data_[key].push_back(memory_message);
	//for (int i = 0; i < data_[key].size(); i++)
		//printf("%s",data_[key][i].message.c_str());
	return 0;
}
template <typename Tstruct>int MemoryData<Tstruct>::SelectMessage(std::string stock_id, int begintime, int end_time, std::vector<Tstruct> &result){

	std::vector<MemoryMessage> memory_message;
	try{
		memory_message = data_.at(stock_id);
	}
	catch (std::exception )
	{

		return -1;   //now  no data
	}
	int time = memory_message[0].time;
	if (begintime = -1 || begintime <= time){//begin from the first data
		int i = 0;
		do{
			result.push_back(memory_message[i].message);
			i++;
		} while (i<memory_message.size() && memory_message[i].time>end_time);

	}
	else                            //begin from the non-first data
	{
		for (auto it = memory_message.begin(); it != memory_message.end(); it++)
			if ((*it).time >= begintime)
				do{
				
			result.push_back((*it).message);
				
				} while ((*it).time>end_time);




	}

	return 0;
	}
	*/