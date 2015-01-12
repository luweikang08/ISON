/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _SDS_ORDER_HPP_
#define _SDS_ORDER_HPP_

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

class SDS_Order
{
private:
    char *buffer_;
    int bufferLength_;
    int *positionPtr_;
    int offset_;
    int position_;
    int actingBlockLength_;
    int actingVersion_;

    SDS_Order(const SDS_Order&) {}

public:

    SDS_Order(void) : buffer_(NULL), bufferLength_(0), offset_(0) {}

    static const sbe_uint16_t sbeBlockLength(void)
    {
        return (sbe_uint16_t)50;
    }

    static const sbe_uint16_t sbeTemplateId(void)
    {
        return (sbe_uint16_t)3;
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

    SDS_Order &wrapForEncode(char *buffer, const int offset, const int bufferLength)
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

    SDS_Order &wrapForDecode(char *buffer, const int offset, const int actingBlockLength, const int actingVersion,
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

    SDS_Order &seqno(const sbe_uint32_t value)
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

    SDS_Order &putCode(const char *src)
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

    SDS_Order &date(const sbe_uint32_t value)
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

    SDS_Order &time(const sbe_uint32_t value)
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

    SDS_Order &sDSTime(const sbe_uint32_t value)
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

    SDS_Order &sn(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 32)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int orderId(void)
    {
        return 7;
    }

    static const int orderSinceVersion(void)
    {
         return 0;
    }

    bool orderInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *OrderMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_uint32_t orderNullValue()
    {
        return 4294967294;
    }

    static const sbe_uint32_t orderMinValue()
    {
        return 0;
    }

    static const sbe_uint32_t orderMaxValue()
    {
        return 4294967293;
    }

    sbe_uint32_t order(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 36)));
    }

    SDS_Order &order(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 36)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int priceId(void)
    {
        return 8;
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
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 40)));
    }

    SDS_Order &price(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 40)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int volumeId(void)
    {
        return 9;
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

    static const sbe_uint32_t volumeNullValue()
    {
        return 4294967294;
    }

    static const sbe_uint32_t volumeMinValue()
    {
        return 0;
    }

    static const sbe_uint32_t volumeMaxValue()
    {
        return 4294967293;
    }

    sbe_uint32_t volume(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 44)));
    }

    SDS_Order &volume(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 44)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int orderKindId(void)
    {
        return 10;
    }

    static const int orderKindSinceVersion(void)
    {
         return 0;
    }

    bool orderKindInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *OrderKindMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_char_t orderKindNullValue()
    {
        return (sbe_char_t)0;
    }

    static const sbe_char_t orderKindMinValue()
    {
        return (sbe_char_t)32;
    }

    static const sbe_char_t orderKindMaxValue()
    {
        return (sbe_char_t)126;
    }

    sbe_char_t orderKind(void) const
    {
        return (*((sbe_char_t *)(buffer_ + offset_ + 48)));
    }

    SDS_Order &orderKind(const sbe_char_t value)
    {
        *((sbe_char_t *)(buffer_ + offset_ + 48)) = (value);
        return *this;
    }

    static const int functionCodeId(void)
    {
        return 11;
    }

    static const int functionCodeSinceVersion(void)
    {
         return 0;
    }

    bool functionCodeInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *FunctionCodeMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_char_t functionCodeNullValue()
    {
        return (sbe_char_t)0;
    }

    static const sbe_char_t functionCodeMinValue()
    {
        return (sbe_char_t)32;
    }

    static const sbe_char_t functionCodeMaxValue()
    {
        return (sbe_char_t)126;
    }

    sbe_char_t functionCode(void) const
    {
        return (*((sbe_char_t *)(buffer_ + offset_ + 49)));
    }

    SDS_Order &functionCode(const sbe_char_t value)
    {
        *((sbe_char_t *)(buffer_ + offset_ + 49)) = (value);
        return *this;
    }
};
}
#endif
