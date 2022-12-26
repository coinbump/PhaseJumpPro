using UnityEngine;

/*
 * RATING: 5 stars
 * Simple animation utility
 * CODE REVIEW: 11/13/22
 */
namespace PJ
{
    /// <summary>
    /// Interpolates its transform properties when it receives or loses focus
    /// </summary>
    public class ExtenderEffect2D : AnimatedEffect
    {
        [Header("ExtenderEffect2D Properties")]
        public float offValue = 1.0f;
        public float onValue = 0;
        public float offAlpha = 0;
        public float onAlpha = 1.0f;

        protected override void Start()
        {
            base.Start();

            UpdateAnimatableProperties();
        }

        protected override void UpdateAnimatableProperties()
        {
            base.UpdateAnimatableProperties();
            var valveState = valve.ValveState;

            if (TryGetComponent(out Extender2D extender))
            {
                extender.normalDistance = offValue + (onValue - offValue) * valveState;

                foreach (Transform childTransform in gameObject.transform)
                {
                    var multiRenderer = new MultiRenderer(childTransform.gameObject);
                    multiRenderer.Alpha = offAlpha + (onAlpha - offAlpha) * valveState;
                }
            }
        }
    }
}
