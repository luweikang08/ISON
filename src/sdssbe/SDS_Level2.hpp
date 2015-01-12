/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _SDS_LEVEL2_HPP_
#define _SDS_LEVEL2_HPP_

#if defined(SBE_HAVE_CMATH)
/* cmath needed for std::numeric_limits<double>::quiet_NaN() */
#  include <cmath>
#  define SBE_FLOAT_NAN std::numeric_limits<float>::quiet_NaN()
#  define SBE_DOUBLE_NAN std::numeric_limits<double>::quiet_NaN()
#else
/* math.h needed for NAN */
#  include <math.h>
#  define SBE_FLOAT_NAN NAN
#  define SBE_DOUBLE_NAN NAN
#endif

#include <sbe/sbe.hpp>


using namespace sbe;

namespace baseline {

class SDS_Level2
{
private:
    char *buffer_;
    int bufferLength_;
    int *positionPtr_;
    int offset_;
    int position_;
    int actingBlockLength_;
    int actingVersion_;

    SDS_Level2(const SDS_Level2&) {}

public:

    SDS_Level2(void) : buffer_(NULL), bufferLength_(0), offset_(0) {}

    static const sbe_uint16_t sbeBlockLength(void)
    {
        return (sbe_uint16_t)268;
    }

    static const sbe_uint16_t sbeTemplateId(void)
    {
        return (sbe_uint16_t)1;
    }

    static const sbe_uint16_t sbeSchemaId(void)
    {
        return (sbe_uint16_t)1;
    }

    static const sbe_uint16_t sbeSchemaVersion(void)
    {
        return (sbe_uint16_t)0;
    }

    static const char *sbeSemanticType(void)
    {
        return "";
    }

    sbe_uint64_t offset(void) const
    {
        return offset_;
    }

    SDS_Level2 &wrapForEncode(char *buffer, const int offset, const int bufferLength)
    {
        buffer_ = buffer;
        offset_ = offset;
        bufferLength_ = bufferLength;
        actingBlockLength_ = sbeBlockLength();
        actingVersion_ = sbeSchemaVersion();
        position(offset + actingBlockLength_);
        positionPtr_ = &position_;
        return *this;
    }

    SDS_Level2 &wrapForDecode(char *buffer, const int offset, const int actingBlockLength, const int actingVersion,
                         const int bufferLength)
    {
        buffer_ = buffer;
        offset_ = offset;
        bufferLength_ = bufferLength;
        actingBlockLength_ = actingBlockLength;
        actingVersion_ = actingVersion;
        positionPtr_ = &position_;
        position(offset + actingBlockLength_);
        return *this;
    }

    sbe_uint64_t position(void) const
    {
        return position_;
    }

    void position(const sbe_uint64_t position)
    {
        if (SBE_BOUNDS_CHECK_EXPECT((position > bufferLength_), 0))
        {
            throw "buffer too short";
        }
        position_ = position;
    }

    int size(void) const
    {
        return position() - offset_;
    }

    char *buffer(void)
    {
        return buffer_;
    }

    int actingVersion(void) const
    {
        return actingVersion_;
    }

    static const int seqnoId(void)
    {
        return 1;
    }

    static const int seqnoSinceVersion(void)
    {
         return 0;
    }

    bool seqnoInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *SeqnoMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_uint32_t seqnoNullValue()
    {
        return 4294967294;
    }

    static const sbe_uint32_t seqnoMinValue()
    {
        return 0;
    }

    static const sbe_uint32_t seqnoMaxValue()
    {
        return 4294967293;
    }

