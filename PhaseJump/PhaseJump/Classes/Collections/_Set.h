#ifndef PJSET_H
#define PJSET_H

#include <set>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 6/18/23
 */
namespace PJ
{
    /// <summary>
    /// Adds utility code to std::set
    /// </summary>
    template <class Type, class _Compare = std::less<Type>, class _Allocator = std::allocator<Type>>
    class Set : public std::set<Type, _Compare, _Allocator>
    {
    public:
        using Base = std::set<Type, _Compare, _Allocator>;

        Set() {}
        Set(std::initializer_list<Type> init,
            const _Compare& comp = _Compare(),
            const _Allocator& alloc = _Allocator()) : Base(init, comp, alloc) {
        }
        Set(std::initializer_list<Type> init, const _Allocator& alloc) : Base(init, alloc) {
        }
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

        Set<Type> operator -(Set<Type> const& rhs) const {
            Set<Type> result;

            std::set_difference(this->begin(), this->end(),
                                rhs.begin(), rhs.end(),
                                std::inserter(result, result.end()));

            return result;
        }
    };
}

#endif
