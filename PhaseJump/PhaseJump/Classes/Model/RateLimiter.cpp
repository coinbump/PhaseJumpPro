#include "RateLimiter.h"

using namespace std;
using namespace PJ;

SP<RateLimiter::Type> RateLimiter::Make(float minDelta) {
    SP<Type> result = MAKE<Type>();

    result->core.minDelta = minDelta;

    result->onFireFunc = [](SomeLimiter<Core>& owner) {
        owner.core.timer = 0;
        owner.core.didFire = true;
    };

    result->canFireFunc = [](SomeLimiter<Core>& owner) {
        GUARDR(owner.core.didFire, true)
        return owner.core.timer >= owner.core.minDelta;
    };

    result->onUpdateFunc = [](Updatable& owner, TimeSlice time) {
        auto limiter = static_cast<Type*>(&owner);

        GUARDR(limiter->core.didFire, FinishType::Continue)
        limiter->core.timer += time.delta;

        return FinishType::Continue;
    };

    return result;
}
