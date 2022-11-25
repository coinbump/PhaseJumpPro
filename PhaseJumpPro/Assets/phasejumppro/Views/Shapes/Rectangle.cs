using System;
using UnityEngine;

namespace PJ
{
    namespace View
    {
        /// <summary>
        /// A Rectangle shape view
        /// </summary>
        public class Rectangle : Shape
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

                var framePart = AddPartObject<QuadFrameMeshBuilder>(strokeMaterial, 1);
                if (framePart)
                {
                    framePart.name = "Rectangle2D Frame";
                }
            }
        }
    }
}
