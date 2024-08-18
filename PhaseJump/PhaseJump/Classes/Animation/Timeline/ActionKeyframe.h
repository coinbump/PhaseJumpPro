#ifndef PJACTIONKEYFRAME_H
#define PJACTIONKEYFRAME_H

#include <functional>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 7/5/24
 */
namespace PJ {
    /// Trigger an action at the keyframe's time
    class ActionKeyframe : SomeKeyframe {
    protected:
        std::function<void()> action;

    public:
        ActionKeyframe(std::function<void()> action) :
            action(action) {}

        void Run() {
            GUARD(action)
            action();
        }

        void operator()() {
            Run();
        }
    };
} // namespace PJ

#endif
