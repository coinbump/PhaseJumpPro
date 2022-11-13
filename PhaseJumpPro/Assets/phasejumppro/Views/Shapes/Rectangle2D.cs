using System;
using UnityEngine;

namespace PJ
{
    public class Rectangle2D : Shape2D
    {
        protected MeshRenderer meshRenderer;
        protected QuadFrameMeshBuilder meshBuilder;

        protected override void Start()
        {
            base.Start();

            var fillPart = AddPartObject<QuadMeshBuilder>(fillMaterial, 0);
            if (fillPart)
            {
                fillPart.name = "Rectangle2D Fill";
            }

            var framePart = AddPartObject<QuadFrameMeshBuilder>(strokeMaterial, -0.1f);
            if (framePart)
            {
                framePart.name = "Rectangle2D Frame";
            }
        }
    }
}
