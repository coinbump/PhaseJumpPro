#ifndef PJSTRINGUTILS_H_
#define PJSTRINGUTILS_H_

#include <string>
#include <fstream>

/*
 RATING: 5 stars
 Simple utilities with unit tests
 CODE REVIEW: 11/6/22
 */
namespace PJ {
    struct StringUtils {
        bool HasPrefix(std::string _string, std::string searchString)
        {
            return _string.rfind(searchString, 0) == 0;
        }

        bool HasSuffix(std::string _string, std::string searchString)
        {
            return _string.rfind(searchString) == _string.size() - searchString.size();
        }
    };
}

#endif
