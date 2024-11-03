#pragma once

#include "Base.h"
#include "Updatable.h"
#include "Void.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 9/28/24
 */
namespace PJ {
    /// Limits action by a canFireFunc
    /// Owner must send update events for timers to work
    class SomeLimiter : public Updatable {
    public:
        using This = SomeLimiter;
        using CanFireFunc = std::function<bool(This&)>;
        using OnFireFunc = std::function<void(This&)>;

        CanFireFunc canFireFunc;
        OnFireFunc onFireFunc;

    protected:
        virtual void OnFire() {
            GUARD(onFireFunc)
            onFireFunc(*this);
        }

    public:
        virtual bool CanFire() {
            GUARDR(canFireFunc, false)
            return canFireFunc(*this);
        }

        virtual void Fire() {
            GUARD(CanFire());

            OnFire();
        }
    };

    /// Limiter with a core
    template <class Core = Void>
    class Limiter : public SomeLimiter {
    public:
        Core core{};
    };
} // namespace PJ
