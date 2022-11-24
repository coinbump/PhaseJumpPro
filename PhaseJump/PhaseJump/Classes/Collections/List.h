#ifndef PJLIST_H
#define PJLIST_H

#include <vector>
#include <algorithm>

/*
 RATING: 5 stars
 Adds convenience funcs to std::vector
 CODE REVIEW: 11/20/22
 */
namespace PJ {
    /// <summary>
    /// Extends std::vector with convenience methods
    /// </summary>
    template <class T> class List : public std::vector<T> {
    public:
        typedef std::vector<T> Base;

        // Convenience
        void Append(T const& value) { Base::push_back(value); }
        void Add(T const& value) { Base::push_back(value); }

        // NOTE: Avoid List.Remove for large lists, it is inefficient
        void Remove(T const& value) {
            auto i = std::find(this->begin(), this->end(), value);
            if (i == this->end()) { return; }

            this->erase(i);
        }
    };
}

#endif
