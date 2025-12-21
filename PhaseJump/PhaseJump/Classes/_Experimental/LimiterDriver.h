#pragma once

#include "Driver.h"
#include "Funcs.h"
#include "SomeLimiter.h"

// needs rethink + tests
// Experimental
namespace PJ {
    /// Continuously sends fire events, which are limited by the limiter
    /// Fire events that are not limited drive an action
    /// Example: cannon that continuously fires every N seconds in shump game
    //    class LimiterDriver : public Driver {
    //    public:
    //        using This = LimiterDriver;
    //        using Base = Driver;
    //
    //        UP<SomeLimiter> limiter;
    //
    //        LimiterDriver(UP<SomeLimiter>& _limiter, ActionFunc action) :
    //            Base(action),
    //            limiter(std::move(_limiter)) {
    //            GUARD(limiter)
    //
    //            SomeLimiter::OnFireFunc onFireFunc = [action](auto& limiter) { action(); };
    //            Override(limiter->onFireFunc, onFireFunc);
    //
    //            onUpdateFunc = [this](auto& updatable, auto time) {
    //                limiter->OnUpdate(time);
    //                limiter->Fire();
    //
    //                return FinishType::Continue;
    //            };
    //        }
    //    };
} // namespace PJ
