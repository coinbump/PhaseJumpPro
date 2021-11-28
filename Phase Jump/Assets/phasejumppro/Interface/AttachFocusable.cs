using UnityEngine;
using System.Collections;

namespace PJ
{
    public class AttachFocusable : Focusable
    {
        public GameObject target;
        public GeometryTransform offTransform = GeometryTransform.defaultTransform;
        public GeometryTransform onTransform = GeometryTransform.defaultTransform;

        protected GameObject attachment = null;

        protected override void Start()
        {
            base.Start();

            UpdateAttachement();
        }

        protected override void Update()
        {
            base.Update();

            UpdateAttachement();
        }

        protected void UpdateAttachement()
        {
            var valveState = focusValve.ValveState;

            if (valveState == 0 && attachment != null)
            {
                GameObject.Destroy(attachment);
                return;
            }

            if (valveState > 0 && attachment == null)
            {
                attachment = Instantiate(target);
                attachment.transform.parent = transform;
            }

            if (null == attachment)
            {
                return;
            }

            var transformScale = offTransform.scale + (onTransform.scale - offTransform.scale) * valveState;
            attachment.transform.localScale = transformScale;

            var transformRotation = offTransform.rotation + (onTransform.rotation - offTransform.rotation) * valveState;
            attachment.transform.localRotation = Quaternion.Euler(transformRotation.x, transformRotation.y, transformRotation.z);

            var transformPosition = offTransform.position + (onTransform.position - offTransform.position) * valveState;
            attachment.transform.localPosition = transformPosition;

            // Fade in the attachment
            SpriteRenderer spriteRenderer = attachment.GetComponent<SpriteRenderer>();
            if (null == spriteRenderer) { return; }
            Color color = spriteRenderer.color;
            color.a = valveState;
            spriteRenderer.color = color;
        }
    }
}
