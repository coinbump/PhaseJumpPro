using System;
using UnityEngine;

namespace PJ
{

    /// <summary>
    /// Organizes objects in Z space for all types
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
        /// Scene identifier (Also known as "sceneName")
        /// </summary>
        [NonSerialized]
        public string sceneId;
    }
}
