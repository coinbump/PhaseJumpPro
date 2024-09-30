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
    /// Limits whether or not an event can occur
    /// Owner must send OnUpdate events for timers to work
    template <class Core = Void>
    class SomeLimiter : public OwnerBase<Core>, public Updatable {
    public:
        using This = SomeLimiter<Core>;
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
        bool Fire() {
            GUARDR(CanFire(), false);

            OnFire();
            return true;
        }

        virtual bool CanFire() {
            GUARDR(canFireFunc, false)
            return canFireFunc(*this);
        }
    };

} // namespace PJ
