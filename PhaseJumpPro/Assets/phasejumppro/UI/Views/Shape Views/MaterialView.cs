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
    /// A color shape view
    /// </summary>
    public class MaterialView : SomeShapeView
    {
        protected override void BuildShape()
        {
            AddPartObject<QuadMeshBuilder>("shape.material.fill", fillMaterial, 0);
        }
    }
}
