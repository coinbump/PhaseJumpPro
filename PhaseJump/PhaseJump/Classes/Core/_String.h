// Named _String.h to avoid conflict with C++ string.h
#pragma once

#include "StringUtils.h"
#include "Utils.h"
#include <algorithm>
#include <functional>
#include <sstream>
#include <stdio.h>
#include <string>

// TODO: rethink using inheritance here
/*
 RATING: 5 stars
 Simple utility with unit tests
 CODE REVIEW: 7/6/24
 */
namespace PJ {
    /// Adds utility methods to std::string
    struct String : public std::string {
    public:
        String() :
            std::string() {}

        String(String const& str) :
            std::string(str) {}

        String(const std::string& str) :
            std::string(str) {}

        String(const char* s) :
            std::string(s) {}

        String(float value) {
            std::stringstream stream;
            stream << value;
            (*this) = stream.str();
        }

        String(int value) {
            std::stringstream stream;
            stream << value;
            (*this) = stream.str();
        }

        // Standard
        bool HasPrefix(String const& findString) const {
            auto& _string = *this;
            if (findString.size() > _string.size()) {
                return false;
            }
            return _string.substr(0, findString.size()) == findString;
        }

        bool HasSuffix(String const& findString) const {
            auto& _string = *this;
            if (findString.size() > _string.size()) {
                return false;
            }
            return _string.substr(_string.size() - findString.size()) == findString;
        }

        bool StartsWith(String const& findString) const {
            return HasPrefix(findString);
        }

        bool EndsWith(String const& findString) const {
            return HasSuffix(findString);
        }

        bool CompareNoCase(const std::string_view& rhs) {
            return std::equal(
                this->begin(), this->end(), rhs.begin(), rhs.end(),
                [](char a, char b) { return tolower(a) == tolower(b); }
            );
        }

        template <class T>
        T TypeValue() const {
            std::stringstream stream;
            stream << *this;

            T result;
            stream >> result;
            return result;
        }

        int64_t Int64Value() const {
            return TypeValue<int64_t>();
        }

        int IntValue() const {
            return TypeValue<int>();
        }

        unsigned int UIntValue() const {
            return TypeValue<unsigned int>();
        }

        float FloatValue() const {
            return TypeValue<float>();
        }

        double DoubleValue() const {
            return TypeValue<double>();
        }

        bool BoolValue() const {
            return TypeValue<bool>();
        }

        String ReplacingSuffix(String search, String replace) const {
            auto result = *this;
            if (!HasSuffix(search)) {
                return result;
            }

            result.erase(result.size() - search.size(), search.size());
            result += replace;

            return result;
        }

        String Suffix(size_t size) const;
        String Prefix(size_t size) const;

        String ToLower() const;
        String ToUpper() const;

        String Lowercased() const {
            return ToLower();
        }

        String Uppercased() const {
            return ToUpper();
        }

        String Filter(std::function<bool(char)> check) {
            String result;
            GUARDR(check, result);
            std::copy_if(this->begin(), this->end(), std::back_inserter(result), check);
            return result;
        }

        void RemoveIf(std::function<bool(char)> check) {
            this->erase(std::remove_if(this->begin(), this->end(), check), this->end());
        }

        bool IsEmpty() const {
            return size() <= 0;
        }
    };

    using StringC = String const&;
} // namespace PJ

// MARK: - std::hash

template <>
struct std::hash<PJ::String> {
    std::size_t operator()(PJ::String const& s) const noexcept {
        return std::hash<string>{}(s);
    }
};
