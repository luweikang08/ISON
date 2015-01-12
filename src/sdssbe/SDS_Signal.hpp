/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _SDS_SIGNAL_HPP_
#define _SDS_SIGNAL_HPP_

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

class SDS_Signal
{
private:
    char *buffer_;
    int bufferLength_;
    int *positionPtr_;
    int offset_;
    int position_;
    int actingBlockLength_;
    int actingVersion_;

    SDS_Signal(const SDS_Signal&) {}

public:

    SDS_Signal(void) : buffer_(NULL), bufferLength_(0), offset_(0) {}

    static const sbe_uint16_t sbeBlockLength(void)
    {
        return (sbe_uint16_t)75;
    }

    static const sbe_uint16_t sbeTemplateId(void)
    {
        return (sbe_uint16_t)8;
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

    SDS_Signal &wrapForEncode(char *buffer, const int offset, const int bufferLength)
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

    SDS_Signal &wrapForDecode(char *buffer, const int offset, const int actingBlockLength, const int actingVersion,
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

    SDS_Signal &signalID(const sbe_uint8_t value)
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

    SDS_Signal &putCode(const char *src)
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

    SDS_Signal &date(const sbe_uint32_t value)
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

    SDS_Signal &time(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 21)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int infoId(void)
    {
        return 5;
    }

    static const int infoSinceVersion(void)
    {
         return 0;
    }

    bool infoInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *InfoMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_char_t infoNullValue()
    {
        return (sbe_char_t)0;
    }

    static const sbe_char_t infoMinValue()
    {
        return (sbe_char_t)32;
    }

    static const sbe_char_t infoMaxValue()
    {
        return (sbe_char_t)126;
    }

    static const int infoLength(void)
    {
        return 50;
    }

    const char *info(void) const
    {
        return (buffer_ + offset_ + 25);
    }

    sbe_char_t info(const int index) const
    {
        if (index < 0 || index >= 50)
        {
            throw "index out of range for info";
        }

        return (*((sbe_char_t *)(buffer_ + offset_ + 25 + (index * 1))));
    }

    void info(const int index, const sbe_char_t value)
    {
        if (index < 0 || index >= 50)
        {
            throw "index out of range for info";
        }

        *((sbe_char_t *)(buffer_ + offset_ + 25 + (index * 1))) = (value);
    }

    int getInfo(char *dst, const int length) const
    {
        if (length > 50)
        {
             throw "length too large for getInfo";
        }

        ::memcpy(dst, buffer_ + offset_ + 25, length);
        return length;
    }

    SDS_Signal &putInfo(const char *src)
    {
        ::memcpy(buffer_ + offset_ + 25, src, 50);
        return *this;
    }
};
}
#endif