    sbe_uint32_t seqno(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 0)));
    }

    SDS_Level2 &seqno(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 0)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int codeId(void)
    {
        return 2;
    }

    static const int codeSinceVersion(void)
    {
         return 0;
    }

    bool codeInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *CodeMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_char_t codeNullValue()
    {
        return (sbe_char_t)0;
    }

    static const sbe_char_t codeMinValue()
    {
        return (sbe_char_t)32;
    }

    static const sbe_char_t codeMaxValue()
    {
        return (sbe_char_t)126;
    }

    static const int codeLength(void)
    {
        return 16;
    }

    const char *code(void) const
    {
        return (buffer_ + offset_ + 4);
    }

    sbe_char_t code(const int index) const
    {
        if (index < 0 || index >= 16)
        {
            throw "index out of range for code";
        }

        return (*((sbe_char_t *)(buffer_ + offset_ + 4 + (index * 1))));
    }

    void code(const int index, const sbe_char_t value)
    {
        if (index < 0 || index >= 16)
        {
            throw "index out of range for code";
        }

        *((sbe_char_t *)(buffer_ + offset_ + 4 + (index * 1))) = (value);
    }

    int getCode(char *dst, const int length) const
    {
        if (length > 16)
        {
             throw "length too large for getCode";
        }

        ::memcpy(dst, buffer_ + offset_ + 4, length);
        return length;
    }

    SDS_Level2 &putCode(const char *src)
    {
        ::memcpy(buffer_ + offset_ + 4, src, 16);
        return *this;
    }

    static const int dateId(void)
    {
        return 3;
    }

    static const int dateSinceVersion(void)
    {
         return 0;
    }

    bool dateInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *DateMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_uint32_t dateNullValue()
    {
        return 4294967294;
    }

    static const sbe_uint32_t dateMinValue()
    {
        return 0;
    }

    static const sbe_uint32_t dateMaxValue()
    {
        return 4294967293;
    }

    sbe_uint32_t date(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 20)));
    }

    SDS_Level2 &date(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 20)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int timeId(void)
    {
        return 4;
    }

    static const int timeSinceVersion(void)
    {
         return 0;
    }

    bool timeInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *TimeMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_uint32_t timeNullValue()
    {
        return 4294967294;
    }

    static const sbe_uint32_t timeMinValue()
    {
        return 0;
    }

    static const sbe_uint32_t timeMaxValue()
    {
        return 4294967293;
    }

    sbe_uint32_t time(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 24)));
    }

    SDS_Level2 &time(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 24)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int sDSTimeId(void)
    {
        return 5;
    }

    static const int sDSTimeSinceVersion(void)
    {
         return 0;
    }

    bool sDSTimeInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *SDSTimeMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_uint32_t sDSTimeNullValue()
    {
        return 4294967294;
    }

    static const sbe_uint32_t sDSTimeMinValue()
    {
        return 0;
    }

    static const sbe_uint32_t sDSTimeMaxValue()
    {
        return 4294967293;
    }

    sbe_uint32_t sDSTime(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 28)));
    }

    SDS_Level2 &sDSTime(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 28)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int snId(void)
    {
        return 6;
    }

    static const int snSinceVersion(void)
    {
         return 0;
    }

    bool snInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *SnMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_uint32_t snNullValue()
    {
        return 4294967294;
    }

    static const sbe_uint32_t snMinValue()
    {
        return 0;
    }

    static const sbe_uint32_t snMaxValue()
    {
        return 4294967293;
    }

    sbe_uint32_t sn(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 32)));
    }

    SDS_Level2 &sn(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 32)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int preCloseId(void)
    {
        return 7;
    }

    static const int preCloseSinceVersion(void)
    {
         return 0;
    }

    bool preCloseInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *PreCloseMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_uint32_t preCloseNullValue()
    {
        return 4294967294;
    }

    static const sbe_uint32_t preCloseMinValue()
    {
        return 0;
    }

    static const sbe_uint32_t preCloseMaxValue()
    {
        return 4294967293;
    }

    sbe_uint32_t preClose(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 36)));
    }

    SDS_Level2 &preClose(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 36)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int openId(void)
    {
        return 8;
    }

    static const int openSinceVersion(void)
    {
         return 0;
    }

    bool openInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *OpenMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_uint32_t openNullValue()
    {
        return 4294967294;
    }

    static const sbe_uint32_t openMinValue()
    {
        return 0;
    }

    static const sbe_uint32_t openMaxValue()
    {
        return 4294967293;
    }

    sbe_uint32_t open(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 40)));
    }

    SDS_Level2 &open(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 40)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int highId(void)
    {
        return 9;
    }

    static const int highSinceVersion(void)
    {
         return 0;
    }

    bool highInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *HighMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_uint32_t highNullValue()
    {
        return 4294967294;
    }

    static const sbe_uint32_t highMinValue()
    {
        return 0;
    }

    static const sbe_uint32_t highMaxValue()
    {
        return 4294967293;
    }

    sbe_uint32_t high(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 44)));
    }

    SDS_Level2 &high(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 44)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int lowId(void)
    {
        return 10;
    }

    static const int lowSinceVersion(void)
    {
         return 0;
    }

    bool lowInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *LowMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_uint32_t lowNullValue()
    {
        return 4294967294;
    }

    static const sbe_uint32_t lowMinValue()
    {
        return 0;
    }

    static const sbe_uint32_t lowMaxValue()
    {
        return 4294967293;
    }

    sbe_uint32_t low(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 48)));
    }

    SDS_Level2 &low(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 48)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int priceId(void)
    {
        return 11;
    }

    static const int priceSinceVersion(void)
    {
         return 0;
    }

    bool priceInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *PriceMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_uint32_t priceNullValue()
    {
        return 4294967294;
    }

    static const sbe_uint32_t priceMinValue()
    {
        return 0;
    }

    static const sbe_uint32_t priceMaxValue()
    {
        return 4294967293;
    }

    sbe_uint32_t price(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 52)));
    }

    SDS_Level2 &price(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 52)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int numTradesId(void)
    {
        return 12;
    }

    static const int numTradesSinceVersion(void)
    {
         return 0;
    }

    bool numTradesInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *NumTradesMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_uint32_t numTradesNullValue()
    {
        return 4294967294;
    }

    static const sbe_uint32_t numTradesMinValue()
    {
        return 0;
    }

    static const sbe_uint32_t numTradesMaxValue()
    {
        return 4294967293;
    }

    sbe_uint32_t numTrades(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 56)));
    }

    SDS_Level2 &numTrades(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 56)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int volumeId(void)
    {
        return 13;
    }

    static const int volumeSinceVersion(void)
    {
         return 0;
    }

    bool volumeInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *VolumeMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_uint64_t volumeNullValue()
    {
        return 0xffffffffffffffffL;
    }

    static const sbe_uint64_t volumeMinValue()
    {
        return 0x0L;
    }

    static const sbe_uint64_t volumeMaxValue()
    {
        return 0xfffffffffffffffeL;
    }

    sbe_uint64_t volume(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_64(*((sbe_uint64_t *)(buffer_ + offset_ + 60)));
    }

    SDS_Level2 &volume(const sbe_uint64_t value)
    {
        *((sbe_uint64_t *)(buffer_ + offset_ + 60)) = SBE_LITTLE_ENDIAN_ENCODE_64(value);
        return *this;
    }

    static const int turnoverId(void)
    {
        return 14;
    }

    static const int turnoverSinceVersion(void)
    {
         return 0;
    }

    bool turnoverInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *TurnoverMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_uint64_t turnoverNullValue()
    {
        return 0xffffffffffffffffL;
    }

    static const sbe_uint64_t turnoverMinValue()
    {
        return 0x0L;
    }

    static const sbe_uint64_t turnoverMaxValue()
    {
        return 0xfffffffffffffffeL;
    }

    sbe_uint64_t turnover(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_64(*((sbe_uint64_t *)(buffer_ + offset_ + 68)));
    }

    SDS_Level2 &turnover(const sbe_uint64_t value)
    {
        *((sbe_uint64_t *)(buffer_ + offset_ + 68)) = SBE_LITTLE_ENDIAN_ENCODE_64(value);
        return *this;
    }

    static const int totalBidVolId(void)
    {
        return 15;
    }

    static const int totalBidVolSinceVersion(void)
    {
         return 0;
    }

    bool totalBidVolInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *TotalBidVolMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_uint64_t totalBidVolNullValue()
    {
        return 0xffffffffffffffffL;
    }

    static const sbe_uint64_t totalBidVolMinValue()
    {
        return 0x0L;
    }

    static const sbe_uint64_t totalBidVolMaxValue()
    {
        return 0xfffffffffffffffeL;
    }

    sbe_uint64_t totalBidVol(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_64(*((sbe_uint64_t *)(buffer_ + offset_ + 76)));
    }

    SDS_Level2 &totalBidVol(const sbe_uint64_t value)
    {
        *((sbe_uint64_t *)(buffer_ + offset_ + 76)) = SBE_LITTLE_ENDIAN_ENCODE_64(value);
        return *this;
    }

    static const int totalAskVolId(void)
    {
        return 16;
    }

    static const int totalAskVolSinceVersion(void)
    {
         return 0;
    }

    bool totalAskVolInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *TotalAskVolMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_uint64_t totalAskVolNullValue()
    {
        return 0xffffffffffffffffL;
    }

    static const sbe_uint64_t totalAskVolMinValue()
    {
        return 0x0L;
    }

    static const sbe_uint64_t totalAskVolMaxValue()
    {
        return 0xfffffffffffffffeL;
    }

    sbe_uint64_t totalAskVol(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_64(*((sbe_uint64_t *)(buffer_ + offset_ + 84)));
    }

    SDS_Level2 &totalAskVol(const sbe_uint64_t value)
    {
        *((sbe_uint64_t *)(buffer_ + offset_ + 84)) = SBE_LITTLE_ENDIAN_ENCODE_64(value);
        return *this;
    }

    static const int avgBidPriceId(void)
    {
        return 17;
    }

    static const int avgBidPriceSinceVersion(void)
    {
         return 0;
    }

    bool avgBidPriceInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *AvgBidPriceMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_uint32_t avgBidPriceNullValue()
    {
        return 4294967294;
    }

    static const sbe_uint32_t avgBidPriceMinValue()
    {
        return 0;
    }

    static const sbe_uint32_t avgBidPriceMaxValue()
    {
        return 4294967293;
    }

    sbe_uint32_t avgBidPrice(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 92)));
    }

    SDS_Level2 &avgBidPrice(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 92)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int avgAskPriceId(void)
    {
        return 18;
    }

    static const int avgAskPriceSinceVersion(void)
    {
         return 0;
    }

    bool avgAskPriceInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *AvgAskPriceMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_uint32_t avgAskPriceNullValue()
    {
        return 4294967294;
    }

    static const sbe_uint32_t avgAskPriceMinValue()
    {
        return 0;
    }

    static const sbe_uint32_t avgAskPriceMaxValue()
    {
        return 4294967293;
    }

    sbe_uint32_t avgAskPrice(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 96)));
    }

    SDS_Level2 &avgAskPrice(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 96)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int highLimitedId(void)
    {
        return 19;
    }

    static const int highLimitedSinceVersion(void)
    {
         return 0;
    }

    bool highLimitedInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *HighLimitedMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_uint32_t highLimitedNullValue()
    {
        return 4294967294;
    }

    static const sbe_uint32_t highLimitedMinValue()
    {
        return 0;
    }

    static const sbe_uint32_t highLimitedMaxValue()
    {
        return 4294967293;
    }

    sbe_uint32_t highLimited(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 100)));
    }

    SDS_Level2 &highLimited(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 100)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int lowLimitedId(void)
    {
        return 20;
    }

    static const int lowLimitedSinceVersion(void)
    {
         return 0;
    }

    bool lowLimitedInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *LowLimitedMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_uint32_t lowLimitedNullValue()
    {
        return 4294967294;
    }

    static const sbe_uint32_t lowLimitedMinValue()
    {
        return 0;
    }

    static const sbe_uint32_t lowLimitedMaxValue()
    {
        return 4294967293;
    }

    sbe_uint32_t lowLimited(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 104)));
    }

    SDS_Level2 &lowLimited(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 104)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int askPriceId(void)
    {
        return 21;
    }

    static const int askPriceSinceVersion(void)
    {
         return 0;
    }

    bool askPriceInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *AskPriceMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_uint32_t askPriceNullValue()
    {
        return 4294967294;
    }

    static const sbe_uint32_t askPriceMinValue()
    {
        return 0;
    }

    static const sbe_uint32_t askPriceMaxValue()
    {
        return 4294967293;
    }

    static const int askPriceLength(void)
    {
        return 10;
    }

    const char *askPrice(void) const
    {
        return (buffer_ + offset_ + 108);
    }

    sbe_uint32_t askPrice(const int index) const
    {
        if (index < 0 || index >= 10)
        {
            throw "index out of range for askPrice";
        }

        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 108 + (index * 4))));
    }

    void askPrice(const int index, const sbe_uint32_t value)
    {
        if (index < 0 || index >= 10)
        {
            throw "index out of range for askPrice";
        }

        *((sbe_uint32_t *)(buffer_ + offset_ + 108 + (index * 4))) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
    }

    int getAskPrice(char *dst, const int length) const
    {
        if (length > 10)
        {
             throw "length too large for getAskPrice";
        }

        ::memcpy(dst, buffer_ + offset_ + 108, length);
        return length;
    }

    SDS_Level2 &putAskPrice(const char *src)
    {
        ::memcpy(buffer_ + offset_ + 108, src, 10);
        return *this;
    }

    static const int askVolId(void)
    {
        return 22;
    }

    static const int askVolSinceVersion(void)
    {
         return 0;
    }

    bool askVolInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *AskVolMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_uint32_t askVolNullValue()
    {
        return 4294967294;
    }

    static const sbe_uint32_t askVolMinValue()
    {
        return 0;
    }

    static const sbe_uint32_t askVolMaxValue()
    {
        return 4294967293;
    }

    static const int askVolLength(void)
    {
        return 10;
    }

    const char *askVol(void) const
    {
        return (buffer_ + offset_ + 148);
    }

    sbe_uint32_t askVol(const int index) const
    {
        if (index < 0 || index >= 10)
        {
            throw "index out of range for askVol";
        }

        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 148 + (index * 4))));
    }

    void askVol(const int index, const sbe_uint32_t value)
    {
        if (index < 0 || index >= 10)
        {
            throw "index out of range for askVol";
        }

        *((sbe_uint32_t *)(buffer_ + offset_ + 148 + (index * 4))) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
    }

    int getAskVol(char *dst, const int length) const
    {
        if (length > 10)
        {
             throw "length too large for getAskVol";
        }

        ::memcpy(dst, buffer_ + offset_ + 148, length);
        return length;
    }

    SDS_Level2 &putAskVol(const char *src)
    {
        ::memcpy(buffer_ + offset_ + 148, src, 10);
        return *this;
    }

    static const int bidPriceId(void)
    {
        return 23;
    }

    static const int bidPriceSinceVersion(void)
    {
         return 0;
    }

    bool bidPriceInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *BidPriceMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_uint32_t bidPriceNullValue()
    {
        return 4294967294;
    }

    static const sbe_uint32_t bidPriceMinValue()
    {
        return 0;
    }

    static const sbe_uint32_t bidPriceMaxValue()
    {
        return 4294967293;
    }

    static const int bidPriceLength(void)
    {
        return 10;
    }

    const char *bidPrice(void) const
    {
        return (buffer_ + offset_ + 188);
    }

    sbe_uint32_t bidPrice(const int index) const
    {
        if (index < 0 || index >= 10)
        {
            throw "index out of range for bidPrice";
        }

        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 188 + (index * 4))));
    }

    void bidPrice(const int index, const sbe_uint32_t value)
    {
        if (index < 0 || index >= 10)
        {
            throw "index out of range for bidPrice";
        }

        *((sbe_uint32_t *)(buffer_ + offset_ + 188 + (index * 4))) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
    }

    int getBidPrice(char *dst, const int length) const
    {
        if (length > 10)
        {
             throw "length too large for getBidPrice";
        }

        ::memcpy(dst, buffer_ + offset_ + 188, length);
        return length;
    }

    SDS_Level2 &putBidPrice(const char *src)
    {
        ::memcpy(buffer_ + offset_ + 188, src, 10);
        return *this;
    }

    static const int bidVolId(void)
    {
        return 24;
    }

    static const int bidVolSinceVersion(void)
    {
         return 0;
    }

    bool bidVolInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *BidVolMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_uint32_t bidVolNullValue()
    {
        return 4294967294;
    }

    static const sbe_uint32_t bidVolMinValue()
    {
        return 0;
    }

    static const sbe_uint32_t bidVolMaxValue()
    {
        return 4294967293;
    }

    static const int bidVolLength(void)
    {
        return 10;
    }

    const char *bidVol(void) const
    {
        return (buffer_ + offset_ + 228);
    }

    sbe_uint32_t bidVol(const int index) const
    {
        if (index < 0 || index >= 10)
        {
            throw "index out of range for bidVol";
        }

        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 228 + (index * 4))));
    }

    void bidVol(const int index, const sbe_uint32_t value)
    {
        if (index < 0 || index >= 10)
        {
            throw "index out of range for bidVol";
        }

        *((sbe_uint32_t *)(buffer_ + offset_ + 228 + (index * 4))) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
    }

    int getBidVol(char *dst, const int length) const
    {
        if (length > 10)
        {
             throw "length too large for getBidVol";
        }

        ::memcpy(dst, buffer_ + offset_ + 228, length);
        return length;
    }

    SDS_Level2 &putBidVol(const char *src)
    {
        ::memcpy(buffer_ + offset_ + 228, src, 10);
        return *this;
    }
};
}
#endif
