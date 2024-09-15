#include "StringUtils.h"
#include "VectorList.h"
#include <algorithm>
#include <sstream>
#include <stdio.h>

using namespace std;
using namespace PJ;

String PJ::ToLower(StringView s) {
    String result(s);
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);

    return result;
}

String PJ::ToUpper(StringView s) {
    String result(s);
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);

    return result;
}

String PJ::Prefix(StringView s, size_t size) {
    if (size > s.size()) {
        return String();
    }

    return String(s.substr(0, size));
}

String PJ::Suffix(StringView s, size_t size) {
    if (size > s.size()) {
        return String();
    }

    return String(s.substr(s.size() - size, size));
}

bool PJ::StartsWith(StringView _string, StringView searchString) {
    return HasPrefix(_string, searchString);
}

bool PJ::EndsWith(StringView _string, StringView searchString) {
    return HasSuffix(_string, searchString);
}

bool PJ::CompareNoCase(StringView lhs, StringView rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), [](char a, char b) {
        return tolower(a) == tolower(b);
    });
}

bool PJ::IsNumbers(StringView _string) {
    return _string.find_first_not_of("0123456789") == std::string::npos;
}

bool PJ::IsNumericInt(StringView _string) {
    return _string.find_first_not_of("0123456789-") == std::string::npos;
}

bool PJ::IsNumericReal(StringView _string) {
    return _string.find_first_not_of("0123456789.-") == std::string::npos;
}

String PJ::ReplacingSuffix(StringView _string, StringView search, StringView replace) {
    String result(_string);
    if (!HasSuffix(_string, search)) {
        return result;
    }

    result.erase(result.size() - search.size(), search.size());
    result += replace;

    return result;
}

String PJ::Lowercased(StringView _string) {
    return ToLower(_string);
}

String PJ::Uppercased(StringView _string) {
    return ToUpper(_string);
}

String PJ::Filter(String _string, std::function<bool(char)> check) {
    String result;
    GUARDR(check, result);
    std::copy_if(_string.begin(), _string.end(), std::back_inserter(result), check);
    return result;
}

std::string PJ::U32CharToString(uint32_t _char) {
    // This isn't doing any Unicode conversions, but this function exists
    // so we have this wrapped for future Unicode support
    // FUTURE: support Unicode
    std::string stringChar(1, _char);
    return stringChar;
}

std::u32string PJ::ToU32String(StringView utf8) {
    std::u32string result;

    // This isn't doing any Unicode conversions, but this function exists
    // so we have this wrapped for future Unicode support
    // FUTURE: support Unicode
    for (size_t i = 0; i < utf8.size(); i++) {
        result += utf8[i];
    }

    return result;
}

bool PJ::HasPrefix(StringView _string, StringView searchString) {
    if (searchString.size() > _string.size()) {
        return false;
    }
    return _string.substr(0, searchString.size()) == searchString;
}

bool PJ::HasSuffix(StringView _string, StringView searchString) {
    if (searchString.size() > _string.size()) {
        return false;
    }
    return _string.substr(_string.size() - searchString.size()) == searchString;
}

std::vector<std::string> PJ::ComponentsSeparatedBy(StringView string, char delimiter) {
    std::vector<std::string> result;
    Split(String(string), delimiter, [&](String s) { result.push_back(s); });
    return result;
}

String PJ::Joined(std::initializer_list<String> initializer, String delimiter) {
    std::ostringstream stream;
    VectorList<String> collection{ initializer };
    std::copy(
        collection.begin(), collection.end(),
        InfixOStreamIterator<String>(stream, delimiter.c_str())
    );
    return stream.str();
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
