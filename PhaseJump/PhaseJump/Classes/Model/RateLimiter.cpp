#include "RateLimiter.h"

using namespace std;
using namespace PJ;

RateLimiter::RateLimiter(float minDelta) {
    core.rate = minDelta;

    onFireFunc = [this](auto& owner) {
        core.timer = 0;
        core.didFire = true;
    };

    canFireFunc = [](SomeLimiter& owner) {
        auto& limiter = *(static_cast<This*>(&owner));

        GUARDR(limiter.core.didFire, true)
        return limiter.core.timer >= limiter.core.rate;
    };

    onUpdateFunc = [](auto& owner, TimeSlice time) {
        auto limiter = static_cast<This*>(&owner);

        GUARDR(limiter->core.didFire, FinishType::Continue)
        limiter->core.timer += time.delta;

        return FinishType::Continue;
    };
}
