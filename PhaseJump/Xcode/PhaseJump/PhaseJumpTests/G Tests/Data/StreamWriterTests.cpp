#include "gtest/gtest.h"
#include "StreamReader.h"
#include "StreamWriter.h"

using namespace PJ;
using namespace std;

namespace StreamWriterTests {
}

using namespace StreamWriterTests;

TEST(StreamWriter, WriteIntegralSameEndian) {
    stringstream ss;
    auto sut = StreamWriter({.nativeEndian = std::endian::big, .streamEndian = std::endian::big, .stream = ss });
    sut << static_cast<uint32_t>(0x01020304);

    // Raw binary write: 4 bytes in native byte order.
    auto bytes = ss.str();
    ASSERT_EQ(sizeof(uint32_t), bytes.size());
    uint32_t readBack = 0;
    std::memcpy(&readBack, bytes.data(), sizeof(uint32_t));
    ASSERT_EQ(0x01020304u, readBack);
}

TEST(StreamWriter, WriteIntegralDifferentEndian) {
    stringstream ss;
    auto sut = StreamWriter({.nativeEndian = std::endian::little, .streamEndian = std::endian::big, .stream = ss });
    sut << static_cast<uint32_t>(0x01020304);

    // Byte-swapped binary write.
    auto bytes = ss.str();
    ASSERT_EQ(sizeof(uint32_t), bytes.size());
    uint32_t readBack = 0;
    std::memcpy(&readBack, bytes.data(), sizeof(uint32_t));
    ASSERT_EQ(0x04030201u, readBack);
}

TEST(StreamWriter, WriteTriviallyCopyableStructRoundTrip) {
    struct Sample {
        uint32_t a;
        float b;
    };
    Sample source{ 0xDEADBEEF, 3.14f };

    stringstream ss;
    auto writer = StreamWriter({.nativeEndian = std::endian::native, .streamEndian = std::endian::native, .stream = ss });
    writer << source;

    auto reader = StreamReader({.nativeEndian = std::endian::native, .streamEndian = std::endian::native, .stream = ss });
    Sample readBack{};
    reader.Read(readBack);

    ASSERT_EQ(source.a, readBack.a);
    ASSERT_EQ(source.b, readBack.b);
}

TEST(StreamWriter, WriteIntegralRoundTripViaStreamReader) {
    stringstream ss;
    auto writer = StreamWriter({.nativeEndian = std::endian::little, .streamEndian = std::endian::big, .stream = ss });
    writer << static_cast<uint32_t>(0x01020304);

    // Reader uses matching config: should round-trip to the original value.
    auto reader = StreamReader({.nativeEndian = std::endian::little, .streamEndian = std::endian::big, .stream = ss });
    uint32_t readBack = 0;
    reader.Read(readBack);
    ASSERT_EQ(0x01020304u, readBack);
}
