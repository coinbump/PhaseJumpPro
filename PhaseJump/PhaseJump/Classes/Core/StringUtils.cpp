#include "StringUtils.h"

using namespace std;
using namespace PJ;

std::string PJ::U32CharToString(uint32_t _char) {
    // This isn't doing any Unicode conversions, but this function exists
    // so we have this wrapped for future Unicode support
    // FUTURE: support Unicode
    std::string stringChar(1, _char);
    return stringChar;
}

std::u32string PJ::ToU32String(std::string const& utf8) {
    std::u32string result;

    // This isn't doing any Unicode conversions, but this function exists
    // so we have this wrapped for future Unicode support
    // FUTURE: support Unicode
    for (size_t i = 0; i < utf8.size(); i++) {
        result += utf8[i];
    }

    return result;
}

bool PJ::HasPrefix(std::string const& _string, std::string const& searchString) {
    if (searchString.size() > _string.size()) {
        return false;
    }
    return _string.substr(0, searchString.size()) == searchString;
}

bool PJ::HasSuffix(std::string const& _string, std::string const& searchString) {
    if (searchString.size() > _string.size()) {
        return false;
    }
    return _string.substr(_string.size() - searchString.size()) == searchString;
}

std::vector<std::string> PJ::ComponentsSeparatedBy(std::string string, char delimiter) {
    std::vector<std::string> result;
    Split(string, delimiter, std::back_inserter(result));
    return result;
}

// FUTURE: implement unicode support and test
// std::u32string utf8_to_utf32(const std::string& utf8) {
//    std::vector<uint32_t> unicode;
//    size_t i = 0;
//    while (i < utf8.size()) {
//        uint32_t uni;
//        size_t todo;
//        bool error = false;
//        unsigned char ch = utf8[i++];
//        if (ch <= 0x7F)
//        {
//            uni = ch;
//            todo = 0;
//        }
//        else if (ch <= 0xBF)
//        {
//            throw std::logic_error("not a UTF-8 string");
//        }
//        else if (ch <= 0xDF)
//        {
//            uni = ch&0x1F;
//            todo = 1;
//        }
//        else if (ch <= 0xEF)
//        {
//            uni = ch&0x0F;
//            todo = 2;
//        }
//        else if (ch <= 0xF7)
//        {
//            uni = ch&0x07;
//            todo = 3;
//        }
//        else
//        {
//            throw std::logic_error("not a UTF-8 string");
//        }
//
//        for (size_t j = 0; j < todo; ++j)
//        {
//            if (i == utf8.size())
//                throw std::logic_error("not a UTF-8 string");
//            unsigned char ch = utf8[i++];
//            if (ch < 0x80 || ch > 0xBF)
//                throw std::logic_error("not a UTF-8 string");
//            uni <<= 6;
//            uni += ch & 0x3F;
//        }
//        if (uni >= 0xD800 && uni <= 0xDFFF)
//            throw std::logic_error("not a UTF-8 string");
//        if (uni > 0x10FFFF)
//            throw std::logic_error("not a UTF-8 string");
//        unicode.push_back(uni);
//    }
//    std::u32string result;
//    for (size_t i = 0; i < unicode.size(); ++i)
//    {
//        uint32_t uni = unicode[i];
//        if (uni <= 0xFFFF)
//        {
//            result += (uint32_t)uni;
//        }
//        else
//        {
//            uni -= 0x10000;
//            result += (uint32_t)((uni >> 10) + 0xD800);
//            result += (uint32_t)((uni & 0x3FF) + 0xDC00);
//        }
//    }
//    return result;
//}
