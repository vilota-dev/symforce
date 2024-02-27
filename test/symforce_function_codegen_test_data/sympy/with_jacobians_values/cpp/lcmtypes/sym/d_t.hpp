/** THIS IS AN AUTOMATICALLY GENERATED FILE.  DO NOT MODIFY
 * BY HAND!!
 *
 * Generated by lcm-gen
 **/

#include <lcm/lcm_coretypes.h>
#include <lcm/lcm_cpptypes.hpp>

#ifndef __sym_d_t_hpp__
#define __sym_d_t_hpp__

#if defined(SKYMARSHAL_PRINTING_ENABLED)
#include <lcm/lcm_reflection.hpp>
#include <array>
#endif
#include <ostream>

#include "lcmtypes/eigen_lcm/Vector2d.hpp"

namespace sym
{

class d_t
{
    public:
        double x;

        ::eigen_lcm::Vector2d y;

    public:
        d_t() = default;

        /**
         * Member constructor
         */
        inline d_t(
            const double& x_arg,
            const ::eigen_lcm::Vector2d& y_arg
        );

        /**
         * Encode a message into binary form.
         *
         * @param buf The output buffer.
         * @param offset Encoding starts at thie byte offset into @p buf.
         * @param maxlen Maximum number of bytes to write.  This should generally be
         *  equal to getEncodedSize().
         * @return The number of bytes encoded, or <0 on error.
         */
        inline __lcm_buffer_size encode(void *buf, __lcm_buffer_size offset, __lcm_buffer_size maxlen) const;

        /**
         * Check how many bytes are required to encode this message.
         */
        inline __lcm_buffer_size getEncodedSize() const;

        /**
         * Decode a message from binary form into this instance.
         *
         * @param buf The buffer containing the encoded message.
         * @param offset The byte offset into @p buf where the encoded message starts.
         * @param maxlen The maximum number of bytes to read while decoding.
         * @return The number of bytes decoded, or <0 if an error occured.
         */
        inline __lcm_buffer_size decode(const void *buf, __lcm_buffer_size offset, __lcm_buffer_size maxlen);

        /**
         * Retrieve the 64-bit fingerprint identifying the structure of the message.
         * Note that the fingerprint is the same for all instances of the same
         * message type, and is a fingerprint on the message type definition, not on
         * the message contents.
         */
        inline static int64_t getHash();

        using type_name_array_t = const char[4];

        inline static constexpr type_name_array_t* getTypeNameArrayPtr();

        /**
         * Returns "d_t"
         */
        inline static constexpr const char* getTypeName();

        /**
         * Returns "sym"
         */
        inline static constexpr const char * getPackageName();

        // LCM support functions. Users should not call these
        inline __lcm_buffer_size _encodeNoHash(void *buf, __lcm_buffer_size offset, __lcm_buffer_size maxlen) const;
        inline __lcm_buffer_size _getEncodedSizeNoHash() const;
        inline __lcm_buffer_size _decodeNoHash(const void *buf, __lcm_buffer_size offset, __lcm_buffer_size maxlen);
#if !defined(SKYDIO_DISABLE_LCM_NO_INLINE)
        __attribute__((noinline))
#endif
        static uint64_t _computeHash(const __lcm_hash_ptr *p)
        {
            const __lcm_hash_ptr *fp;
            for(fp = p; fp != NULL; fp = fp->parent)
                if(fp->v == d_t::getHash)
                    return 0;
            const __lcm_hash_ptr cp = { p, d_t::getHash };

            uint64_t hash = 0x21f0a3980ff1bd27LL +
                ::eigen_lcm::Vector2d::_computeHash(&cp);

            return (hash<<1) + ((hash>>63)&1);
        }

        // Comparison operators.
        inline bool operator==(const d_t& other) const;
        inline bool operator!=(const d_t& other) const;

#if defined(SKYMARSHAL_PRINTING_ENABLED)
        constexpr static std::array<const char*, 2> fields()
        {
            return {{
                "x",
                "y",
            }};
        }

        // Return true if field was found
        bool format_field(std::ostream& _stream, uint16_t field_index, uint16_t _indent) const
        {
            switch (field_index) {
                case 0:
                lcm::format_json(_stream, x, _indent);
                return true;
                case 1:
                lcm::format_json(_stream, y, _indent);
                return true;
                default:
                return false;
            }
        }

