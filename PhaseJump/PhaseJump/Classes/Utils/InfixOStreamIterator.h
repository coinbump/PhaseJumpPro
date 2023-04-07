#ifndef INFIXOSTREAMITERATOR_H
#define INFIXOSTREAMITERATOR_H

#include <ostream>
#include <iterator>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 4/4/23
 */
namespace PJ
{
    /**
     Infix iterator for joining values with separator delimiters

     Documentation:
     https://codereview.stackexchange.com/questions/13176/infix-iterator-code
     https://www.fluentcpp.com/2018/05/08/std-iterator-deprecated/
     https://en.cppreference.com/w/cpp/io/basic_ostream
     */
    template <class T,
    class CharT = char,
    class Traits = std::char_traits<CharT>>
    class InfixOStreamIterator
    {
        using iterator_category = std::output_iterator_tag;
        using value_type = T;
        using difference_type = T;
        using pointer = T*;
        using reference = T&;

        std::basic_ostream<CharT, Traits> *os;
        CharT const* delimiter;
        bool isFirstElement = false;

    public:
        using OStreamType = std::basic_ostream<CharT, Traits>;

        InfixOStreamIterator(OStreamType& stream)
        : os(&stream), delimiter(NULL), isFirstElement(true)
        {}

        InfixOStreamIterator(OStreamType& stream, CharT const *delimiter)
        : os(&stream), delimiter(delimiter), isFirstElement(true)
        {}

        InfixOStreamIterator<T, CharT, Traits>& operator=(T const &item)
        {
            if (!isFirstElement && delimiter != NULL)
                *os << delimiter;
            *os << item;
            isFirstElement = false;
            return *this;
        }

        InfixOStreamIterator<T, CharT, Traits> &operator*() {
            return *this;
        }

        InfixOStreamIterator<T, CharT, Traits> &operator++() {
            return *this;
        }

        InfixOStreamIterator<T, CharT, Traits> &operator++(int) {
            return *this;
        }
    };
}

#endif
