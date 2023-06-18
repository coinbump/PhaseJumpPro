using System;
using System.Collections.Generic;
using UnityEngine;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 1/14/23
*/
namespace PJ
{
    /// <summary>
    /// A specialized tool that handles drag events
    /// </summary>
    public class MarqueeSelectTool2D : SomeDragCanvasTool2D
    {
        /// <summary>
        /// Object template for drawing the marquee (with a mesh builder)
        /// </summary>
        public GameObject marqueeObject;

        protected GameObject dragMarqueeObject;
        protected SomeMeshBuilder dragMeshBuilder;

        public override void OnDragStart(ToolCanvas2D canvas, ViewPosition inputPosition)
        {
            base.OnDragStart(canvas, inputPosition);
            BuildDragMarquee(canvas);
            UpdateDragMarquee(canvas, startDragPosition, inputPosition);
        }

        public override void OnDragUpdate(ToolCanvas2D canvas, ViewPosition inputPosition)
        {
            base.OnDragUpdate(canvas, inputPosition);

            UpdateDragMarquee(canvas, startDragPosition, inputPosition);

            var worldStartPosition = canvas.ViewToWorldPosition(startDragPosition);
            var worldEndPosition = canvas.ViewToWorldPosition(inputPosition);

            var selectHandlers = new HashSet<SelectHandler>();
            var colliders = Physics2D.OverlapAreaAll(worldStartPosition, worldEndPosition);
            foreach (var collider in colliders)
            {
                if (collider.TryGetComponent(out SelectHandler selectHandler))
                {
                    selectHandlers.Add(selectHandler);
                }
            }
            UISystem.Selection = selectHandlers;
        }

        public override void OnDragEnd()
        {
            base.OnDragEnd();

            Destroy(dragMarqueeObject);
            dragMarqueeObject = null;
            dragMeshBuilder = null;
        }

        protected virtual void UpdateDragMarquee(ToolCanvas2D canvas, ViewPosition startPosition, ViewPosition endPosition)
        {
            var worldStartPosition = canvas.ViewToWorldPosition(startPosition);
            var worldEndPosition = canvas.ViewToWorldPosition(endPosition);
            var worldSize = new Vector2(Mathf.Abs(endPosition.x - startPosition.x), Mathf.Abs(endPosition.y - startPosition.y));
            dragMeshBuilder.WorldSize2D = worldSize;

            var isXInverted = worldEndPosition.x < worldStartPosition.x;
            var isYInverted = worldEndPosition.y > worldStartPosition.y;

            // Debug.Log("isXInverted: " + isXInverted.ToString() + " isYInverted: " + isYInverted.ToString());

            dragMeshBuilder.transform.position = new Vector3(
                !isXInverted ? worldStartPosition.x + worldSize.x / 2.0f : worldEndPosition.x + worldSize.x / 2.0f,
                !isYInverted ? worldStartPosition.y + (worldSize.y / 2.0f) * Vector2.down.y : worldEndPosition.y + (worldSize.y / 2.0f) * Vector2.down.y,
                canvas.transform.position.z - 0.1f
            );
        }

        protected virtual void BuildDragMarquee(ToolCanvas2D canvas)
        {
            if (null == marqueeObject) { return; }
            if (null != dragMarqueeObject) { return; }

            dragMarqueeObject = Instantiate(marqueeObject, canvas.transform);
            dragMeshBuilder = dragMarqueeObject.GetComponent<SomeMeshBuilder>();
        }
    }
}
