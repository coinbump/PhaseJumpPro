#pragma once

#include "TimeBlock.h"
#include "Updatable.h"

// CODE REVIEW: ?/23
namespace PJ {
    // TODO: reconsider with composition
    // TODO: needs unit tests
    class TimerTimeBlock : public TimeBlock {
    protected:
        float duration = 0;

        /// Time value
        float state = 0;

        /// Time left over from last update
        float timeOverflow = 0;

    public:
        using Base = TimeBlock;

        TimerTimeBlock(float duration) :
            duration(duration) {}

        void Reset() override {
            state = 0;
            timeOverflow = 0;
            isFinished = false;
        }

        void OnUpdateBlock(TimeSlice time) override {
            GUARD(!isFinished)

            Base::OnUpdateBlock(time);

            state += (time.delta + timeOverflow);
            timeOverflow = 0;

            // Keep track of overflows so our timers are precise
            if (state >= duration) {
                timeOverflow = state % duration;
                Finish();
            }
        }
    };

    /// An updatable that can be composed with other updatables
    class ComposingUpdatable : public Updatable {
    public:
        using Base = Updatable;

        UP<Updatable> funcUpdatable;

        ComposingUpdatable(OnUpdateFunc onUpdateFunc) :
            Updatable(onUpdateFunc) {}

        ComposingUpdatable(UP<Updatable>& funcUpdatable) :
            funcUpdatable(std::move(funcUpdatable)),
            Updatable([this](TimeSlice time) {
            return this->funcUpdatable.OnUpdate(time); },  {
            }
    };

    /// Runs updatable for N time
    class TimedUpdatable : public ComposingUpdatable {
    public:
        using Base = ComposingUpdatable;

    protected:
        float duration = 0;
        float state = 0;

    public:
        //        TimedUpdatable(float duration, OnUpdateFunc onUpdateFunc) :
        //            Base([this](TimeSlice time) {
        //                if (this->state < this->duration) {
        //                    this->state += time.delta;
        //                    GUARDR(this->state >= this->duration, FinishType::Continue)
        //                }
        //                return onUpdateFunc(time);
        //            }),
        //            duration(duration) {
        //        }
        //
        //        TimedUpdatable(float duration, UP<Updatable>& funcUpdatable) :
        //            Base([duration, state = duration](TimeSlice time) mutable {
        //                if (state < duration) {
        //                    state += time.delta;
        //                    GUARDR(state >= duration, FinishType::Continue)
        //                }
        //                return onUpdateFunc(time);
        //            }),
        //            duration(duration) {}

        TimedUpdatable(float duration, UP<Updatable>& funcUpdatable) :
            Base([duration, state = duration](TimeSlice time) mutable {
                if (state < duration) {
                    state += time.delta;
                    GUARDR(state >= duration, FinishType::Continue)
                }
                return onUpdateFunc(time);
            }),
            duration(duration) {}
    };

#if FALSE
    /// ???? PROBLEM: how do we compose Delay and then timed?
    DelayUpdatable(3, TimedUpdatable(4))

        template <Updatables.AddDelay(3, Updatables::MakeTimed(4, [] { ... }))
#endif

                  /// Runs updatable after N time delay
                  class DelayUpdatable : public Updatable {
    public:
        using Base = ComposingUpdatable;

        float delay = 0;

        DelayUpdatable(float delay, OnUpdateFunc onUpdateFunc) :
            Base([this](TimeSlice time) {
                this->delay -= time.delta;
                GUARDR(this->delay <= 0, FinishType::Continue)
                return onUpdateFunc(time);
            }),
            delay(delay) {}
    };

} // namespace PJ
