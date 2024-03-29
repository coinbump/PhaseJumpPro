﻿using UnityEditor;
using UnityEngine;
using PJ;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 12/28/22
*/
namespace PJ
{
    /// <summary>
    /// Warps a plane mesh with a sine wave across a specified axis
    /// </summary>
    public class SinMeshBuilder : SomeMeshBuilder
    {
        public Vector2Int meshSize = new Vector2Int(3, 3);
        public Vector2 worldSize = new Vector2(1.0f, 1.0f);
        public Axis faceAxis = Axis.Z;
        public Axis2D mapAxis = Axis2D.X;
        public Axis modifierAxis = Axis.Z;
        public float scale = 1.0f;
        public float animationSpeed = 0;

        protected float offset = 0;

        protected override Vector2 WorldSizeInternal
        {
            get => worldSize;
            set => worldSize = value;
        }

        public override Mesh BuildMesh()
        {
            var sinMap = new Graph.SinMap1DNode(true, offset);
            var graphBuilder = new Graph.SerialBuilder(sinMap);
            graphBuilder = graphBuilder.ToMap2D(Axis2D.X);
            var modifierNode = new Graph.PlaneMeshModifierNode(new PlaneMesh(meshSize, worldSize, faceAxis), modifierAxis, scale, true);

            graphBuilder.OutputTo("value", modifierNode, "map");

            modifierNode.Go();
            return modifierNode.planeMesh.mesh;
        }

        protected override void Update()
        {
            if (animationSpeed <= 0) { return; }

            offset += animationSpeed * Time.deltaTime;
            offset %= 1.0f;

            Build();
        }
    }
}
