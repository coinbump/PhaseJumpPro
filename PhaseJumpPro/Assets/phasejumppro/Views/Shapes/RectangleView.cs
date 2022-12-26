using System;
using UnityEngine;

namespace PJ
{
    /// <summary>
    /// A Rectangle shape view
    /// </summary>
    public class RectangleView : SomeShapeView
    {
        protected override void BuildShape()
        {
            var fillPart = AddPartObject<QuadMeshBuilder>(fillMaterial, 0);
            if (fillPart)
            {
                fillPart.name = "Rectangle2D Fill";
            }

            var framePart = AddPartObject<QuadFrameMeshBuilder>(strokeMaterial, 1);
            if (framePart)
            {
                framePart.name = "Rectangle2D Frame";
            }
        }
    }
}
