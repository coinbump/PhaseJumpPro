#pragma once

#include <iterator>
#include <ostream>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/21/24
 */
namespace PJ {
    /**
     Infix iterator for joining values with separator delimiters

     Documentation:
     https://codereview.stackexchange.com/questions/13176/infix-iterator-code
     */
    template <class T, class CharT = char, class Traits = std::char_traits<CharT>>
    class InfixOStreamIterator {
        std::basic_ostream<CharT, Traits>& os{};
        CharT const* delimiter{};
        bool isFirstElement = true;

    public:
        using This = InfixOStreamIterator<T, CharT, Traits>;
        using OStreamType = std::basic_ostream<CharT, Traits>;

        InfixOStreamIterator(OStreamType& stream, CharT const* delimiter) :
            os(stream),
            delimiter(delimiter) {}

        This& operator=(T const& item) {
            if (!isFirstElement && delimiter != nullptr)
                os << delimiter;
            os << item;
            isFirstElement = false;
            return *this;
        }

        This& operator*() {
            return *this;
        }

        This& operator++() {
            return *this;
        }

        This& operator++(int) {
            return *this;
        }
    };
} // namespace PJ
