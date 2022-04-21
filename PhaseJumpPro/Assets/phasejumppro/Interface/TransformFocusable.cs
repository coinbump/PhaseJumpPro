using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace PJ
{
    /// <summary>
    /// Needed, because Unity.Transform is not meant to be created separately from a GameObject.
    /// </summary>
    [System.Serializable]
    public struct GeometryTransform
    {
        public Vector3 position;
        public Vector3 rotation;
        public Vector3 scale;

        public static GeometryTransform defaultTransform = new GeometryTransform(Vector3.zero, Vector3.zero, new Vector3(1.0f, 1.0f, 1.0f));

        public GeometryTransform(Vector3 position, Vector3 rotation, Vector3 scale)
        {
            this.position = position;
            this.rotation = rotation;
            this.scale = scale;
        }
    }

    public class TransformFocusable : Focusable
    {
        public GeometryTransform offTransform = GeometryTransform.defaultTransform;
        public GeometryTransform onTransform = GeometryTransform.defaultTransform;

        protected override void Start()
        {
            base.Start();

            UpdateTransform();
        }

        protected override void Update()
        {
            base.Update();

            UpdateTransform();
        }

        protected void UpdateTransform()
        {
            var valveState = focusValve.ValveState;

            var transformScale = offTransform.scale + (onTransform.scale - offTransform.scale) * valveState;
            transform.localScale = transformScale;

            var transformRotation = offTransform.rotation + (onTransform.rotation - offTransform.rotation) * valveState;
            transform.localRotation = Quaternion.Euler(transformRotation.x, transformRotation.y, transformRotation.z);

            var transformPosition = offTransform.position + (onTransform.position - offTransform.position) * valveState;
            transform.localPosition = transformPosition;
        }
    }
}
