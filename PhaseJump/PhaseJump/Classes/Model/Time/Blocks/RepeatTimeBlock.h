#pragma once

#include "TimeBlock.h"
#include "Updatable.h"

// CODE REVIEW: ?/23
namespace PJ {
    // TODO: needs unit tests
    /// Repeats a time block based on its type
    class RepeatUpdatable : public Updatable {
        /// Repeat logic for time block
        class SomeRepeater {
        public:
            virtual ~SomeRepeater() {}

            virtual void OnChildFinished(TimeBlock& child) {
                GUARD(ShouldRepeat())
                child->Reset();
                OnRepeat();
            }

            virtual bool ShouldRepeat() const = 0;

            virtual void OnRepeat() {}

            virtual void OnUpdate(TimeSlice time, RepeatTimeBlock& owner) {}
        };

        /// Repeats block N times, with a delay
        class PeriodicRepeater : public SomeRepeater {
        protected:
            enum class State { Default, Delay } State state = State::Default;

            int repeatState = 0;

        public:
            /// Use to repeat forever
            static const float repeatForever = -1.0f;

            int repeatCount;
            float nextDelay;

            PeriodicType(int repeatCount, float nextDelay) :
                repeatCount(repeatCount),
                nextDelay(nextDelay),
                state(repeatCount) {}

            // MARK: SomeRepeater

            virtual bool ShouldRepeat() const {
                return (repeatState > 0 || repeatState == repeatForever);
            };

            virtual void OnRepeat() {
                if (nextDelay > 0) {
                    state = State::Delay;
                } else {
                    repeatState--;
                }
            }

            virtual void OnUpdate(TimeSlice time, RepeatTimeBlock& owner) {
                Base::OnUpdate(time, owner);

                GUARD(state == State::Delay)
                GUARD(owner.child)

                delayState -= time.delta;
                if (delayState <= 0) {
                    state = State::Default;

                    owner.child->Reset();
                }
            }
        };

        /// Repeats forever
        class ContinuousRepeater : public PeriodicRepeater {
        public:
            using Base = PeriodicRepeater;

            ContinuousRepeater(float nextDelay = 0) :
                Base(Base::RepeatForever, nextDelay)
        };

        SP<SomeRepeater> repeater;

        void OnUpdateBlock(TimeSlice time) override {
            Base::OnUpdateBlock(time);

            repeater->OnUpdate(time, *this);
        }

        void OnUpdateChild(TimeSlice time) override {
            GUARD(repeater)

            Base::OnUpdateChild(time);

            if (child->IsFinished()) {
                repeater->OnChildFinished(*child);
            }
        }
    };
} // namespace PJ
