#pragma once

#include <bit>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/6/25
 */
namespace PJ {
    /// Utility for writing streams
    /// - Byte swaps between big and little endian formats for integral values
    struct StreamWriter {
    public:
        using This = StreamWriter;

        struct Config {
            std::endian nativeEndian = std::endian::native;
            std::endian streamEndian;
            std::ostream& stream;
        };

        Config config;

        StreamWriter(Config config) :
            config(config) {}

        template <typename Type>
        This const& Write(Type const& value) const {
            config.stream << value;
            return *this;
        }

        /// Integral specialization of Write
        template <typename Type>
        This const& Write(Type const& value) const
            requires std::integral<Type>
        {
            if (config.nativeEndian != config.streamEndian) {
                config.stream << std::byteswap(value);
            } else {
                config.stream << value;
            }

            return *this;
        }

        template <typename Type>
        This const& operator<<(Type const& value) const {
            return Write(value);
        }
    };
} // namespace PJ
