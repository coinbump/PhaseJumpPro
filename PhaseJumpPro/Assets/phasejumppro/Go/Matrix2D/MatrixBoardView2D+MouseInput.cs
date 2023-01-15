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
    public partial class MatrixBoardView2D
    {
        protected void OnUpdateMouseInput()
        {
            if (!mouseDevice.IsAvailable()) { return; }

            var worldPosition = mouseDevice.WorldPosition;
            if (null == worldPosition) { return; }

            mouseFocusedCell = LocationAtWorldPosition(new Vector2(worldPosition.x, worldPosition.y));

            if (focusIndicator)
            {
                if (null != mouseFocusedCell)
                {
                    var cellPosition = LocationToWorldPosition(mouseFocusedCell.value);

                    focusIndicator.transform.position = new Vector3(cellPosition.x, cellPosition.y, focusIndicator.transform.position.z);
                    var meshRenderer = focusIndicator.GetComponent<MeshRenderer>();
                    if (meshRenderer)
                    {
                        meshRenderer.enabled = true;
                    }
                }
                else
                {
                    var meshRenderer = focusIndicator.GetComponent<MeshRenderer>();
                    if (meshRenderer)
                    {
                        meshRenderer.enabled = false;
                    }
                }
            }
            // Debug.Log(mouseFocusedCell.ToString());
        }
    }
}
