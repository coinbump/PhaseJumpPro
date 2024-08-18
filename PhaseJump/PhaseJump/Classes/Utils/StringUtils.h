#ifndef PJSTRINGUTILS_H_
#define PJSTRINGUTILS_H_

#include <fstream>
#include <string>

/*
 RATING: 5 stars
 TODO: add unit tests (copy from String)
 CODE REVIEW: 5/11/24
 */
namespace PJ {
    struct StringUtils {
        bool HasPrefix(std::string const& _string, std::string const& searchString) {
            if (searchString.size() > _string.size()) {
                return false;
            }
            return _string.substr(0, searchString.size()) == searchString;
        }

        bool HasSuffix(std::string const& _string, std::string const& searchString) {
            if (searchString.size() > _string.size()) {
                return false;
            }
            return _string.substr(_string.size() - searchString.size()) == searchString;
        }
    };
} // namespace PJ

#endif
