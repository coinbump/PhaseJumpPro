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

    updatable.onUpdateFunc = [this](auto& owner, TimeSlice time) {
        GUARDR(this->core.didFire, FinishType::Continue)
        this->core.timer += time.delta;

        return FinishType::Continue;
    };
}
