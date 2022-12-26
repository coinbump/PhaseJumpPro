using System;
using System.Collections.Generic;
using UnityEngine;

/*
RATING: 5 stars
Has unit tests
CODE REVIEW: 12/17/22
*/
namespace PJ
{
    /// <summary>
    /// A timeline has a start time, duration, and keyframes
    /// Child nodes can be used as tracks for the timeline
    /// </summary>
    public class Timeline : AcyclicGraphNode<StandardEdgeModel>
    {
        protected float time;
        protected List<SomeKeyframe> keyframes = new();

        public float startTime;
        public float duration;

        public List<SomeKeyframe> Keyframes
        {
            get => keyframes;
        }

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
            var result = new List<SomeKeyframe>();

            foreach (var keyframe in keyframes)
            {
                if (range.IsInside(keyframe.time))
                {
                    result.Add(keyframe);
                }
            }

            return result;
        }
    }
}
