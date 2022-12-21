#ifndef PJACTIONKEYFRAME_H
#define PJACTIONKEYFRAME_H

#include <functional>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/17/22
 */
namespace PJ
{
    /// <summary>
    /// Trigger an action at the keyframe's time
    /// </summary>
    class ActionKeyframe : SomeKeyframe
    {
    protected:
        std::function<void()> action;

    public:
        ActionKeyframe(std::function<void()> action) : action(action)
        {
        }

        void Go()
        {
            action();
        }
    };
}

#endif
