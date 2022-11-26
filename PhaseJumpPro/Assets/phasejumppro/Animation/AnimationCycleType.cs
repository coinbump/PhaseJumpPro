using System;

/*
 * RATING: 5 stars
 * Simple types
 * CODE REVIEW: 4/21/22
 * PORTED TO: C++
 */
namespace PJ
{
    /// <summary>
    /// Specifies how an animation should cycle when it reaches the end of its timer duration
    /// </summary>
    public enum AnimationCycleType
    {
        // Cycle once
        Once,

        // Cycle once, then in reverse
        PingPong,

        // Cycle once, then start over
        Loop
    }

    public static class Extensions_AnimationCycleState
    {
        public static AnimationCycleState Flip(this AnimationCycleState state)
        {
            switch (state)
            {
                case AnimationCycleState.Forward:
                    return AnimationCycleState.Reverse;
                case AnimationCycleState.Reverse:
                    return AnimationCycleState.Forward;
            }

            return AnimationCycleState.Forward;
        }
    }

    /// <summary>
    /// Specifies which way an animation is cycling
    /// (Ping pong: 
    /// </summary>
    public enum AnimationCycleState
    {
        Forward,

        Reverse
    }
}
