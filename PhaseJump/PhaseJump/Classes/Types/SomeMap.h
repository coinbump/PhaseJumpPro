#ifndef PJSOME_MAP_H_
#define PJSOME_MAP_H_

/*
 RATING: 5 stars
 Simple class
 CODE REVIEW: 11/1/22
 */
namespace PJ {
    /// <summary>
    /// A class that maps a key to a value
    /// </summary>
    template <class Key, class Value>
    class SomeMap
    {
    public:
        virtual Value ValueFor(Key key) const = 0;

        Value operator[](Key key) const {
            return ValueFor(key);
        }
    };
}

#endif  // SOME_MAP_H_
