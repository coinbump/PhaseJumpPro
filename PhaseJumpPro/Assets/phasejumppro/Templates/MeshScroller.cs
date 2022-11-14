using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 4/15/22
 */
namespace PJ
{
    /// <summary>
    /// Scrolls a mesh texture horizontally over time or manually via scrollProgress
    /// </summary>
    public class MeshScroller : MonoBehaviour
    {
        public enum Mode
        {
            // Scroll via a UV scroller mesh. Works for non-repeating textures, doesn't alter the material
            Mesh,

            // Scroll via a Texture offset (requires that the texture have repeat mode turned on)
            // LIMITATIONS: alters all materials via sharedMaterial
            TextureOffset
        }

        public Mode mode = Mode.Mesh;
        public Vector2 worldSize = new Vector2(1.0f, 1.0f);

        [Range(0, 1.0f)]
        public float scrollOffset;

        /// <summary>
        /// Duration of complete scroll
        /// </summary>
        public float scrollTime = 5.0f;

        protected float scrollProgress;
        protected float scrollTimer;
        protected MeshFilter meshFilter;
        protected MeshRenderer meshRenderer;
        protected UVScrollerMesh scrollerMesh;

        public float ScrollProgress
        {
            get => scrollProgress;
            set
            {
                if (scrollProgress != value)
                {
                    scrollProgress = value % 1.0f;
                    Build();
                }
            }
        }

        protected override void Start()
        {
            meshRenderer = gameObject.GetComponent<MeshRenderer>();
            if (null == meshRenderer)
            {
                Debug.Log("Error. Mesh scroller requires a mesh renderer");
                return;
            }

            Build();
        }

        protected override void Update()
        {
            if (scrollTime > 0)
            {
                scrollTimer += Time.deltaTime;
                scrollTimer %= scrollTime;
                ScrollProgress = scrollTimer / scrollTime;
            }
        }

        public void Build()
        {
            var progress = (scrollOffset + scrollProgress) % 1.0f;

            switch (mode)
            {
                case Mode.TextureOffset:
                    meshRenderer.sharedMaterial.SetTextureOffset("_MainTex", new Vector2(progress, 0));
                    break;
                case Mode.Mesh:
                    if (null == scrollerMesh)
                    {
                        scrollerMesh = new UVScrollerMesh(worldSize, 0);
                    }
                    if (null == meshFilter)
                    {
                        meshFilter = gameObject.GetComponent<MeshFilter>();
                        if (null == meshFilter)
                        {
                            Debug.Log("Error. Mesh scroller requires a mesh filter");
                            return;
                        }
                    }

                    scrollerMesh.ScrollValue = progress;
                    var mesh = scrollerMesh.mesh;
                    meshFilter.mesh = mesh;
                    break;
            }
        }

#if UNITY_EDITOR
        [CustomEditor(typeof(MeshScroller))]
        public class Editor : UnityEditor.Editor
        {
            public override void OnInspectorGUI()
            {
                DrawDefaultInspector();

                if (GUILayout.Button("Build"))
                {
                    MeshScroller meshscroller = (MeshScroller)target;
                    meshscroller.Build();
                }
            }
        }
#endif
    }
}
