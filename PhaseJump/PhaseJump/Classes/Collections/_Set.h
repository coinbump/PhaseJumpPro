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
    template <class Type> class Set : public std::set<Type>
    {
    public:
        using Base = std::set<Type>;

        Set() {}
        virtual ~Set() {}

        void Add(Type const& value) {
            this->insert(value);
        }

        void Remove(Type const& value) {
            this->erase(value);
        }

        bool Contains(Type const& value) const {
            return this->find(value) != this->end();
        }

        size_t Count() const { return this->size(); }
        
        bool IsEmpty() const { return this->empty(); }

        void AddOrRemove(Type const& value, bool add) {
            if (add) {
                Add(value);
            } else {
                Remove(value);
            }
        }
    };
}

#endif
