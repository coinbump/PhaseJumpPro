// Named _String.h to avoid conflict with C++ string.h
#ifndef PJSTRING_H_
#define PJSTRING_H_

#include "StringUtils.h"
#include <string>
#include <sstream>

/*
 RATING: 5 stars
 Simple utility with unit tests
 CODE REVIEW: 11/6/22
 */
namespace PJ {
    /// <summary>
    /// Wraps `string` to add extension functions
    /// </summary>
    struct String : std::string {
    public:
        String() : std::string() {}
        String(const std::string& str) : std::string(str) {}
        String(const char* s) : std::string(s) {}

        // Standard
        bool HasPrefix(std::string findString) { return StringUtils().HasPrefix(*this, findString); }
        bool HasSuffix(std::string findString) { return StringUtils().HasSuffix(*this, findString); }
        bool StartsWith(std::string findString) { return HasPrefix(findString); }
        bool EndsWith(std::string findString) { return HasSuffix(findString); }

        template <class T>
        T TypeValue() const {
            std::stringstream stream;
            stream << *this;

            T result;
            stream >> result;
            return result;
        }

        int64_t Int64Value() const { return TypeValue<int64_t>(); }
        int IntValue() const { return TypeValue<int>(); }
        unsigned int UIntValue() const { return TypeValue<uint>(); }
        float FloatValue() const { return TypeValue<float>(); }
        double DoubleValue() const { return TypeValue<double>(); }
        bool BoolValue() const { return TypeValue<bool>(); }

        String ReplacingSuffix(String search, String replace) {
            auto result = *this;
            if (!HasSuffix(search)) { return result; }

            result.erase(result.size() - search.size(), search.size());
            result += replace;

            return result;
        }

        String Suffix(size_t size);
        String Prefix(size_t size);

        String ToLower();
        String ToUpper();
        String Lowercased() { return ToLower(); }
        String Uppercased() { return ToUpper(); }
    };
}

#endif
