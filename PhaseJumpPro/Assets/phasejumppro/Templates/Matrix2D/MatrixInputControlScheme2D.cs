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
    public class MatrixInputControlScheme2D : SomeInputControlScheme
    {
        public MapDirectionAxisLimit axisLimit;
        public float moveTime = .5f;
        public float deadZone = 0.025f;

        protected MatrixBoardRunner2D boardRunner;
        protected MatrixNode2D node;

        public void Start()
        {
            node = GetComponent<MatrixNode2D>();
            if (null == node)
            {
                Debug.Log("Error. Matrix Control Scheme requires a MatrixNode2D");
                return;
            }

            boardRunner = GetComponentInParent<MatrixBoardRunner2D>();
            if (null == boardRunner)
            {
                Debug.Log("Error. Matrix Control Scheme requires a Board Runner");
                return;
            }
        }

        public void Update()
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

            var angle = AngleUtils.Vector2ToDegreeAngle(axisVector);
            var direction = MapUtils.DegreeAngleToMapDirection(angle, axisLimit);

            var moveResult = boardRunner.MovePiece(node.piece, direction, moveTime);
            OnMoveResult(node, direction, moveResult);
        }

        protected virtual bool CanMove() => true;
        protected virtual void OnMoveResult(MatrixNode2D node, MapDirection direction, MatrixBoardRunner2D.MoveResult moveResult) { }
    }
}
