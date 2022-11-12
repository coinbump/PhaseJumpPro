#ifndef PJANIMATIONCYCLETYPE_H_
#define PJANIMATIONCYCLETYPE_H_

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 11/7/22
 */
namespace PJ
{
    /// <summary>
    /// Specifies how an animation should cycle when it reaches the end of its timer duration
    /// </summary>
    enum class AnimationCycleType
    {
        // Cycle once
        Once,

        // Cycle once, then in reverse
        PingPong,

        // Cycle once, then start over
        Loop
    };

    /// <summary>
    /// Specifies which way an animation is cycling
    /// (Ping pong:
    /// </summary>
    enum class AnimationCycleState
    {
        Forward,

        Reverse
    };

    namespace AnimationCycleStateExt {
        static AnimationCycleState Flip(AnimationCycleState state)
        {
            switch (state)
            {
                case AnimationCycleState::Forward:
                    return AnimationCycleState::Reverse;
                case AnimationCycleState::Reverse:
                    return AnimationCycleState::Forward;
            }

            return AnimationCycleState::Forward;
        }
    }
}

#endif
