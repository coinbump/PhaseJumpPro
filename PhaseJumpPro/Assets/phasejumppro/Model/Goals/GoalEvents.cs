/*
 * RATING: 5 stars
 * Simple utility
 * CODE REVIEW: 12/29/22
 * PORTED TO: C++
 */
namespace PJ
{
    /// <summary>
    /// Event: goal has been completed
    /// </summary>
    public class EventGoalComplete : Event
    {
        public SomeGoal goal;

        public EventGoalComplete(SomeGoal goal)
        {
            this.goal = goal;
        }
    }
}
