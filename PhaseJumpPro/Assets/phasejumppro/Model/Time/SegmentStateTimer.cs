using System;
using System.Collections;
using System.Collections.Generic;

/*
 * RATING: 5 stars
 * Simple wrapper class
 * CODE REVIEW: 6/22/22
 */
namespace PJ
{
	/// <summary>
	/// A timer that sets a state when it enters a segment (a slice of time within the time size)
	/// Example: a day-state timer for morning, afternoon, night
	/// </summary>
	public class SegmentStateTimer<State> : Timer where State : IEquatable<State>
    {
        public SegmentState<State> segmentState;

		public SegmentStateTimer(State initialState, float duration, SomeTimed.RunType type = SomeTimed.RunType.KeepRunning)
			: base(duration, type)
		{
            segmentState = new SegmentState<State>(initialState);
        }

        public override void OnUpdate(TimeSlice time)
        {
            base.OnUpdate(time);

            segmentState.NormalValue = Progress;
        }
    }
}
