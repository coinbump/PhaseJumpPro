#pragma once

#include "List.h"
#include "Macros.h"
#include "Updatable.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 8/25/24
 */
namespace PJ {
    /// Updates a list of updatables
    class Updatables : public Updatable {
    public:
        using OnUpdateFunc = std::function<FinishType(TimeSlice)>;
        using UpdatableList = List<SP<Updatable>>;
        UpdatableList list;

        size_t Count() const {
            return list.size();
        }

        void Add(SP<Updatable> value) {
            GUARD(value)
            GUARD(!value->IsFinished())

            list.push_back(value);
        }

        /// Adds an updatable func that can finish
        SP<Updatable> Add(OnUpdateFunc onUpdateFunc) {
            auto result =
                MAKE<Updatable>([=](auto&, TimeSlice time) { return onUpdateFunc(time); });
            Add(result);

            return result;
        }

        /// Adds an updatable func that will continue running
        SP<Updatable> AddContinue(std::function<void(TimeSlice time)> onUpdateFunc) {
            auto result = MAKE<Updatable>([=](auto&, TimeSlice time) {
                onUpdateFunc(time);
                return FinishType::Continue;
            });
            Add(result);

            return result;
        }

        /// Adds an updatable func that runs for duration (in seconds)
        SP<Updatable> AddTimed(float duration, OnUpdateFunc onUpdateFunc);

        /// Adds an updatable func that starts after delay (in seconds)
        SP<Updatable> AddDelay(float delay, OnUpdateFunc onUpdateFunc);

        /// Adds an updatable func that runs at  the specified speed (default is 1)
        SP<Updatable> AddSpeed(float speed, OnUpdateFunc onUpdateFunc);

        // MARK: Updatable

        void OnUpdate(TimeSlice time) override {
            UpdatableList freshList;

            auto iter = list;
            std::for_each(iter.begin(), iter.end(), [&](SP<Updatable> u) {
                GUARD(!u->IsFinished())

                u->OnUpdate(time);
                GUARD(!u->IsFinished())

                freshList.push_back(u);
            });

            list = freshList;
        }

        // MARK: Funcs

        /// Make an updatable func that runs for duration (in seconds)
        static OnUpdateFunc MakeTimedFunc(float duration, OnUpdateFunc onUpdateFunc);

        /// Make an updatable func that starts after delay (in seconds)
        static OnUpdateFunc MakeDelayFunc(float delay, OnUpdateFunc onUpdateFunc);

        /// Make an updatable func that runs at  the specified speed (default is 1)
        static OnUpdateFunc MakeSpeedFunc(float speed, OnUpdateFunc onUpdateFunc);
    };
} // namespace PJ
