#pragma once

#include "InfixOStreamIterator.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

/*
 CODE REVIEW: ?/23
 TODO: needs unit tests (copy from String)
 */
namespace PJ {
    bool HasPrefix(std::string const& _string, std::string const& searchString);

    bool HasSuffix(std::string const& _string, std::string const& searchString);

    std::vector<std::string> ComponentsSeparatedBy(std::string string, char delimiter);

    std::string U32CharToString(uint32_t _char);

    std::u32string ToU32String(std::string const& utf8);

    template <typename Inserter>
    void Split(const std::string& s, char delimiter, Inserter inserter) {
        std::istringstream stream(s);
        std::string item;
        while (std::getline(stream, item, delimiter)) {
            *inserter++ = item;
        }
    }

    template <class Collection>
    std::string Joined(Collection const& collection, typename Collection::value_type delimiter) {
        std::ostringstream stream;
        std::copy(
            collection.begin(), collection.end(),
            InfixOStreamIterator<typename Collection::value_type>(stream, delimiter.c_str())
        );
        return stream.str();
    }
} // namespace PJ
