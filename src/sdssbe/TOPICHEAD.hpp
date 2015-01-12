/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _TOPICHEAD_HPP_
#define _TOPICHEAD_HPP_

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

class TOPICHEAD
{
private:
    char *buffer_;
    int bufferLength_;
    int *positionPtr_;
    int offset_;
    int position_;
    int actingBlockLength_;
    int actingVersion_;

    TOPICHEAD(const TOPICHEAD&) {}

public:

    TOPICHEAD(void) : buffer_(NULL), bufferLength_(0), offset_(0) {}

    static const sbe_uint16_t sbeBlockLength(void)
    {
        return (sbe_uint16_t)16;
    }

    static const sbe_uint16_t sbeTemplateId(void)
    {
        return (sbe_uint16_t)7;
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

    TOPICHEAD &wrapForEncode(char *buffer, const int offset, const int bufferLength)
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

    TOPICHEAD &wrapForDecode(char *buffer, const int offset, const int actingBlockLength, const int actingVersion,
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

    static const int topicId(void)
    {
        return 1;
    }

    static const int topicSinceVersion(void)
    {
         return 0;
    }

    bool topicInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *topicMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_uint16_t topicNullValue()
    {
        return (sbe_uint16_t)65535;
    }

    static const sbe_uint16_t topicMinValue()
    {
        return (sbe_uint16_t)0;
    }

    static const sbe_uint16_t topicMaxValue()
    {
        return (sbe_uint16_t)65534;
    }

    sbe_uint16_t topic(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_16(*((sbe_uint16_t *)(buffer_ + offset_ + 0)));
    }

    TOPICHEAD &topic(const sbe_uint16_t value)
    {
        *((sbe_uint16_t *)(buffer_ + offset_ + 0)) = SBE_LITTLE_ENDIAN_ENCODE_16(value);
        return *this;
    }

    static const int msId(void)
    {
        return 2;
    }

    static const int msSinceVersion(void)
    {
         return 0;
    }

    bool msInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *msMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_uint16_t msNullValue()
    {
        return (sbe_uint16_t)65535;
    }

    static const sbe_uint16_t msMinValue()
    {
        return (sbe_uint16_t)0;
    }

    static const sbe_uint16_t msMaxValue()
    {
        return (sbe_uint16_t)65534;
    }

    sbe_uint16_t ms(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_16(*((sbe_uint16_t *)(buffer_ + offset_ + 2)));
    }

    TOPICHEAD &ms(const sbe_uint16_t value)
    {
        *((sbe_uint16_t *)(buffer_ + offset_ + 2)) = SBE_LITTLE_ENDIAN_ENCODE_16(value);
        return *this;
    }

    static const int kwId(void)
    {
        return 3;
    }

    static const int kwSinceVersion(void)
    {
         return 0;
    }

    bool kwInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *kwMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_uint32_t kwNullValue()
    {
        return 4294967294;
    }

    static const sbe_uint32_t kwMinValue()
    {
        return 0;
    }

    static const sbe_uint32_t kwMaxValue()
    {
        return 4294967293;
    }

    sbe_uint32_t kw(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 4)));
    }

    TOPICHEAD &kw(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 4)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int snId(void)
    {
        return 4;
    }

    static const int snSinceVersion(void)
    {
         return 0;
    }

    bool snInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *snMetaAttribute(const MetaAttribute::Attribute metaAttribute)
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
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 8)));
    }

    TOPICHEAD &sn(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 8)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int tmId(void)
    {
        return 5;
    }

    static const int tmSinceVersion(void)
    {
         return 0;
    }

    bool tmInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *tmMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_uint32_t tmNullValue()
    {
        return 4294967294;
    }

    static const sbe_uint32_t tmMinValue()
    {
        return 0;
    }

    static const sbe_uint32_t tmMaxValue()
    {
        return 4294967293;
    }

    sbe_uint32_t tm(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 12)));
    }

    TOPICHEAD &tm(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 12)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }
};
}
#endif
