#ifndef PJSET_H
#define PJSET_H

#include <set>

/*
 RATING: 5 stars
 Simple utility
 CODE REVIEW: 11/20/22
 */
namespace PJ
{
    /// <summary>
    /// Adds utility code to std::map
    /// </summary>
    template <class Value> class Set : public std::set<Value>
    {
    public:
        typedef std::set<Value> Base;

        Set() {}
        virtual ~Set() {}

        void Add(Value const& value) {
            Base::insert(value);
        }

        void Remove(Value const& value) {
            Base::erase(value);
        }
    };
}

#endif
