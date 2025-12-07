#include "gtest/gtest.h"
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
    
    auto intValue = std::stoi(ss.str());
    ASSERT_EQ(0x01020304, intValue);
}

TEST(StreamWriter, WriteIntegralDifferentEndian) {
    stringstream ss;
    auto sut = StreamWriter({.nativeEndian = std::endian::little, .streamEndian = std::endian::big, .stream = ss });
    sut << static_cast<uint32_t>(0x01020304);
    
    auto intValue = std::stoi(ss.str());
    ASSERT_EQ(0x04030201, intValue);
}

TEST(StreamWriter, WriteNonIntegralSameEndian) {
    stringstream ss;
    auto sut = StreamWriter({.nativeEndian = std::endian::big, .streamEndian = std::endian::big, .stream = ss });
    sut << "1234";
    
    auto stringValue = ss.str();
    ASSERT_EQ(String("1234"), stringValue);
}

TEST(StreamWriter, WriteNonIntegralDifferentEndian) {
    stringstream ss;
    auto sut = StreamWriter({.nativeEndian = std::endian::little, .streamEndian = std::endian::big, .stream = ss });
    sut << "1234";
    
    auto stringValue = ss.str();
    ASSERT_EQ(String("1234"), stringValue);
}
