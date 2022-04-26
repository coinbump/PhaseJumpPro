using System;

namespace PJ
{
    /// <summary>
    /// Type erasure for keyframes so they can be placed in a collection
    /// </summary>
    public class AnyAnimationKeyframe
    {
        public float time;
    }

    /// <summary>
    /// Key frame for an animatable attribute
    /// Keyframes may be interpolated (position, alpha) or discrete (state changes)
    /// </summary>
    public class AnimationKeyframe<T> : AnyAnimationKeyframe
    {
        public T value;

        public AnimationKeyframe(float time, T value)
        {
            this.time = time;
            this.value = value;
        }
    }
}
