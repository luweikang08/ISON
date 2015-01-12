/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _SDS_CLOSEMARKET_HPP_
#define _SDS_CLOSEMARKET_HPP_

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

class SDS_CloseMarket
{
private:
    char *buffer_;
    int bufferLength_;
    int *positionPtr_;
    int offset_;
    int position_;
    int actingBlockLength_;
    int actingVersion_;

    SDS_CloseMarket(const SDS_CloseMarket&) {}

public:

    SDS_CloseMarket(void) : buffer_(NULL), bufferLength_(0), offset_(0) {}

    static const sbe_uint16_t sbeBlockLength(void)
    {
        return (sbe_uint16_t)14;
    }

    static const sbe_uint16_t sbeTemplateId(void)
    {
        return (sbe_uint16_t)6;
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

    SDS_CloseMarket &wrapForEncode(char *buffer, const int offset, const int bufferLength)
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

    SDS_CloseMarket &wrapForDecode(char *buffer, const int offset, const int actingBlockLength, const int actingVersion,
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

    SDS_CloseMarket &seqno(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 0)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int timeId(void)
    {
        return 2;
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
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 4)));
    }

    SDS_CloseMarket &time(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 4)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int marketId(void)
    {
        return 3;
    }

    static const int marketSinceVersion(void)
    {
         return 0;
    }

    bool marketInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *MarketMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_char_t marketNullValue()
    {
        return (sbe_char_t)0;
    }

    static const sbe_char_t marketMinValue()
    {
        return (sbe_char_t)32;
    }

    static const sbe_char_t marketMaxValue()
    {
        return (sbe_char_t)126;
    }

    static const int marketLength(void)
    {
        return 6;
    }

    const char *market(void) const
    {
        return (buffer_ + offset_ + 8);
    }

    sbe_char_t market(const int index) const
    {
        if (index < 0 || index >= 6)
        {
            throw "index out of range for market";
        }

        return (*((sbe_char_t *)(buffer_ + offset_ + 8 + (index * 1))));
    }

    void market(const int index, const sbe_char_t value)
    {
        if (index < 0 || index >= 6)
        {
            throw "index out of range for market";
        }

        *((sbe_char_t *)(buffer_ + offset_ + 8 + (index * 1))) = (value);
    }

    int getMarket(char *dst, const int length) const
    {
        if (length > 6)
        {
             throw "length too large for getMarket";
        }

        ::memcpy(dst, buffer_ + offset_ + 8, length);
        return length;
    }

    SDS_CloseMarket &putMarket(const char *src)
    {
        ::memcpy(buffer_ + offset_ + 8, src, 6);
        return *this;
    }
};
}
#endif
