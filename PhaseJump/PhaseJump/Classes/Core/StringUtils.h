#pragma once

#include "InfixOStreamIterator.h"
#include "Utils.h"
#include "VectorList.h"
#include <functional>
#include <sstream>
#include <string>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 8/25/24
 */
namespace PJ {
    /// Aliases `std::string` so can build an alternate implementation later if needed
    using String = std::string;

    /// Aliases `std::string_view` so can build an alternate implementation later if needed
    /// StringView is recommend over using String const&, for performance
    using StringView = std::string_view;

    template <class T>
    String MakeString(T value) {
        std::ostringstream stream;
        stream << value;
        return stream.str();
    }

    template <class... Arguments>
    String MakeString(Arguments&&... args) {
        std::ostringstream stream;

        ([&] { stream << args; }(), ...);

        return stream.str();
    }

    String ToLower(StringView s);
    String ToUpper(StringView s);

    String Lowercased(StringView _string);

    String Uppercased(StringView _string);

    bool HasPrefix(StringView _string, StringView searchString);

    bool HasSuffix(StringView _string, StringView searchString);

    bool StartsWith(StringView _string, StringView searchString);

    bool EndsWith(StringView _string, StringView searchString);

    String Prefix(StringView s, size_t size);

    String Suffix(StringView s, size_t size);

    String ReplacingSuffix(StringView _string, StringView search, StringView replace);

    std::vector<String> ComponentsSeparatedBy(StringView string, char delimiter);

    String U32CharToString(uint32_t _char);

    std::u32string ToU32String(StringView utf8);

    bool CompareNoCase(StringView lhs, StringView rhs);

    /// Returns true if the string has numbers only (no - or delimiters)
    bool IsNumbers(StringView _string);

    /// Returns true if the string is a positive or negative numeric integer
    bool IsNumericInt(StringView _string);

    /// Returns true if the string is a postive or negative numeric real
    bool IsNumericReal(StringView _string);

    template <typename Inserter>
    void Split(String const& s, char delimiter, Inserter inserter) {
        std::istringstream stream(s);
        String item;
        while (std::getline(stream, item, delimiter)) {
            inserter(item);
        }
    }

    template <class Collection>
    String Joined(Collection const& collection, typename Collection::value_type delimiter) {
        std::ostringstream stream;
        std::copy(
            collection.begin(), collection.end(),
            InfixOStreamIterator<typename Collection::value_type>(stream, delimiter.c_str())
        );
        return stream.str();
    }

    String Joined(std::initializer_list<String> initializer, String delimiter);

    template <class T>
    T TypeValue(String _string) {
        std::stringstream stream;
        stream << _string;

        T result;
        stream >> result;
        return result;
    }

    String Filter(String _string, std::function<bool(char)> check);
} // namespace PJ
