#ifndef PJSTRINGVECTORLIST_H
#define PJSTRINGVECTORLIST_H

#include "InfixOStreamIterator.h"
#include "_String.h"
#include "VectorList.h"
#include <iostream>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 4/4/23
 */
namespace PJ {
    /// Convenience methods for string collection
    class StringVectorList: public VectorList<String>
    {
    public:
        String Joined(String delimiter)
        {
            std::stringstream stream;
            std::copy(this->begin(), this->end(), InfixOStreamIterator<String>(stream, delimiter.c_str()));
            return stream.str();
        }
    };
}

#endif
