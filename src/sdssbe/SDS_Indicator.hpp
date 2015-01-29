/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _SDS_INDICATOR_HPP_
#define _SDS_INDICATOR_HPP_

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

class SDS_Indicator
{
private:
    char *buffer_;
    int bufferLength_;
    int *positionPtr_;
    int offset_;
    int position_;
    int actingBlockLength_;
    int actingVersion_;

    SDS_Indicator(const SDS_Indicator&) {}

public:

    SDS_Indicator(void) : buffer_(NULL), bufferLength_(0), offset_(0) {}

    static const sbe_uint16_t sbeBlockLength(void)
    {
        return (sbe_uint16_t)65;
    }

    static const sbe_uint16_t sbeTemplateId(void)
    {
        return (sbe_uint16_t)10;
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

    SDS_Indicator &wrapForEncode(char *buffer, const int offset, const int bufferLength)
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

    SDS_Indicator &wrapForDecode(char *buffer, const int offset, const int actingBlockLength, const int actingVersion,
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

    static const int signalIDId(void)
    {
        return 1;
    }

    static const int signalIDSinceVersion(void)
    {
         return 0;
    }

    bool signalIDInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *SignalIDMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_uint8_t signalIDNullValue()
    {
        return (sbe_uint8_t)255;
    }

    static const sbe_uint8_t signalIDMinValue()
    {
        return (sbe_uint8_t)0;
    }

    static const sbe_uint8_t signalIDMaxValue()
    {
        return (sbe_uint8_t)254;
    }

    sbe_uint8_t signalID(void) const
    {
        return (*((sbe_uint8_t *)(buffer_ + offset_ + 0)));
    }

    SDS_Indicator &signalID(const sbe_uint8_t value)
    {
        *((sbe_uint8_t *)(buffer_ + offset_ + 0)) = (value);
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
        return (buffer_ + offset_ + 1);
    }

    sbe_char_t code(const int index) const
    {
        if (index < 0 || index >= 16)
        {
            throw "index out of range for code";
        }

        return (*((sbe_char_t *)(buffer_ + offset_ + 1 + (index * 1))));
    }

    void code(const int index, const sbe_char_t value)
    {
        if (index < 0 || index >= 16)
        {
            throw "index out of range for code";
        }

        *((sbe_char_t *)(buffer_ + offset_ + 1 + (index * 1))) = (value);
    }

    int getCode(char *dst, const int length) const
    {
        if (length > 16)
        {
             throw "length too large for getCode";
        }

        ::memcpy(dst, buffer_ + offset_ + 1, length);
        return length;
    }

    SDS_Indicator &putCode(const char *src)
    {
        ::memcpy(buffer_ + offset_ + 1, src, 16);
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
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 17)));
    }

    SDS_Indicator &date(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 17)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
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
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 21)));
    }

    SDS_Indicator &time(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 21)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int dataFstId(void)
    {
        return 5;
    }

    static const int dataFstSinceVersion(void)
    {
         return 0;
    }

    bool dataFstInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *dataFstMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_double_t dataFstNullValue()
    {
        return SBE_DOUBLE_NAN;
    }

    static const sbe_double_t dataFstMinValue()
    {
        return 4.9E-324;
    }

    static const sbe_double_t dataFstMaxValue()
    {
        return 1.7976931348623157E308;
    }

    sbe_double_t dataFst(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_64(*((sbe_double_t *)(buffer_ + offset_ + 25)));
    }

    SDS_Indicator &dataFst(const sbe_double_t value)
    {
        *((sbe_double_t *)(buffer_ + offset_ + 25)) = SBE_LITTLE_ENDIAN_ENCODE_64(value);
        return *this;
    }

    static const int dataSecId(void)
    {
        return 6;
    }

    static const int dataSecSinceVersion(void)
    {
         return 0;
    }

    bool dataSecInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *dataSecMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_double_t dataSecNullValue()
    {
        return SBE_DOUBLE_NAN;
    }

    static const sbe_double_t dataSecMinValue()
    {
        return 4.9E-324;
    }

    static const sbe_double_t dataSecMaxValue()
    {
        return 1.7976931348623157E308;
    }

    sbe_double_t dataSec(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_64(*((sbe_double_t *)(buffer_ + offset_ + 33)));
    }

    SDS_Indicator &dataSec(const sbe_double_t value)
    {
        *((sbe_double_t *)(buffer_ + offset_ + 33)) = SBE_LITTLE_ENDIAN_ENCODE_64(value);
        return *this;
    }

    static const int dataTrdId(void)
    {
        return 7;
    }

    static const int dataTrdSinceVersion(void)
    {
         return 0;
    }

    bool dataTrdInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *dataTrdMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_double_t dataTrdNullValue()
    {
        return SBE_DOUBLE_NAN;
    }

    static const sbe_double_t dataTrdMinValue()
    {
        return 4.9E-324;
    }

    static const sbe_double_t dataTrdMaxValue()
    {
        return 1.7976931348623157E308;
    }

    sbe_double_t dataTrd(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_64(*((sbe_double_t *)(buffer_ + offset_ + 41)));
    }

    SDS_Indicator &dataTrd(const sbe_double_t value)
    {
        *((sbe_double_t *)(buffer_ + offset_ + 41)) = SBE_LITTLE_ENDIAN_ENCODE_64(value);
        return *this;
    }

    static const int dataFouId(void)
    {
        return 8;
    }

    static const int dataFouSinceVersion(void)
    {
         return 0;
    }

    bool dataFouInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *dataFouMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_double_t dataFouNullValue()
    {
        return SBE_DOUBLE_NAN;
    }

    static const sbe_double_t dataFouMinValue()
    {
        return 4.9E-324;
    }

    static const sbe_double_t dataFouMaxValue()
    {
        return 1.7976931348623157E308;
    }

    sbe_double_t dataFou(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_64(*((sbe_double_t *)(buffer_ + offset_ + 49)));
    }

    SDS_Indicator &dataFou(const sbe_double_t value)
    {
        *((sbe_double_t *)(buffer_ + offset_ + 49)) = SBE_LITTLE_ENDIAN_ENCODE_64(value);
        return *this;
    }

    static const int dataFivId(void)
    {
        return 9;
    }

    static const int dataFivSinceVersion(void)
    {
         return 0;
    }

    bool dataFivInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *dataFivMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_double_t dataFivNullValue()
    {
        return SBE_DOUBLE_NAN;
    }

    static const sbe_double_t dataFivMinValue()
    {
        return 4.9E-324;
    }

    static const sbe_double_t dataFivMaxValue()
    {
        return 1.7976931348623157E308;
    }

    sbe_double_t dataFiv(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_64(*((sbe_double_t *)(buffer_ + offset_ + 57)));
    }

    SDS_Indicator &dataFiv(const sbe_double_t value)
    {
        *((sbe_double_t *)(buffer_ + offset_ + 57)) = SBE_LITTLE_ENDIAN_ENCODE_64(value);
        return *this;
    }
};
}
#endif
