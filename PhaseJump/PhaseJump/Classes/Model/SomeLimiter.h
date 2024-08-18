#ifndef PJSOMELIMITER_H
#define PJSOMELIMITER_H

#include "Base.h"
#include "Updatable.h"
#include "Void.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 6/8/24
 */
namespace PJ {
    /// Limits whether or not an event can occur
    /// Owner must send OnUpdate events for timers to work
    template <class Core = Void>
    class SomeLimiter : public OwnerBase<Core>, public Updatable {
    public:
        // TODO: rethink this. Is this actually better?
        std::function<bool(SomeLimiter&)> canFireFunc;
        std::function<void(SomeLimiter&)> onFireFunc;
        std::function<void(SomeLimiter&, TimeSlice)> onUpdateFunc;

    protected:
        virtual void OnFire() {
            GUARD(onFireFunc)
            onFireFunc(*this);
        }

    public:
        bool Fire() {
            auto result = CanFire();
            if (result) {
                OnFire();
            }
            return result;
        }

        virtual bool CanFire() {
            GUARDR(canFireFunc, false)
            return canFireFunc(*this);
        }

        // MARK: Updatable

        void OnUpdate(TimeSlice time) override {
            GUARD(onUpdateFunc)
            onUpdateFunc(*this, time);
        }
    };

} // namespace PJ

#endif
