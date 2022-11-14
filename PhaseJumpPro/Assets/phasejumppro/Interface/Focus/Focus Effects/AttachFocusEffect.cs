using UnityEngine;
using System.Collections;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 11/13/22
 */
namespace PJ
{
    /// <summary>
    /// When the object has focus, this attaches a child object to indicate focus
    /// and animates the child object's opacity and transform
    /// Example: add a focus frame to the object
    /// </summary>
    public class AttachFocusEffect : AnimatedFocusEffect
    {
        public GameObject attachment;
        public _Transform offTransform = _Transform.defaultTransform;
        public _Transform onTransform = _Transform.defaultTransform;

        protected GameObject _attachment = null;

        protected override void Start()
        {
            base.Start();

            UpdateAttachment();
        }

        protected override void Update()
        {
            base.Update();

            UpdateAttachment();
        }

        protected void UpdateAttachment()
        {
            var valveState = focusValve.ValveState;

            if (valveState == 0 && _attachment != null)
            {
                GameObject.Destroy(_attachment);
                return;
            }

            if (valveState > 0 && _attachment == null)
            {
                _attachment = Instantiate(attachment);
                _attachment.transform.parent = transform;
            }

            if (null == _attachment)
            {
                return;
            }

            var transformScale = offTransform.scale + (onTransform.scale - offTransform.scale) * valveState;
            _attachment.transform.localScale = transformScale;

            var transformRotation = offTransform.rotation + (onTransform.rotation - offTransform.rotation) * valveState;
            _attachment.transform.localRotation = Quaternion.Euler(transformRotation.x, transformRotation.y, transformRotation.z);

            var transformPosition = offTransform.position + (onTransform.position - offTransform.position) * valveState;
            _attachment.transform.localPosition = transformPosition;

            // Fade in the attachment
            SpriteRenderer spriteRenderer = _attachment.GetComponent<SpriteRenderer>();
            if (!spriteRenderer) { return; }
            Color color = spriteRenderer.color;
            color.a = valveState;
            spriteRenderer.color = color;
        }
    }
}
