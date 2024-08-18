#ifndef PJANIMATIONCYCLETYPE_H_
#define PJANIMATIONCYCLETYPE_H_

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: TODO
 */
namespace PJ {
    /// Specifies how an animation should cycle when it reaches the end of its
    /// timer duration
    // TODO: use composition for behavior, not enums
    enum class AnimationCycleType {
        // Use fixed enum values for serialization
        // Cycle once
        Once = 0,

        // Cycle once, then in reverse
        PingPong = 1,

        // Cycle once, then start over
        Loop = 2
    };

    /// Specifies which way an animation is cycling
    /// (Ping pong:
    enum class AnimationCycleState {
        // Use fixed enum values for serialization
        Forward = 0,

        Reverse = 1
    };

    static AnimationCycleState Flip(AnimationCycleState state) {
        switch (state) {
        case AnimationCycleState::Forward:
            return AnimationCycleState::Reverse;
        case AnimationCycleState::Reverse:
            return AnimationCycleState::Forward;
        }

        return AnimationCycleState::Forward;
    }
} // namespace PJ

#endif
