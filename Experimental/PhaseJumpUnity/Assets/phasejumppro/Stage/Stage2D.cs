using System;
using UnityEngine;

/*
 * RATING: 4 stars
 * Tested and works. Could use more functionality
 * CODE REVIEW: 4/27/22
 */
namespace PJ
{
    /// <summary>
    /// A Stage is a mechanism for handling common animations for presenting and removing
    /// objects onscreen.
    /// A stage entity enters the stage by following a path to its position (with an optional fade)
    /// and exits the stage by reversing the direction of the path.
    /// Useful for sliding UI panels and visual novels
    /// </summary>
    public class Stage2D : WorldComponent
    {
        public void EnterAll()
        {
            // Stage entities have layout path parents, so we need to do a deep search
            EnterForChildren(transform);
        }

        protected void EnterForChildren(Transform transform)
        {
            foreach (Transform childTransform in transform)
            {
                var entity = childTransform.GetComponent<StageEntity>();
                if (null == entity) {
                    EnterForChildren(childTransform);
                    continue;
                }

                entity.EnterStage();
            }
        }

        public void ExitAll()
        {
            ExitForChildren(transform);
        }

        protected void ExitForChildren(Transform transform)
        {
            // Stage entities have layout path parents, so we need to do a deep search
            foreach (Transform childTransform in transform)
            {
                var entity = childTransform.GetComponent<StageEntity>();
                if (null == entity) {
                    ExitForChildren(transform);
                    continue;
                }

                entity.ExitStage();
            }
        }
    }
}
