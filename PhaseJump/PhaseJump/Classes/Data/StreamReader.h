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

        StreamReader(Config const& config) :
            config(config) {}

        /// Reads the raw bytes of `value` from the stream. `Type` must be trivially copyable
        /// so the byte representation is well-defined; non-POD types with vtables or owning
        /// handles are rejected at compile time (strings, VectorList, etc. need their own
        /// serialization path — see the `String` specialization below).
        template <typename Type>
        void Read(Type& value) const
            requires std::is_trivially_copyable_v<Type> && (!std::integral<Type>)
        {
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

        /// Reads null-terminated strings. This is a non-template overload so it sidesteps the
        /// ill-formed in-class explicit specialization and avoids the trivially-copyable
        /// constraint on the primary `Read<Type>`.
        void Read(String& value) const {
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
