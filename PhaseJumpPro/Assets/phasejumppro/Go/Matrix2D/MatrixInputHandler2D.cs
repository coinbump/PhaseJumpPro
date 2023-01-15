using UnityEngine;
using System.Collections;
using System;
using UnityEngine.InputSystem;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 4/24/22
 */
namespace PJ
{
    /// <summary>
    /// 2D x-y axis control scheme for matrix board objects
    /// </summary>
    public class MatrixInputHandler2D : SomeInputHandler
    {
        public MapDirectionAxisLimit axisLimit;
        public float moveTime = .5f;
        public float deadZone = 0.025f;

        protected MatrixBoardView2D boardRunner;
        protected MatrixNode2D node;

        protected override void Start()
        {
            node = GetComponent<MatrixNode2D>();
            if (null == node)
            {
                Debug.Log("Error. Matrix Control Scheme requires a MatrixNode2D");
                return;
            }

            boardRunner = GetComponentInParent<MatrixBoardView2D>();
            if (null == boardRunner)
            {
                Debug.Log("Error. Matrix Control Scheme requires a Board Runner");
                return;
            }
        }

        protected override void Update()
        {
        }

        public void OnInputMove(InputAction.CallbackContext context)
        {
            if (!CanMove()) { return; }
            if (null == node || null == boardRunner) { return; }

            //Debug.Log("Input Move: " + context.ToString());
            var axisVector = context.ReadValue<Vector2>();

            var distance = AngleUtils.Distance(Vector2.zero, axisVector);
            if (distance <= deadZone)
            {
                return;
            }

            var angle = new Angle(axisVector);
            var direction = MapUtils.AngleToMapDirection(angle, axisLimit);

            var moveResult = boardRunner.MovePiece(node.piece, direction, moveTime);
            OnMoveResult(node, direction, moveResult);
        }

        protected virtual bool CanMove() => true;
        protected virtual void OnMoveResult(MatrixNode2D node, MapDirection direction, MatrixBoardView2D.MoveResult moveResult) { }
    }
}
