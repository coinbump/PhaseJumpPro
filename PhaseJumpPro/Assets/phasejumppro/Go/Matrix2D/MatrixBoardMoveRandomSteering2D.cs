using System;
using UnityEngine;

/*
 * RATING: 4 stars
 * Works, but could use more advanced behavior, like checking for blocked directions
 * CODE REVIEW: 4/24/22
 */
namespace PJ
{
    /// <summary>
    /// Move objects in matrix board at random (for testing, or idle animations)
    /// </summary>
    public class MatrixBoardMoveRandomSteering2D : WorldComponent
    {
        public float duration = 1.0f;

        protected SomeNormalRandom random = new UnityRandom();

        protected override void Update()
        {
            var runner = GetComponent<MatrixBoardView2D>();
            if (null == runner)
            {
                Debug.Log("Error. Matrix steering requires runner");
                return;
            }

            foreach (Transform childTransform in transform)
            {
                var node = childTransform.gameObject.GetComponent<MatrixNode2D>();
                if (null == node) { continue; }

                // FUTURE: remove directions that are blocked
                var directionChoice = random.Value * 7.0f;
                MapDirection direction = (MapDirection)Mathf.RoundToInt(directionChoice);

                runner.MovePiece(node.piece, direction, duration);
            }
        }
    }
}
