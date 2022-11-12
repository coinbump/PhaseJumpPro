//
//  List.h
//  PhaseJump
//
//  Created by Jeremy Vineyard on 11/12/22.
//

#ifndef PJLIST_H
#define PJLIST_H

#include <vector>

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
    };
}

#endif
