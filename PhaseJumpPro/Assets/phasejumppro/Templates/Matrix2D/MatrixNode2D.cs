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
    public class MatrixNode2D : GoNode2D
    {
        [Space(10)]
        [Header("MatrixNode2D Properties")]
        public Vector2Int initialOrigin;

        // For editor
        public List<string> pieceShape = new List<string>();

        public MatrixPiece piece;

        public CycleAnimator<Vector2> moveAnimator;

        public bool IsAnimating
        {
            get
            {
                return null != moveAnimator && !moveAnimator.IsFinished;
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
            if (rigidbody)
            {
                rigidbody.isKinematic = true;
            }
        }
    }
}
