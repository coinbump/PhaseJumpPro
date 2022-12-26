using UnityEngine;
using System.Collections;

/*
 * RATING: 5 stars
 * Simple animation
 * CODE REVIEW: 4/12/22
 */
namespace PJ
{
    public class RotateAnimation2D : WorldComponent
    {
        /// <summary>
        /// Angles per second
        /// </summary>
        public float anglesPerSecond = 360.0f;

        protected GoNode2D node;

        protected override void Start()
        {
            node = GetComponent<GoNode2D>();
        }

        protected override void Update()
        {
            if (null == node)
            {
                var eulerAngles = transform.eulerAngles;
                eulerAngles.z -= anglesPerSecond * Time.deltaTime;
                transform.localEulerAngles = eulerAngles;
                return;
            }

            node.Rotation = Angle.DegreesAngle(node.Rotation.Degrees + anglesPerSecond * Time.deltaTime);
        }
    }
}
