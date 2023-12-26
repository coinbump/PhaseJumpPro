using System;
using System.Collections.Generic;
using System.Linq;
using Godot;

/*
RATING: 5 stars
Has unit tests
CODE REVIEW: 12/17/22
*/
namespace PJ
{
    /// <summary>
    /// A timeline has a start time, duration, and keyframes
    /// </summary>
    public class Timeline
    {
        /// <summary>
        /// Where the timeline is currently at
        /// </summary>
        protected float time;

        /// <summary>
        /// Keyframes that occur at specific times along the timeline
        /// </summary>
        protected List<SomeKeyframe> keyframes = new();

        /// <summary>
        /// (OPTIONAL) Start time in global time
        /// </summary>
        public float startTime;

        /// <summary>
        /// (OPTIONAL) Duration in global time
        /// </summary>
        public float duration;

        public List<SomeKeyframe> Keyframes => keyframes;

        public void Add(SomeKeyframe _keyframe)
        {
            for (int i = 0; i < keyframes.Count; i++)
            {
                var keyframe = keyframes[i];
                if (keyframe.time == _keyframe.time)
                {
                    keyframes[i] = _keyframe;
                    return;
                }

                if (keyframe.time > _keyframe.time)
                {
                    keyframes.Insert(i, _keyframe);
                    return;
                }
            }

            keyframes.Add(_keyframe);
        }

        public void AddAt(SomeKeyframe _keyframe, float time)
        {
            _keyframe.time = time;
            Add(_keyframe);
        }

        public void RemoveIn(Range<float> range)
        {
            keyframes.RemoveAll(keyframe => range.IsInside(keyframe.time));
        }

        public void RemoveAt(float time)
        {
            for (int i = 0; i < keyframes.Count; i++)
            {
                var keyframe = keyframes[i];
                if (keyframe.time == time)
                {
                    keyframes.RemoveAt(i);
                    return;
                }
            }
        }

        public SomeKeyframe KeyframeBefore(float time)
        {
            var reverseKeyframes = new List<SomeKeyframe>(keyframes);
            reverseKeyframes.Reverse();

            foreach (var keyframe in reverseKeyframes)
            {
                if (keyframe.time < time)
                {
                    return keyframe;
                }
            }

            return null;
        }

        public SomeKeyframe KeyframeAfterOrAt(float time)
        {
            foreach (var keyframe in keyframes)
            {
                if (keyframe.time >= time)
                {
                    return keyframe;
                }
            }

            return null;
        }

        public List<SomeKeyframe> KeyframesIn(Range<float> range)
        {
            return keyframes.Where(keyframes => range.IsInside(keyframes.time)).ToList();
        }

        public virtual void OnUpdate(TimeSlice time)
        {
            this.time += time.delta;
        }
    }
}
