#ifndef PJSTRINGUTILS_H_
#define PJSTRINGUTILS_H_

#include <string>
#include <fstream>

/*
 RATING: 5 stars
 Simple utilities with unit tests
 CODE REVIEW: 5/11/24
 */
namespace PJ {
    struct StringUtils {
        bool HasPrefix(std::string _string, std::string searchString)
        {
            if (searchString.size() > _string.size()) {
                return false;
            }
            return _string.substr(0, searchString.size()) == searchString;
        }

        bool HasSuffix(std::string _string, std::string searchString)
        {
            if (searchString.size() > _string.size()) {
                return false;
            }
            return _string.substr(_string.size() - searchString.size()) == searchString;
        }
    };
}

#endif
