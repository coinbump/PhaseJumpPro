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
        using Base = std::set<Value>;

        Set() {}
        virtual ~Set() {}

        void Add(Value const& value) {
            this->insert(value);
        }

        void Remove(Value const& value) {
            this->erase(value);
        }

        bool Contains(Value const& value) const {
            return this->find(value) != this->end();
        }

        size_t Count() const { return this->size(); }
        
        bool IsEmpty() const { return this->empty(); }
    };
}

#endif
