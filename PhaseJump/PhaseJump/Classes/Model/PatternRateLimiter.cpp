#include "PatternRateLimiter.h"

using namespace std;
using namespace PJ;

PatternRateLimiter::PatternRateLimiter(VectorList<Pattern> _patterns) :
    counter(0),
    patterns(_patterns) {
    GUARD(!IsEmpty(patterns))

    auto buildRateLimiter = [this]() {
        auto rateCountPattern = std::get_if<RateCountPattern>(&pattern);
        if (rateCountPattern) {
            counter.maxCount = rateCountPattern->count;
            rateLimiter = NEW<RateLimiter>(rateCountPattern->rate);
        } else {
            rateLimiter = nullptr;
        }
    };

    auto advancePattern = [buildRateLimiter, this]() {
        patternIndex++;
        patternIndex %= patterns.size();
        pattern = patterns[patternIndex];

        buildRateLimiter();
    };

    counter.onFinishSequenceFunc = [advancePattern, this](auto& counter) { advancePattern(); };

    canFireFunc = [this](auto& _limiter) {
        GUARDR(rateLimiter, false)
        return rateLimiter->CanFire();
    };

    pattern = patterns[0];
    buildRateLimiter();

    onUpdateFunc = [advancePattern, this](auto& updatable, auto time) {
        GUARDR(!IsEmpty(patterns), FinishType::Finish);

        auto delayPattern = std::get_if<DelayPattern>(&pattern);
        if (delayPattern) {
            if (delayPattern->delay > 0) {
                delayPattern->delay -= time.delta;
            }

            if (delayPattern->delay <= 0) {
                advancePattern();
            }
        } else if (rateLimiter) {
            rateLimiter->OnUpdate(time);
        }

        return FinishType::Continue;
    };
}

void PatternRateLimiter::OnFire() {
    Base::OnFire();

    if (rateLimiter) {
        // Keep rate limiter states in sync
        rateLimiter->Fire();
    }

    counter.Fire();
}
