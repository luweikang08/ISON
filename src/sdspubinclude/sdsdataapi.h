#ifndef ISON_SDS_DATA_API_H_
#define ISON_SDS_DATA_API_H_
#include "MessageHeader.hpp"
#include "SDS_Level2.hpp"
#include "SDS_Transaction.hpp"
#include "SDS_OderQueen.hpp"
#include  <string>
#define messageHeaderVersion  0
#define sbeBufferLength 1024
enum SBE_ERR{
	SBE_ERR_OVER_BUFFER = 200,
	SBE_ERR_SIZE,
	SBE_ERR_CODE_ERR,
	SBE_TYPE_ERR
};
using namespace baseline;
template<typename Interface_SBE>
inline int DecodeSBE(const std::string &smss, Interface_SBE &SDS_SBE);
#endif
