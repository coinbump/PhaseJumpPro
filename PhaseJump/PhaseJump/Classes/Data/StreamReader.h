#pragma once

#include <bit>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/4/25
 */
namespace PJ {
    /// Utility for reading streams
    /// - Reads null terminated strings
    /// - Byte swaps between big and little endian formats for integral values
    struct StreamReader {
    public:
        using This = StreamReader;

        struct Config {
            std::endian nativeEndian = std::endian::native;
            std::endian streamEndian;
            std::istream& stream;
        };

        Config config;

        StreamReader(Config config) :
            config(config) {}

        template <typename Type>
        void Read(Type& value) const {
            config.stream.read((char*)&value, sizeof(Type));
        }

        /// Integral specialization of Read
        template <typename Type>
        void Read(Type& value) const
            requires std::integral<Type>
        {
            config.stream.read((char*)&value, sizeof(Type));
            if (config.nativeEndian != config.streamEndian) {
                value = std::byteswap(value);
            }
        }

        /// Reads null-terminated strings
        template <>
        void Read<String>(String& value) const {
            std::getline(config.stream, value, '\0');
        }

        template <typename Type>
        Type ReadValue() const {
            Type result{};
            Read(result);
            return result;
        }
    };
} // namespace PJ
