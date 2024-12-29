#pragma once

#include "RateLimiter.h"
#include "SequenceCounter.h"
#include "SomeLimiter.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 10/5/24
 */
namespace PJ {
    /// Rate limits in patterns
    /// Example: Every few seconds we emit a burst of bullets in a shump game
    class PatternRateLimiter : public Limiter<> {
    public:
        using Base = Limiter<>;
        using This = PatternRateLimiter;

        /// Fires N times at the specified rate, then switches to the next pattern
        class RateCountPattern {
        public:
            /// Rate for the rate limiter
            float rate{};

            /// How many times Fire will be called before we advance to the next pattern
            int count{};

            RateCountPattern(float rate, int count) :
                rate(rate),
                count(count) {}
        };

        /// Delays for N seconds, then switches to the next pattern
        class DelayPattern {
        public:
            /// Delay to wait before continuing
            float delay{};
        };

        using Pattern = std::variant<DelayPattern, RateCountPattern>;

    protected:
        SequenceCounter counter;
        UP<RateLimiter> rateLimiter;
        int patternIndex{};
        Pattern pattern;

    public:
        VectorList<Pattern> patterns;

        PatternRateLimiter(VectorList<Pattern> _patterns);

        int SequenceCount() const {
            return counter.SequenceCount();
        }

        int PatternIndex() const {
            return patternIndex;
        }

        // MARK: SomeLimiter:

        void OnFire() override;
    };
} // namespace PJ
