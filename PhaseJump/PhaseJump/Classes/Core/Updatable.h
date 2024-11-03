#pragma once

#include "Macros.h"
#include "TimeSlice.h"
#include <functional>

/*
 RATING: 5 stars
 TODO: needs unit tests
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

    /// Lightweight updatable, with no stored properties
    class SomeUpdatable {
    public:
        virtual ~SomeUpdatable() {}

        /// Handle time update event
        virtual void OnUpdate(TimeSlice time) = 0;

        /// Allows for cleanup of old updatables
        virtual bool IsFinished() const = 0;

        /// @return Returns the finish type for this updatable
        FinishType GetFinishType() const {
            return IsFinished() ? FinishType::Finish : FinishType::Continue;
        }
    };

    /// An object that can receive time update events and can finish
    class Updatable : public SomeUpdatable {
    protected:
        /// If true, this updatable is finished running and will be removed from any collections it
        /// belongs to
        bool isFinished = false;

        void SetIsFinished(bool value) {
            GUARD(isFinished != value)
            isFinished = value;

            GUARD(isFinished && onFinishFunc)
            onFinishFunc(*this);
        }

    public:
        using This = Updatable;
        using OnUpdateFunc = std::function<FinishType(This&, TimeSlice)>;
        using Func = std::function<void(This&)>;

        /// (Optional). Called for update events
        OnUpdateFunc onUpdateFunc;

        /// (Optional). Called when this finishes
        Func onFinishFunc;

        Updatable() {}

        Updatable(OnUpdateFunc onUpdateFunc) :
            onUpdateFunc(onUpdateFunc) {}

        virtual ~Updatable() {}

        void Finish() {
            SetIsFinished(true);
        }

        FinishType Update(TimeSlice time) {
            OnUpdate(time);
            return isFinished ? FinishType::Finish : FinishType::Continue;
        }

        /// Called for each time delta event
        virtual void OnUpdate(TimeSlice time) {
            GUARD(!isFinished)
            GUARD(onUpdateFunc)
            SetIsFinished(onUpdateFunc(*this, time) == FinishType::Finish);
        }

        /// Allows for cleanup of old updatables
        virtual bool IsFinished() const {
            return isFinished;
        }
    };
} // namespace PJ
