#include "RateLimiter.h"

using namespace std;
using namespace PJ;

SP<RateLimiter::Type> RateLimiter::Make() {
    SP<Type> result = MAKE<Type>();
    result->onFireFunc = [](SomeLimiter<Core>& owner) {
        owner.core.timer = 0;
        owner.core.didFire = true;
    };

    result->canFireFunc = [](SomeLimiter<Core>& owner) {
        GUARDR(owner.core.didFire, true)
        return owner.core.timer >= owner.core.minDelta;
    };

    result->onUpdateFunc = [](SomeLimiter<Core>& owner, TimeSlice time) {
        GUARD(owner.core.didFire)
        owner.core.timer += time.delta;
    };

    return result;
}
