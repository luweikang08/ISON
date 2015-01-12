/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _SDS_ODERQUEEN_HPP_
#define _SDS_ODERQUEEN_HPP_

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

class SDS_OderQueen
{
private:
    char *buffer_;
    int bufferLength_;
    int *positionPtr_;
    int offset_;
    int position_;
    int actingBlockLength_;
    int actingVersion_;

    SDS_OderQueen(const SDS_OderQueen&) {}

public:

    SDS_OderQueen(void) : buffer_(NULL), bufferLength_(0), offset_(0) {}

    static const sbe_uint16_t sbeBlockLength(void)
    {
        return (sbe_uint16_t)249;
    }

    static const sbe_uint16_t sbeTemplateId(void)
    {
        return (sbe_uint16_t)4;
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

    SDS_OderQueen &wrapForEncode(char *buffer, const int offset, const int bufferLength)
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

    SDS_OderQueen &wrapForDecode(char *buffer, const int offset, const int actingBlockLength, const int actingVersion,
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

    SDS_OderQueen &seqno(const sbe_uint32_t value)
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

    SDS_OderQueen &putCode(const char *src)
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

    SDS_OderQueen &date(const sbe_uint32_t value)
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

    SDS_OderQueen &time(const sbe_uint32_t value)
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

    SDS_OderQueen &sDSTime(const sbe_uint32_t value)
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

    SDS_OderQueen &sn(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 32)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int sideId(void)
    {
        return 7;
    }

    static const int sideSinceVersion(void)
    {
         return 0;
    }

    bool sideInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *SideMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_char_t sideNullValue()
    {
        return (sbe_char_t)0;
    }

    static const sbe_char_t sideMinValue()
    {
        return (sbe_char_t)32;
    }

    static const sbe_char_t sideMaxValue()
    {
        return (sbe_char_t)126;
    }

    sbe_char_t side(void) const
    {
        return (*((sbe_char_t *)(buffer_ + offset_ + 36)));
    }

    SDS_OderQueen &side(const sbe_char_t value)
    {
        *((sbe_char_t *)(buffer_ + offset_ + 36)) = (value);
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
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 37)));
    }

    SDS_OderQueen &price(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 37)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int ordersId(void)
    {
        return 9;
    }

    static const int ordersSinceVersion(void)
    {
         return 0;
    }

    bool ordersInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *OrdersMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_uint32_t ordersNullValue()
    {
        return 4294967294;
    }

    static const sbe_uint32_t ordersMinValue()
    {
        return 0;
    }

    static const sbe_uint32_t ordersMaxValue()
    {
        return 4294967293;
    }

    sbe_uint32_t orders(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 41)));
    }

    SDS_OderQueen &orders(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 41)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int aBItemsId(void)
    {
        return 10;
    }

    static const int aBItemsSinceVersion(void)
    {
         return 0;
    }

    bool aBItemsInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *ABItemsMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_uint32_t aBItemsNullValue()
    {
        return 4294967294;
    }

    static const sbe_uint32_t aBItemsMinValue()
    {
        return 0;
    }

    static const sbe_uint32_t aBItemsMaxValue()
    {
        return 4294967293;
    }

    sbe_uint32_t aBItems(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 45)));
    }

    SDS_OderQueen &aBItems(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 45)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int aBVolumeId(void)
    {
        return 11;
    }

    static const int aBVolumeSinceVersion(void)
    {
         return 0;
    }

    bool aBVolumeInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *ABVolumeMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_uint32_t aBVolumeNullValue()
    {
        return 4294967294;
    }

    static const sbe_uint32_t aBVolumeMinValue()
    {
        return 0;
    }

    static const sbe_uint32_t aBVolumeMaxValue()
    {
        return 4294967293;
    }

    static const int aBVolumeLength(void)
    {
        return 50;
    }

    const char *aBVolume(void) const
    {
        return (buffer_ + offset_ + 49);
    }

    sbe_uint32_t aBVolume(const int index) const
    {
        if (index < 0 || index >= 50)
        {
            throw "index out of range for aBVolume";
        }

        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 49 + (index * 4))));
    }

    void aBVolume(const int index, const sbe_uint32_t value)
    {
        if (index < 0 || index >= 50)
        {
            throw "index out of range for aBVolume";
        }

        *((sbe_uint32_t *)(buffer_ + offset_ + 49 + (index * 4))) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
    }

    int getABVolume(char *dst, const int length) const
    {
        if (length > 50)
        {
             throw "length too large for getABVolume";
        }

        ::memcpy(dst, buffer_ + offset_ + 49, length);
        return length;
    }

    SDS_OderQueen &putABVolume(const char *src)
    {
        ::memcpy(buffer_ + offset_ + 49, src, 50);
        return *this;
    }
};
}
#endif
