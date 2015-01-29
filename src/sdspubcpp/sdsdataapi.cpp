#include"sdsdataapi.h"
template<typename Interface_SBE>
inline  int  DecodeSBE(const std::string&smss, Interface_SBE &SDS_SBE){
	baseline::MessageHeader hdr;
	char * buffer = const_cast<char *> (smss.c_str());
	hdr.wrap(buffer, 0, messageHeaderVersion, hdr.size());//解析头
	int len = hdr.size() + hdr.blockLength();
	if (len != smss.size()) return SBE_ERR_SIZE; 
	hdr.wrap(buffer, 0, messageHeaderVersion, len);//解析头
	SDS_SBE.wrapForDecode(buffer, hdr.size(), hdr.blockLength(), hdr.version(), len);
	return 0;
}
