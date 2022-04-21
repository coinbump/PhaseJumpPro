using UnityEngine;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 4/12/22
 */
namespace PJ
{
    /// <summary>
    /// Points at a target (with offset) or position
    /// </summary>
    /// FUTURE: support screen edge layout, bouncing arrow animation (need plugin animations tech first)
    public class Pointer2D : Node2D
    {
        public enum LayoutType {
            None,       // Don't change position
            // FUTURE: ScreenEdge  // Keep at screen edge, unless target position is onscreen
        }

        public GameObject target;

        /// <summary>
        /// Allows a parent object to handle rotation (use to change pivot point of arrow's rotation)
        /// </summary>
        public GameObject rotationObject;

        /// <summary>
        /// Absolute position if no target, otherwise, an offset to the target's center
        /// </summary>
        public Vector2 targetPosition = Vector2.zero;

        /// <summary>
        /// Parent object used as a reference for cutoff (arrow's parent)
        /// </summary>
        public GameObject parentObject;

        /// <summary>
        /// Distance to target at which the pointer disappears
        /// </summary>
        public float cutoffDistance = 1.0f;

        public LayoutType layoutType = LayoutType.None;

        protected override void Awake()
        {
            base.Awake();

            if (null == rotationObject)
            {
                rotationObject = gameObject;
            }
        }

        protected override void Start()
        {
            base.Start();
        }

        protected override void OnUpdate(TimeSlice time)
        {
            base.OnUpdate(time);

            var targetPosition = this.targetPosition;
            if (null != target)
            {
                targetPosition.x += target.transform.position.x;
                targetPosition.y += target.transform.position.y;
            }
            
            var degreeAngle = AngleUtils.Vector2ToDegreeAngle(new Vector2(targetPosition.x - transform.position.x, targetPosition.y - transform.position.y));
            rotationObject.transform.eulerAngles = new Vector3(0, 0, -degreeAngle);

            switch (layoutType)
            {
                case LayoutType.None:
                    break;
            }

            if (cutoffDistance > 0)
            {
                if (null == parentObject)
                {
                    Debug.Log("Error. Parent object required for cutoff");
                    return;
                }

                var distance = AngleUtils.Distance(parentObject.transform.position, targetPosition);
                Cutoff(distance <= cutoffDistance);
            }
        }

        protected void Cutoff(bool isCutoff)
        {
            if (null != spriteRenderer)
            {
                spriteRenderer.enabled = !isCutoff;
            }
        }
    }
}
