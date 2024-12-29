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

    protected:
        virtual void OnFire() = 0;

    public:
        virtual bool CanFire() {
            return true;
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
        using Base = SomeLimiter;
        using This = Limiter<Core>;

        using CanFireFunc = std::function<bool(This&)>;
        using OnFireFunc = std::function<void(This&)>;

        CanFireFunc canFireFunc;
        OnFireFunc onFireFunc;

        Core core{};

    protected:
        // MARK: public SomeLimiter

        void OnFire() override {
            GUARD(onFireFunc)
            onFireFunc(*this);
        }

    public:
        bool CanFire() override {
            GUARDR(canFireFunc, false)
            return canFireFunc(*this);
        }
    };
} // namespace PJ
