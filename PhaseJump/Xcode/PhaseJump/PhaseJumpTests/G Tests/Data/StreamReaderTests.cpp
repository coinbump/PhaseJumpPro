#include "gtest/gtest.h"
#include "StreamReader.h"

using namespace PJ;
using namespace std;

namespace StreamReaderTests {
}

using namespace StreamReaderTests;

TEST(StreamReader, ReadIntegralSameEndian) {
    stringstream ss;
    ss << static_cast<unsigned char>(0x01);
    ss << static_cast<unsigned char>(0x02);
    ss << static_cast<unsigned char>(0x03);
    ss << static_cast<unsigned char>(0x04);
    auto sut = StreamReader({.nativeEndian = std::endian::big, .streamEndian = std::endian::big, .stream = ss });
    
    uint32_t data;
    sut.Read(data);
    
    ASSERT_EQ(0x04030201, data);
}

TEST(StreamReader, ReadIntegralDifferentEndian) {
    stringstream ss;
    ss << static_cast<unsigned char>(0x01);
    ss << static_cast<unsigned char>(0x02);
    ss << static_cast<unsigned char>(0x03);
    ss << static_cast<unsigned char>(0x04);
    auto sut = StreamReader({.nativeEndian = std::endian::little, .streamEndian = std::endian::big, .stream = ss });
    
    uint32_t data;
    sut.Read(data);
    
    ASSERT_EQ(std::byteswap(0x04030201), data);
}

TEST(StreamReader, ReadIntegralValueDifferentEndian) {
    stringstream ss;
    ss << static_cast<unsigned char>(0x01);
    ss << static_cast<unsigned char>(0x02);
    ss << static_cast<unsigned char>(0x03);
    ss << static_cast<unsigned char>(0x04);
    auto sut = StreamReader({.nativeEndian = std::endian::little, .streamEndian = std::endian::big, .stream = ss });
    
    uint32_t data = sut.ReadValue<uint32_t>();
    ASSERT_EQ(std::byteswap(0x04030201), data);
}

TEST(StreamReader, ReadNonIntegralSameEndian) {
    stringstream ss;
    ss << "abcd";
    auto sut = StreamReader({.nativeEndian = std::endian::big, .streamEndian = std::endian::big, .stream = ss });
    
    char data[4];
    sut.Read(data);
    
    ASSERT_EQ('a', data[0]);
    ASSERT_EQ('b', data[1]);
    ASSERT_EQ('c', data[2]);
    ASSERT_EQ('d', data[3]);
}

TEST(StreamReader, ReadNonIntegralDifferentEndian) {
    stringstream ss;
    ss << "abcd";
    auto sut = StreamReader({.nativeEndian = std::endian::little, .streamEndian = std::endian::big, .stream = ss });
    
    char data[4];
    sut.Read(data);
    
    ASSERT_EQ('a', data[0]);
    ASSERT_EQ('b', data[1]);
    ASSERT_EQ('c', data[2]);
    ASSERT_EQ('d', data[3]);
}

TEST(StreamReader, ReadNullTerminatedString) {
    stringstream ss;
    ss << "abcd";
    ss << '\0';
    ss << "1234";
    auto sut = StreamReader({.nativeEndian = std::endian::big, .streamEndian = std::endian::big, .stream = ss });
    
    String string;
    sut.Read(string);
    ASSERT_EQ("abcd", string);
    
    sut.Read(string);
    ASSERT_EQ("1234", string);
}
