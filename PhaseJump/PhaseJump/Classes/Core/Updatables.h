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
        using UpdatableList = List<SP<Updatable>>;
        UpdatableList list;

        void Add(SP<Updatable> value) {
            GUARD(value)
            GUARD(!value->IsFinished())

            list.push_back(value);
        }

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

        bool IsFinished() const override {
            return false;
        }
    };
} // namespace PJ
