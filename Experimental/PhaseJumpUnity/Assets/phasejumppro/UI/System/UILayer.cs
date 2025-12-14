using System;
using UnityEngine;

/*
RATING: 5 stars
Simple type
CODE REVIEW: 6/10/23
PORTED TO: C++
*/
namespace PJ
{
    /// <summary>
    /// Specifies which objects are the parent layers for Z-sorting
    /// (Sort Layers only work with sprites)
    /// </summary>
    [Serializable]
    public struct UILayer
    {
        public string id;
        public GameObject parent;

        /// <summary>
        /// If > 0, used for zStep to next layer
        /// </summary>
        public float zStep;

        /// <summary>
        /// (OPTIONAL) Scene identifier (Also known as "sceneName")
        /// Used when we load a scene as a UILayer
        /// </summary>
        [NonSerialized]
        public string sceneId;
    }
}
