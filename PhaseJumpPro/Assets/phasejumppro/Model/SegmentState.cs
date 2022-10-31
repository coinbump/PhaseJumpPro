using System.Collections;
using System.Collections.Generic;

/*
 * RATING: 5 stars
 * Utility, with unit tests
 * CODE REVIEW: 6/22/22
 */
namespace PJ
{
    /// <summary>
    /// States are set based on which segment we are in
    /// Example: a day-state timer for morning, afternoon, night
    /// </summary>
    public class SegmentState<State>
    {
        public class Segment
        {
            public float size = 0.0f;
            public State state;

            public Segment(float size, State state)
            {
                this.size = size;
                this.state = state;
            }
        }

        /// <summary>
        /// Total size of all segments
        /// </summary>
        public float Size
        {
            get
            {
                float result = 0;

                foreach (Segment segment in segments)
                {
                    result += segment.size;
                }

                return result;
            }
        }

        public PublishedValue<State> state;

        public List<Segment> segments = new List<Segment>();

        public SegmentState(State initialState)
        {
            state = new PublishedValue<State>(initialState);
        }

        public float NormalValue
        {
            set {
                var atSize = Size * value;
                foreach (Segment segment in segments)
                {
                    if (atSize <= segment.size)
                    {
                        state.Value = segment.state;
                        return;
                    }
                    atSize -= segment.size;
                }
             }
        }
    }
}
