﻿using System;
using System.Collections.Generic;
using Godot;

/*
 * RATING: 5 stars
 * Simple class
 * CODE REVIEW: 4/9/22
 */
namespace PJ
{
    /// <summary>
    /// Cyclic nodes are managed by CyclicGraph
    /// </summary>
    /// <typeparam name="EdgeModel"></typeparam>
    public class CyclicGraphNode<EdgeModel> : SomeGraphNode<EdgeModel>
    {
        public override void AddEdge(EdgeModel model, SomeGraphNode<EdgeModel> toNode)
        {
            GD.Print("Error. Call CyclicGraph.AddEdge instead");
        }
    }
}
