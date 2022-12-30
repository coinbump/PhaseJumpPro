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
    public partial class MatrixBoardView2D : View2D
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

        public MatrixBoard board;

        [Tooltip("Optional focus indicator")]
        public GameObject focusIndicator;

        protected Optional<Vector2Int> mouseFocusedCell;
        protected MouseDevice mouseDevice = new();

        protected override void Awake()
        {
            base.Awake();

            board = new MatrixBoard(matrixSize);
        }

        protected override void Start()
        {
            base.Start();

            foreach (Transform childTransform in transform)
            {
                var childNode = childTransform.gameObject.GetComponent<MatrixNode2D>();
                if (null == childNode) { continue; }

                Put(childNode, childNode.initialOrigin);
            }
        }

        public void RemoveAt(Vector2Int location, bool destroyGameObject = true)
        {
            if (destroyGameObject)
            {
                var gameObject = GameObjectAt(location);
                if (gameObject)
                {
                    Destroy(gameObject);
                }
            }
            board.RemovePieceAt(location);
        }

        public void RemoveAll()
        {
            for (int x = 0; x < matrixSize.x; x++)
            {
                for (int y = 0; y < matrixSize.y; y++)
                {
                    RemoveAt(new Vector2Int(x, y));
                }
            }
        }

        public bool Replace(MatrixNode2D node, Vector2Int origin)
        {
            var pieceLocations = board.PieceLocationsAt(origin, node.piece);
            if (null == pieceLocations) { return false; }

            foreach (var location in pieceLocations)
            {
                var worldNode = WorldNodeAt(location);
                if (worldNode)
                {
                    Destroy(worldNode);
                }
                board.RemovePieceAt(location);
            }

            return Put(node, origin);
        }

        public bool Put(MatrixNode2D node, Vector2Int origin)
        {
            if (null == node.piece)
            {
                Debug.Log("Error. Matrix node is missing a piece");
                return false;
            }

            var putResult = board.PutPiece(node.piece, origin);
            if (!putResult)
            {
                Debug.Log("Error. Can't place Matrix node at " + origin.ToString());
            }

            var nodePosition = NodePosition(node);
            node.transform.parent = transform;
            node.transform.localPosition = nodePosition;
            return putResult;
        }

        public override void OnUpdate(TimeSlice time)
        {
            base.OnUpdate(time);

            OnUpdateMouseInput();
        }

        public Vector2 CellSize
        {
            get
            {
                return WorldSize / (new Vector2(matrixSize.x, matrixSize.y));
            }
        }

        public Optional<Vector2Int> LocationAtWorldPosition(Vector2 worldPosition)
        {
            var topLeft = TopLeftWorldPosition(transform.position);

            // The Matrix works in right-hand coordinate space
            var viewPosition = new Vector2(worldPosition.x - topLeft.x, Mathf.Abs(worldPosition.y - topLeft.y));
            if (!IsViewPositionInside(viewPosition)) { return null; }

            var cell = LocationAtViewPosition(viewPosition);
            return new Optional<Vector2Int>(cell);
        }

        public Optional<Vector2Int> LocationAtLocalPosition(Vector2 localPosition)
        {
            return LocationAtWorldPosition(transform.TransformPoint(localPosition));
        }

        public Vector2Int LocationAtViewPosition(Vector2 viewPosition)
        {
            var column = (int)(viewPosition.x / CellSize.x);
            var row = (int)(viewPosition.y / CellSize.y);

            return new Vector2Int(column, row);
        }

        public Vector2 LocationToLocalPosition(Vector2Int cell)
        {
            var x = -(WorldSize.x / 2.0f) + CellSize.x / 2.0f + cell.x * CellSize.x;
            var y = (WorldSize.y / 2.0f) * Vector2.up.y + (CellSize.y / 2.0f + cell.y * CellSize.y) * Vector2.down.y;

            return new Vector2(x, y);
        }

        public Vector3 LocationToWorldPosition(Vector2Int cell)
        {
            var topLeft = TopLeftWorldPosition(transform.position);
            var x = topLeft.x + CellSize.x / 2.0f + cell.x * CellSize.x;
            var y = topLeft.y + (CellSize.y / 2.0f + cell.y * CellSize.y) * Vector2.down.y;

            return new Vector3(x, y, 0);
        }

        public MoveResult MovePiece(MatrixPiece piece, MapDirection direction, float duration)
        {
            if (null == piece) { return MoveResult.Fail; }

            var oldOrigin = piece.origin;
            var newOrigin = oldOrigin + direction.Offset();

            return MovePiece(piece, newOrigin, duration);
        }

        public virtual MoveResult MovePiece(MatrixPiece piece, Vector2Int newOrigin, float duration)
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
                node.moveAnimator = new(
                    new Vector2Interpolator(node.transform.localPosition, endPosition),
                    new(duration, AnimationCycleType.Once),
                    new SetBinding<Vector2>((Vector2 position) => node.transform.localPosition = position)
                );
                node.updatables.Add(node.moveAnimator);
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

            var topLeftCellPosition = LocationToLocalPosition(pieceOrigin);
            var bottomRightCellPosition = LocationToLocalPosition(pieceOrigin + new Vector2Int(pieceSize.x - 1, pieceSize.y - 1));

            var x = topLeftCellPosition.x + (bottomRightCellPosition.x - topLeftCellPosition.x) / 2.0f;
            var y = topLeftCellPosition.y + Mathf.Abs((bottomRightCellPosition.y - topLeftCellPosition.y) / 2.0f) * Vector2.down.y;

            return new Vector2(x, y);
        }

        public GameObject GameObjectAt(Vector2Int cell)
        {
            return WorldNodeAt(cell);
        }

        public GameObject WorldNodeAt(Vector2Int cell)
        {
            var piece = board.PieceAt(cell);
            if (null == piece) { return null; }

            var cellOwner = piece.owner;
            if (cellOwner.TryGetTarget(out object cellObject))
            {
                var matrixNode = cellObject as MatrixNode2D;
                if (matrixNode)
                {
                    return matrixNode.gameObject;
                }
            }

            return null;
        }
    }
}
