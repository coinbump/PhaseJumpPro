#pragma once

#include "Updatable.h"
#include "Utils.h"

// CODE REVIEW: ?/23
namespace PJ {
    // TODO: needs unit tests
    /// Composable updatable block that can be composed with others to modify
    /// behavior like repeat, speed, etc.
    class TimeBlock : public Updatable {
    protected:
        UP<TimeBlock> child;

        virtual void OnFinish() {}

    public:
        using Base = Updatable;

        TimeBlock() {}

        TimeBlock(UP<TimeBlock>& child) :
            child(std::move(child)) {}

        void Finish() {
            GUARD(!isFinished)
            isFinished = true;

            OnFinish();
        }

        virtual void Reset() {
            isFinished = false;
        }

        void OnUpdate(TimeSlice time) override {
            GUARD(!isFinished)
            OnUpdateBlock(time);

            OnUpdateChild(time);
        }

        virtual void OnUpdateChild(TimeSlice time) {
            GUARD(child)
            child->OnUpdate(time);
        }

        virtual void OnUpdateBlock(TimeSlice time) = 0;
    };
} // namespace PJ
