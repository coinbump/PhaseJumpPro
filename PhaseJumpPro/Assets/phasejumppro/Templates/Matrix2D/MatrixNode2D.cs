using System;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 4/24/22
 */
namespace PJ
{
    /// <summary>
    /// Matrix nodes are constrained to cell locations, and can animate from one cell to another
    /// Examples: Match-3, Tetris, Ultima, etc.
    ///
    /// This should be used for Kinematic objects only, without colliders
    /// </summary>
    /// FUTURE: support HexNode2D, HexBoardRunner if needed
    public class MatrixNode2D : Node2D
    {
        [Space(10)]
        [Header("MatrixNode2D Properties")]
        public Vector2Int origin;

        // For editor
        public List<string> pieceShape = new List<string>();

        public MatrixPiece piece;

        public Optional<AnimationCurve<Vector2>> moveAnimationCurve;
        public Timer moveAnimationTimer = new Timer(1.0f, SomeTimed.RunType.RunOnce);

        public bool IsAnimating
        {
            get
            {
                return !moveAnimationTimer.IsFinished && null != moveAnimationCurve;
            }
        }

        protected override void Awake()
        {
            base.Awake();

            if (null == piece)
            {
                piece = new MatrixPiece(new Vector2Int(1, 1));
                if (!piece.BuildFromShape(pieceShape))
                {
                    // Default piece: a 1x1
                    piece.Fill();
                }
            }

            piece.owner = new WeakReference<object>(this);

            // Matrix nodes must be kinematic only
            rigidbody.isKinematic = true;
        }

        public override void OnUpdate(TimeSlice time)
        {
            base.OnUpdate(time);

            moveAnimationTimer.OnUpdate(time);

            if (null != moveAnimationCurve)
            {
                float progress = moveAnimationTimer.Progress;
                progress = moveAnimationCurve.value.transform.Transform(progress);

                var position = moveAnimationCurve.value.start + (moveAnimationCurve.value.end - moveAnimationCurve.value.start) * progress;
                transform.localPosition = position;

                if (moveAnimationTimer.IsFinished)
                {
                    moveAnimationCurve = null;
                }
            }
        }
    }
}
