// Named _String.h to avoid conflict with C++ string.h
#ifndef PJSTRING_H_
#define PJSTRING_H_

#include "Utils/StringUtils.h"
#include <string>
#include <sstream>
#include <stdio.h>

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
        bool HasPrefix(std::string findString) const { return StringUtils().HasPrefix(*this, findString); }
        bool HasSuffix(std::string findString) const { return StringUtils().HasSuffix(*this, findString); }
        bool StartsWith(std::string findString) const { return HasPrefix(findString); }
        bool EndsWith(std::string findString) const { return HasSuffix(findString); }

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
        unsigned int UIntValue() const { return TypeValue<unsigned int>(); }
        float FloatValue() const { return TypeValue<float>(); }
        double DoubleValue() const { return TypeValue<double>(); }
        bool BoolValue() const { return TypeValue<bool>(); }

        String ReplacingSuffix(String search, String replace) const {
            auto result = *this;
            if (!HasSuffix(search)) { return result; }

            result.erase(result.size() - search.size(), search.size());
            result += replace;

            return result;
        }

        String Suffix(size_t size) const;
        String Prefix(size_t size) const;

        String ToLower() const;
        String ToUpper() const;
        String Lowercased() const { return ToLower(); }
        String Uppercased() const { return ToUpper(); }
    };
}

#endif
