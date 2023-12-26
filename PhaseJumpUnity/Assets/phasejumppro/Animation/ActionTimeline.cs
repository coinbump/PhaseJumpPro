using UnityEngine;
using System;

/*
RATING: 5 stars
Has unit tests
CODE REVIEW: 12/18/22
*/
namespace PJ
{
    /// <summary>
    /// Perform actions at specific points in time
    /// Each action is defined by an ActionKeyframe
    /// </summary>
    public class ActionTimeline : Timeline
    {
        public override void OnUpdate(TimeSlice time)
        {
            var oldTime = this.time;

            base.OnUpdate(time);

            var newTime = this.time;
            var keyframes = KeyframesIn(new Range<float>(oldTime, newTime, Range<float>.Evaluate.Inclusive, Range<float>.Evaluate.Inside));

            foreach (var keyframe in keyframes)
            {
                var actionKeyframe = keyframe as ActionKeyframe;
                if (null != actionKeyframe)
                {
                    actionKeyframe.Go();
                }
            }
        }
    }
}
