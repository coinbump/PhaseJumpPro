#pragma once

/*
 RATING: 5 stars
 Simple types
 COD EREVIEW: 12/6/25
 */
namespace PJ {
    using TimeInterval = uint64_t;

    struct Date {
        /// Time value in Unix (POSIX) time
        TimeInterval value;

        bool operator<(Date& rhs) {
            return value < rhs.value;
        }

        bool operator>(Date& rhs) {
            return value > rhs.value;
        }

        bool operator==(Date& rhs) {
            return value == rhs.value;
        }
    };
} // namespace PJ
