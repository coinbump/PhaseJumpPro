using System;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 STARS
 * TESTED AND WORKS
 * CODE REVIEW: 4/24/22
 */
namespace PJ
{
    /// <summary>
    /// Attach this to a gameObject whose children are MatrixNode2D nodes and are
    /// locked to matrix movement (staying within cells).
    /// When moving between cells, a move animation can give the impression of free movement
    /// </summary>
    public class MatrixBoardRunner2D : MonoBehaviour
    {
        public enum MoveResult
        {
            // Can't move, it's animating
            Animating,

            // Success
            Success,

            // Destination is blocked
            Blocked,

            // Error occurred
            Fail
        }

        public Vector2Int matrixSize = new Vector2Int(5, 5);
        public Vector2 worldSize = new Vector2(1.0f, 1.0f);

        public MatrixBoard board;

        protected virtual void Awake()
        {
            board = new MatrixBoard(matrixSize);
        }

        protected virtual void Start()
        {
            foreach (Transform childTransform in transform)
            {
                var childNode = childTransform.gameObject.GetComponent<MatrixNode2D>();
                if (null == childNode) { continue; }
                if (null == childNode.piece) {
                    Debug.Log("Error. Matrix node is missing a piece");
                    continue;
                }

                var putResult = board.PutPiece(childNode.piece, childNode.origin);
                if (!putResult)
                {
                    Debug.Log("Error. Can't place Matrix node at " + childNode.origin.ToString());
                }

                var nodePosition = NodePosition(childNode);
                childTransform.localPosition = nodePosition;
            }
        }

        public Vector2 CellSize
        {
            get
            {
                return worldSize/(new Vector2(matrixSize.x, matrixSize.y));
            }
        }

        public Vector2 CellPositionAt(Vector2Int location)
        {
            var x = -(worldSize.x / 2.0f) + CellSize.x / 2.0f + location.x * CellSize.x;
            var y = (worldSize.y / 2.0f) * Vector2.up.y + (CellSize.y / 2.0f + location.y * CellSize.y) * Vector2.down.y;

            return new Vector2(x, y);
        }

        public MoveResult MovePiece(MatrixPiece piece, MapDirection direction, float duration)
        {
            if (null == piece) { return MoveResult.Fail; }

            if (null == piece.owner || !piece.owner.TryGetTarget(out object pieceOwner))
            {
                Debug.Log("Error. Piece owner is missing");
                return MoveResult.Fail;
            }

            MatrixNode2D node = pieceOwner as MatrixNode2D;
            if (null == node)
            {
                Debug.Log("Error. Need MatrixNode2D for Matrix board");
                return MoveResult.Fail;
            }

            if (node.IsAnimating) { return MoveResult.Animating; }

            var oldOrigin = piece.origin;
            var newOrigin = oldOrigin + direction.Offset();

            if (newOrigin == oldOrigin) { return MoveResult.Fail; }

            var excludeList = new HashSet<MatrixPiece>();
            excludeList.Add(piece);
            if (board.IsPieceBlockedAt(newOrigin, piece, excludeList))
            {
                return MoveResult.Blocked;
            }

            board.RemovePiece(piece);
            board.PutPiece(piece, newOrigin);

            var endPosition = NodePosition(node);

            if (duration > 0)
            {
                node.moveAnimationCurve = new Optional<AnimationCurve<Vector2>>(new AnimationCurve<Vector2>());
                node.moveAnimationCurve.value.start = node.transform.localPosition;
                node.moveAnimationCurve.value.end = endPosition;

                node.moveAnimationTimer.duration = duration;
                node.moveAnimationTimer.Reset();
            }
            else
            {
                node.transform.localPosition = endPosition;
            }

            return MoveResult.Success;
        }

        public Vector2 NodePosition(MatrixNode2D node)
        {
            var pieceOrigin = node.piece.origin;
            var pieceSize = node.piece.Size;

            var topLeftCellPosition = CellPositionAt(pieceOrigin);
            var bottomRightCellPosition = CellPositionAt(pieceOrigin + new Vector2Int(pieceSize.x - 1, pieceSize.y -1));

            var x = topLeftCellPosition.x + (bottomRightCellPosition.x - topLeftCellPosition.x) / 2.0f;
            var y = topLeftCellPosition.y + Mathf.Abs((bottomRightCellPosition.y - topLeftCellPosition.y) / 2.0f) * Vector2.down.y;

            return new Vector2(x, y);
        }
    }    
}
