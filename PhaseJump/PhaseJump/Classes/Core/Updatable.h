#pragma once

#include "Macros.h"
#include "TimeSlice.h"
#include <functional>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/2/25
 FUTURE: C#/Swift ports need updates to new pattern
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
        virtual FinishType OnUpdate(TimeSlice time) = 0;

        /// Allows for cleanup of old updatables
        virtual bool IsFinished() const = 0;

        /// Optionally sets the finished value
        virtual void SetIsFinished(bool value) {}

        void Finish() {
            SetIsFinished(true);
        }

        /// Optionally resets the updatable to its initial state
        virtual void Reset() {}
    };

    /// An object that receives time update events
    /// Finish the updatable when it's finished so it can be removed
    class Updatable : public SomeUpdatable {
    protected:
        /// If true, this updatable is finished running and will be removed
        bool isFinished{};

    public:
        using This = Updatable;
        using OnUpdateFunc = std::function<FinishType(This&, TimeSlice)>;
        using Func = std::function<void(This&)>;

        /// (Optional). Called for update events
        OnUpdateFunc onUpdateFunc;

        /// (Optional). Called when this finishes
        Func onFinishFunc;

        Updatable();
        Updatable(OnUpdateFunc onUpdateFunc);

        virtual ~Updatable() {}

        // MARK: SomeUpdatable

        /// Called for each time delta event
        FinishType OnUpdate(TimeSlice time) override;

        void SetIsFinished(bool value) override;

        /// @return Returns true when this updatable is finished running and can be safely removed
        bool IsFinished() const override {
            return isFinished;
        }
    };
} // namespace PJ
