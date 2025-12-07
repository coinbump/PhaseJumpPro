#pragma once

#include "List.h"
#include "Macros.h"
#include "Updatable.h"
#include <algorithm>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 8/25/24
 */
namespace PJ {
    /// Updates a list of updatables
    class Updatables : public SomeUpdatable {
    public:
        /// Used for composed update functions when we don't care about the updatable object
        using ContinueOnUpdateFunc = std::function<void(TimeSlice)>;
        using SimpleOnUpdateFunc = std::function<FinishType(TimeSlice)>;
        using UpdatableList = List<SP<SomeUpdatable>>;

    protected:
        UpdatableList list;

    public:
        size_t Count() const {
            return list.size();
        }

        /// Adds an updatable that can finish
        void Add(SP<SomeUpdatable> value) {
            GUARD(value)
            GUARD(!value->IsFinished())

            list.push_back(value);
        }

        /// Adds an updatable func that can finish
        SP<SomeUpdatable> Add(SimpleOnUpdateFunc onUpdateFunc);

        /// Adds an updatable func that will continue running
        SP<SomeUpdatable> AddContinue(ContinueOnUpdateFunc onUpdateFunc);

        /// Adds an updatable func that runs for duration (in seconds)
        SP<SomeUpdatable> AddTimed(float duration, SimpleOnUpdateFunc onUpdateFunc);

        /// Adds an updatable that will start after delay
        SP<SomeUpdatable> AddDelay(float delay, SP<SomeUpdatable> value);

        /**
         @brief Adds an updatable func that starts after delay (in seconds)

         @param delay Delay before sending update events to the func
         @returns
         An updatable that is used to receive update events for the func
         */
        SP<SomeUpdatable> AddDelay(float delay, SimpleOnUpdateFunc onUpdateFunc);

        /// Adds an updatable func that runs at  the specified speed (default is 1)
        SP<SomeUpdatable> AddSpeed(float speed, SimpleOnUpdateFunc onUpdateFunc);

        /// Removes all updatables
        void RemoveAll();

        /// Removes the specified updatable
        void Remove(SomeUpdatable& value) {
            RemoveFirstIf(list, [&](auto& i) { return i.get() == &value; });
        }

        // MARK: SomeUpdatable

        /// Run updates for all updatables in the list
        FinishType OnUpdate(TimeSlice time) override;

        bool IsFinished() const override {
            return false;
        }

        // MARK: Funcs

        /// Make an updatable func that runs for duration (in seconds)
        static SimpleOnUpdateFunc MakeTimedFunc(float duration, SimpleOnUpdateFunc onUpdateFunc);

        /// Make an updatable func that starts after delay (in seconds)
        static SimpleOnUpdateFunc MakeDelayFunc(float delay, SimpleOnUpdateFunc onUpdateFunc);

        /// Make an updatable func that runs at  the specified speed (default is 1)
        static SimpleOnUpdateFunc MakeSpeedFunc(float speed, SimpleOnUpdateFunc onUpdateFunc);

        /// Make an updatable func that starts after delay (in seconds)
        SimpleOnUpdateFunc MakeDelayFunc(float delay, SP<SomeUpdatable> updatable);
    };
} // namespace PJ
