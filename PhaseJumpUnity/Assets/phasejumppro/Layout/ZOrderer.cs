using UnityEditor;
using UnityEngine;
using System.Collections;
using System;

/*
 * RATING: 5 stars
 * Useful utility behavior for 2D games
 * CODE REVIEW: 1/16/23
 */
namespace PJ
{
    /// <summary>
    /// Sets Z position for child objects based on their order in the parent
    /// Useful for a stack of things, like cards
    /// </summary>
    public class ZOrderer : WorldComponent
    {
        public float offset = 0.001f;
        public bool isReverse = false;

        protected override void Start()
        {
            base.Start();

            ApplyOrder();
        }

        protected virtual void ApplyOrder()
        {
            var order = transform.position.z;

            if (isReverse)
            {
                for (int i = transform.childCount - 1; i >= 0; i--)
                {
                    var childTransform = transform.GetChild(i);
                    order = ApplyOrderTo(childTransform.gameObject, order);
                }
            }
            else
            {
                foreach (Transform childTransform in transform)
                {
                    order = ApplyOrderTo(childTransform.gameObject, order);
                }
            }
        }

        protected float ApplyOrderTo(GameObject gameObject, float order)
        {
            Vector3 position = gameObject.transform.position;
            position.z = order;
            gameObject.transform.position = position;

            return order + offset * Vector3.back.z;
        }

#if UNITY_EDITOR
        public virtual void OnValidate()
        {
            ApplyOrder();
        }

        [CustomEditor(typeof(ZOrderer), true)]
        public class Editor : UnityEditor.Editor
        {
            public override void OnInspectorGUI()
            {
                DrawDefaultInspector();

                ZOrderer orderer = (ZOrderer)target;
                if (GUILayout.Button("Apply Order"))
                {
                    orderer.ApplyOrder();
                }
            }
        }
#endif
    }
}
