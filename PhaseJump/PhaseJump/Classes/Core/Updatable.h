#pragma once

#include "Macros.h"
#include "TimeSlice.h"
#include <functional>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/22/24
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

    /// An object that receives time update events
    /// Finish the updatable when it's finished so it can be removed
    class Updatable : public SomeUpdatable {
    protected:
        /// If true, this updatable is finished running and will be removed
        bool isFinished{};

        void SetIsFinished(bool value);

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

        /// Finishes the updatable. After calling this it will not receive any more time updates
        void Finish();

        /// @return Updates and returns finish type
        FinishType Update(TimeSlice time);

        /// Called for each time delta event
        virtual void OnUpdate(TimeSlice time);

        /// @return Returns true when this updatable is finished running and can be safely removed
        virtual bool IsFinished() const {
            return isFinished;
        }
    };
} // namespace PJ
