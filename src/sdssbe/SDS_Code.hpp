/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _SDS_CODE_HPP_
#define _SDS_CODE_HPP_

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

class SDS_Code
{
private:
    char *buffer_;
    int bufferLength_;
    int *positionPtr_;
    int offset_;
    int position_;
    int actingBlockLength_;
    int actingVersion_;

    SDS_Code(const SDS_Code&) {}

public:

    SDS_Code(void) : buffer_(NULL), bufferLength_(0), offset_(0) {}

    static const sbe_uint16_t sbeBlockLength(void)
    {
        return (sbe_uint16_t)80;
    }

    static const sbe_uint16_t sbeTemplateId(void)
    {
        return (sbe_uint16_t)5;
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

    SDS_Code &wrapForEncode(char *buffer, const int offset, const int bufferLength)
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

    SDS_Code &wrapForDecode(char *buffer, const int offset, const int actingBlockLength, const int actingVersion,
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

    SDS_Code &putCode(const char *src)
    {
        ::memcpy(buffer_ + offset_ + 0, src, 16);
        return *this;
    }

    static const int eNNameId(void)
    {
        return 2;
    }

    static const int eNNameSinceVersion(void)
    {
         return 0;
    }

    bool eNNameInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *ENNameMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_char_t eNNameNullValue()
    {
        return (sbe_char_t)0;
    }

    static const sbe_char_t eNNameMinValue()
    {
        return (sbe_char_t)32;
    }

    static const sbe_char_t eNNameMaxValue()
    {
        return (sbe_char_t)126;
    }

    static const int eNNameLength(void)
    {
        return 32;
    }

    const char *eNName(void) const
    {
        return (buffer_ + offset_ + 16);
    }

    sbe_char_t eNName(const int index) const
    {
        if (index < 0 || index >= 32)
        {
            throw "index out of range for eNName";
        }

        return (*((sbe_char_t *)(buffer_ + offset_ + 16 + (index * 1))));
    }

    void eNName(const int index, const sbe_char_t value)
    {
        if (index < 0 || index >= 32)
        {
            throw "index out of range for eNName";
        }

        *((sbe_char_t *)(buffer_ + offset_ + 16 + (index * 1))) = (value);
    }

    int getENName(char *dst, const int length) const
    {
        if (length > 32)
        {
             throw "length too large for getENName";
        }

        ::memcpy(dst, buffer_ + offset_ + 16, length);
        return length;
    }

    SDS_Code &putENName(const char *src)
    {
        ::memcpy(buffer_ + offset_ + 16, src, 32);
        return *this;
    }

    static const int cNNameId(void)
    {
        return 3;
    }

    static const int cNNameSinceVersion(void)
    {
         return 0;
    }

    bool cNNameInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *CNNameMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const sbe_char_t cNNameNullValue()
    {
        return (sbe_char_t)0;
    }

    static const sbe_char_t cNNameMinValue()
    {
        return (sbe_char_t)32;
    }

    static const sbe_char_t cNNameMaxValue()
    {
        return (sbe_char_t)126;
    }

    static const int cNNameLength(void)
    {
        return 32;
    }

    const char *cNName(void) const
    {
        return (buffer_ + offset_ + 48);
    }

    sbe_char_t cNName(const int index) const
    {
        if (index < 0 || index >= 32)
        {
            throw "index out of range for cNName";
        }

        return (*((sbe_char_t *)(buffer_ + offset_ + 48 + (index * 1))));
    }

    void cNName(const int index, const sbe_char_t value)
    {
        if (index < 0 || index >= 32)
        {
            throw "index out of range for cNName";
        }

        *((sbe_char_t *)(buffer_ + offset_ + 48 + (index * 1))) = (value);
    }

    int getCNName(char *dst, const int length) const
    {
        if (length > 32)
        {
             throw "length too large for getCNName";
        }

        ::memcpy(dst, buffer_ + offset_ + 48, length);
        return length;
    }

    SDS_Code &putCNName(const char *src)
    {
        ::memcpy(buffer_ + offset_ + 48, src, 32);
        return *this;
    }
};
}
#endif
