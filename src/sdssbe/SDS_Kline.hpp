/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _SDS_KLINE_HPP_
#define _SDS_KLINE_HPP_

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

class SDS_Kline
{
private:
    char *buffer_;
    int bufferLength_;
    int *positionPtr_;
    int offset_;
    int position_;
    int actingBlockLength_;
    int actingVersion_;

    SDS_Kline(const SDS_Kline&) {}

public:

    SDS_Kline(void) : buffer_(NULL), bufferLength_(0), offset_(0) {}

    static const sbe_uint16_t sbeBlockLength(void)
    {
        return (sbe_uint16_t)61;
    }

    static const sbe_uint16_t sbeTemplateId(void)
    {
        return (sbe_uint16_t)9;
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

    SDS_Kline &wrapForEncode(char *buffer, const int offset, const int bufferLength)
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

    SDS_Kline &wrapForDecode(char *buffer, const int offset, const int actingBlockLength, const int actingVersion,
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

    static const int codeId(void)
    {
        return 1;
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
        return (buffer_ + offset_ + 0);
    }

    sbe_char_t code(const int index) const
    {
        if (index < 0 || index >= 16)
        {
            throw "index out of range for code";
        }

        return (*((sbe_char_t *)(buffer_ + offset_ + 0 + (index * 1))));
    }

    void code(const int index, const sbe_char_t value)
    {
        if (index < 0 || index >= 16)
        {
            throw "index out of range for code";
        }

        *((sbe_char_t *)(buffer_ + offset_ + 0 + (index * 1))) = (value);
    }

    int getCode(char *dst, const int length) const
    {
        if (length > 16)
        {
             throw "length too large for getCode";
        }

        ::memcpy(dst, buffer_ + offset_ + 0, length);
        return length;
    }

    SDS_Kline &putCode(const char *src)
    {
        ::memcpy(buffer_ + offset_ + 0, src, 16);
        return *this;
    }

    static const int dateId(void)
    {
        return 2;
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
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 16)));
    }

    SDS_Kline &date(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 16)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int timeId(void)
    {
        return 3;
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
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 20)));
    }

    SDS_Kline &time(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 20)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int timeStatusId(void)
    {
        return 4;
    }

    static const int timeStatusSinceVersion(void)
    {
         return 0;
    }

    bool timeStatusInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *TimeStatusMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_uint8_t timeStatusNullValue()
    {
        return (sbe_uint8_t)255;
    }

    static const sbe_uint8_t timeStatusMinValue()
    {
        return (sbe_uint8_t)0;
    }

    static const sbe_uint8_t timeStatusMaxValue()
    {
        return (sbe_uint8_t)254;
    }

    sbe_uint8_t timeStatus(void) const
    {
        return (*((sbe_uint8_t *)(buffer_ + offset_ + 24)));
    }

    SDS_Kline &timeStatus(const sbe_uint8_t value)
    {
        *((sbe_uint8_t *)(buffer_ + offset_ + 24)) = (value);
        return *this;
    }

    static const int preCloseId(void)
    {
        return 5;
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
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 25)));
    }

    SDS_Kline &preClose(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 25)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int openId(void)
    {
        return 6;
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
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 29)));
    }

    SDS_Kline &open(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 29)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int closeId(void)
    {
        return 7;
    }

    static const int closeSinceVersion(void)
    {
         return 0;
    }

    bool closeInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *CloseMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_uint32_t closeNullValue()
    {
        return 4294967294;
    }

    static const sbe_uint32_t closeMinValue()
    {
        return 0;
    }

    static const sbe_uint32_t closeMaxValue()
    {
        return 4294967293;
    }

    sbe_uint32_t close(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 33)));
    }

    SDS_Kline &close(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 33)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int highId(void)
    {
        return 8;
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
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 37)));
    }

    SDS_Kline &high(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 37)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int lowId(void)
    {
        return 9;
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
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 41)));
    }

    SDS_Kline &low(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 41)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int volumeId(void)
    {
        return 10;
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
        return SBE_LITTLE_ENDIAN_ENCODE_64(*((sbe_uint64_t *)(buffer_ + offset_ + 45)));
    }

    SDS_Kline &volume(const sbe_uint64_t value)
    {
        *((sbe_uint64_t *)(buffer_ + offset_ + 45)) = SBE_LITTLE_ENDIAN_ENCODE_64(value);
        return *this;
    }

    static const int turnoverId(void)
    {
        return 11;
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
        return SBE_LITTLE_ENDIAN_ENCODE_64(*((sbe_uint64_t *)(buffer_ + offset_ + 53)));
    }

    SDS_Kline &turnover(const sbe_uint64_t value)
    {
        *((sbe_uint64_t *)(buffer_ + offset_ + 53)) = SBE_LITTLE_ENDIAN_ENCODE_64(value);
        return *this;
    }
};
}
#endif
