#pragma once

#include "Macros.h"
#include "TimeSlice.h"
#include <functional>

/*
 RATING: 5 stars
 Simple protocol
 CODE REVIEW: 6/13/24
 PORTED TO: C++, C#
 */
namespace PJ {
    /// Return type for a finishing updatable func
    enum class FinishType {
        /// Keep running the updatable
        Continue,

        /// Complete the updatable
        Finish
    };

    /// Protocol-only version of updatable, with no stored properties
    class SomeUpdatable {
    public:
        virtual ~SomeUpdatable() {}

        /// Handle time update event
        virtual void OnUpdate(TimeSlice time) = 0;

        /// Allows for cleanup of old updatables
        virtual bool IsFinished() const = 0;
    };

    /// An object that can receive time update events and can finish
    class Updatable : public SomeUpdatable {
    protected:
        /// If true, this updatable is finished running and will be removed from any collections it
        /// belongs to
        bool isFinished = false;

        void SetIsFinished(bool value) {
            isFinished = value;
        }

    public:
        using This = Updatable;
        using OnUpdateFunc = std::function<FinishType(This&, TimeSlice)>;

        /// (Optional). Called for update events
        OnUpdateFunc onUpdateFunc;

        Updatable() {}

        Updatable(OnUpdateFunc onUpdateFunc) :
            onUpdateFunc(onUpdateFunc) {}

        virtual ~Updatable() {}

        FinishType Update(TimeSlice time) {
            OnUpdate(time);
            return isFinished ? FinishType::Finish : FinishType::Continue;
        }

        /// Called for each time delta event
        virtual void OnUpdate(TimeSlice time) {
            GUARD(!isFinished)
            GUARD(onUpdateFunc)
            isFinished = onUpdateFunc(*this, time) == FinishType::Finish;
        }

        /// Allows for cleanup of old updatables
        virtual bool IsFinished() const {
            return isFinished;
        }
    };
} // namespace PJ
