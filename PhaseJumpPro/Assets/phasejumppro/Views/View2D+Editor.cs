using System;
using UnityEngine;
using UnityEditor;

namespace PJ {
    public partial class View2D {
#if UNITY_EDITOR
        protected override void OnValidate()
        {
            base.OnValidate();
            RootView().ApplyLayout();
        }

        protected override void RenderGizmos(EditorUtils.RenderState renderState)
        {
            EditorUtils.DrawRect(transform.position, WorldSize.x, WorldSize.y, renderState);
        }

        [CustomEditor(typeof(View2D), true)]
        public class Editor : UnityEditor.Editor
        {
            public override void OnInspectorGUI()
            {
                DrawDefaultInspector();

                View2D _target = (View2D)target;

                GUILayout.Label("Frame Origin: " + _target.frame.origin.x + ", " + _target.frame.origin.y);
                GUILayout.Label("Frame Size: " + _target.frame.size.x + ", " + _target.frame.size.y);

                if (GUILayout.Button("Apply Layout"))
                {
                    _target.ApplyLayout();
                }
            }
        }
#endif
    }
}
