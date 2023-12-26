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
    /// Sets render sort order for child objects based on their order in the parent.
    /// Useful for a stack of things, like cards
    /// </summary>
    public class SpriteOrderer : WorldComponent
    {
        public int offset = 1;
        public bool isReverse = false;

        protected override void Start()
        {
            base.Start();

            ApplyOrder();
        }

        protected virtual void ApplyOrder()
        {
            var order = 0;

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

        protected int ApplyOrderTo(GameObject gameObject, int order)
        {
            if (gameObject.TryGetComponent(out SpriteRenderer spriteRenderer))
            {
                spriteRenderer.sortingOrder = order;
            }

            return order + Math.Abs(offset);
        }

#if UNITY_EDITOR
        public virtual void OnValidate()
        {
            ApplyOrder();
        }

        [CustomEditor(typeof(SpriteOrderer), true)]
        public class Editor : UnityEditor.Editor
        {
            public override void OnInspectorGUI()
            {
                DrawDefaultInspector();

                SpriteOrderer orderer = (SpriteOrderer)target;
                if (GUILayout.Button("Apply Order"))
                {
                    orderer.ApplyOrder();
                }
            }
        }
#endif
    }
}