        // Ability to print to standard streams as well as the fmt library.
        friend std::ostream& operator<<(std::ostream& _stream, const d_t& obj) {
            lcm::format_json(_stream, obj, 0);
            return _stream;
        }

#else
        friend std::ostream& operator<<(std::ostream& _stream, const d_t& obj) {
            _stream << "<FORMATTING DISABLED>";
            return _stream;
        }
#endif

#if defined(SKYMARSHAL_STORING_ENABLED)
        // Return value is 0 if the operation succeeded.
        // If the operation failed, return value is equal to 1 + the index of the first invalid field,
        // 1 + field_size if there are not enough fields, or 2 + field_size if the value is invalid.
        __attribute__((nodiscard)) uint16_t store_field(const char* _fields[], uint16_t _num_fields, const char* _value)
        {
            if (_num_fields == 0 || _fields[0] == nullptr) {
                return 1;
            }
            if (strcmp(_fields[0], fields()[0]) == 0) {
                uint16_t ret = lcm::store_field(_fields + 1, _num_fields - 1, x, _value);
                return ret == 0 ? ret : ret + 1;
            }
            if (strcmp(_fields[0], fields()[1]) == 0) {
                uint16_t ret = lcm::store_field(_fields + 1, _num_fields - 1, y, _value);
                return ret == 0 ? ret : ret + 1;
            }
            return 1;
        }
#endif
};

d_t::d_t(
    const double& x_arg,
    const ::eigen_lcm::Vector2d& y_arg
) : x(x_arg),
    y(y_arg) {}

__lcm_buffer_size d_t::encode(void *buf, __lcm_buffer_size offset, __lcm_buffer_size maxlen) const
{
    __lcm_buffer_size pos = 0, tlen;
    int64_t hash = (int64_t)getHash();

    tlen = __int64_t_encode_array(buf, offset + pos, maxlen - pos, &hash, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = this->_encodeNoHash(buf, offset + pos, maxlen - pos);
    if (tlen < 0) return tlen; else pos += tlen;

    return pos;
}

__lcm_buffer_size d_t::decode(const void *buf, __lcm_buffer_size offset, __lcm_buffer_size maxlen)
{
    __lcm_buffer_size pos = 0, thislen;

    int64_t msg_hash;
    thislen = __int64_t_decode_array(buf, offset + pos, maxlen - pos, &msg_hash, 1);
    if (thislen < 0) return thislen; else pos += thislen;
    if (msg_hash != getHash()) return -1;

    thislen = this->_decodeNoHash(buf, offset + pos, maxlen - pos);
    if (thislen < 0) return thislen; else pos += thislen;

    return pos;
}

__lcm_buffer_size d_t::getEncodedSize() const
{
    return 8 + _getEncodedSizeNoHash();
}

int64_t d_t::getHash()
{
    static int64_t hash = _computeHash(NULL);
    return hash;
}

constexpr d_t::type_name_array_t* d_t::getTypeNameArrayPtr() {
    return &"d_t";
}

constexpr const char* d_t::getTypeName()
{
    return *d_t::getTypeNameArrayPtr();
}

constexpr const char * d_t::getPackageName()
{
    return "sym";
}

__lcm_buffer_size d_t::_encodeNoHash(void *buf, __lcm_buffer_size offset, __lcm_buffer_size maxlen) const
{
    __lcm_buffer_size pos = 0, tlen;

    tlen = __double_encode_array(buf, offset + pos, maxlen - pos, &this->x, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = this->y._encodeNoHash(buf, offset + pos, maxlen - pos);
    if(tlen < 0) return tlen; else pos += tlen;

    return pos;
}

__lcm_buffer_size d_t::_decodeNoHash(const void *buf, __lcm_buffer_size offset, __lcm_buffer_size maxlen)
{
    __lcm_buffer_size pos = 0, tlen;

    tlen = __double_decode_array(buf, offset + pos, maxlen - pos, &this->x, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = this->y._decodeNoHash(buf, offset + pos, maxlen - pos);
    if(tlen < 0) return tlen; else pos += tlen;

    return pos;
}

__lcm_buffer_size d_t::_getEncodedSizeNoHash() const
{
    __lcm_buffer_size enc_size = 0;
    enc_size += __double_encoded_array_size(NULL, 1);
    enc_size += this->y._getEncodedSizeNoHash();
    return enc_size;
}

bool d_t::operator==(const d_t& other) const {
  return (
          (x==other.x) && 
          (y==other.y));
}

bool d_t::operator!=(const d_t& other) const {
  return !(*this==other);
}

}

#endif