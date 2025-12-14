using System;
using UnityEngine;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 6/17/23
*/
namespace PJ
{
    /// <summary>
    /// A Rectangle shape view
    /// </summary>
    public class RectangleView : SomeShapeView
    {
        protected override void BuildShape()
        {
            AddPartObject<QuadMeshBuilder>("shape.rectangle.fill", fillMaterial, 0);
            AddPartObject<QuadFrameMeshBuilder>("shape.rectangle.frame", strokeMaterial, 1);
        }
    }
}
