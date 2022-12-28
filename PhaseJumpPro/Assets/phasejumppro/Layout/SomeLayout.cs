using UnityEditor;
using UnityEngine;
using System.Collections.Generic;

/*
 * RATING: 5 stars
 * Simple abstract class
 * CODE REVIEW: 12/27/22
 * PORTED TO: C++
 */
namespace PJ
{
    /// <summary>
    /// Defines the layout of child game objects
    /// (different than Unity's Layout Group for UI, which requires a Canvas)
    /// </summary>
    public abstract class SomeLayout : PJ.WorldComponent
    {
        /// <summary>
        /// If true, layout will be updated automatically.
        /// If false, you must update layout manually
        /// </summary>
        public bool autoApply = true;

        /// <summary>
        /// Return the bounds-size of the layout
        /// </summary>
        /// <returns></returns>
        public virtual Vector3 Size()
        {
            return Vector3.zero;
        }

        /// <summary>
        /// Arrange child objects according to the layout
        /// </summary>
        public abstract void ApplyLayout();

        protected override void Start()
        {
            base.Start();

            if (autoApply)
            {
                ApplyLayout();
            }
        }

        protected override void LateUpdate()
        {
            base.LateUpdate();

            if (autoApply)
            {
                ApplyLayout();
            }
        }

        public virtual Vector3 LayoutPositionAt(int index)
        {
            return Vector3.zero;
        }

#if UNITY_EDITOR
        protected virtual void OnValidate()
        {
            ApplyLayout();
        }

        [CustomEditor(typeof(SomeLayout), true)]
        public class Editor : UnityEditor.Editor
        {
            public override void OnInspectorGUI()
            {
                DrawDefaultInspector();

                SomeLayout layout = (SomeLayout)target;
                if (GUILayout.Button("Apply Layout"))
                {
                    layout.ApplyLayout();
                }
            }
        }
#endif
    }
}
