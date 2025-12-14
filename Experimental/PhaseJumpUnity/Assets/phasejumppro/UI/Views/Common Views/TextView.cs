using System;
using TMPro;
using UnityEngine;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 6/10/23
*/
namespace PJ
{
    /// <summary>
    /// Allows TextMeshPro components to be placed in a view layout
    /// </summary>
    public class TextView : View2D
    {
        protected Optional<string> cachedText;
        protected TextMeshPro textMesh;

        protected TextMeshPro TextMesh
        {
            get
            {
                if (null == textMesh)
                {
                    textMesh = GetComponent<TextMeshPro>();
                }
                return textMesh;
            }
        }

        protected override void Awake()
        {
            base.Awake();

            UpdateTextTransform();
        }

        protected override void OnFrameChange()
        {
            base.OnFrameChange();

            UpdateTextTransform();
        }

        /// <summary>
        /// Must be called when the text changes, to update its cached values
        /// </summary>
        public void OnTextChange()
        {
            UpdateText();
        }

        protected void UpdateText()
        {
            var textMesh = TextMesh;
            if (null == textMesh) { return; }

            cachedText = new(textMesh.text);
            intrinsicWidth = null;
            intrinsicHeight = null;

            SetNeedsLayout();
        }

        public override Optional<float> PreferredWidthWithoutConstraints(Vector2 layoutSize)
        {
            CalculatePreferredSizes(layoutSize.x);
            return null != intrinsicWidth ? intrinsicWidth : base.PreferredWidthWithoutConstraints(layoutSize);
        }

        public override Optional<float> PreferredHeightWithoutConstraints(Vector2 layoutSize)
        {
            CalculatePreferredSizes(layoutSize.x);
            return null != intrinsicHeight ? intrinsicHeight : base.PreferredHeightWithoutConstraints(layoutSize);
        }

        protected void CalculatePreferredSizes(float layoutWidth)
        {
            CheckText();

            var needsWidth = isFixedWidth && null == intrinsicWidth;
            var needsHeight = isFixedHeight && null == intrinsicHeight;

            // If we know the width, we can caculate the height
            if (needsWidth || needsHeight)
            {
                // If asked to calculate the width, ignore the layout width and use the max width instead
                var testWidth = isFixedWidth ? ResolvedMaxWidth : layoutWidth;
                var textMesh = TextMesh;
                var preferredSize = textMesh.GetPreferredValues(cachedText.value, testWidth, 0);

                if (isFixedWidth)
                {
                    this.intrinsicWidth = new(preferredSize.x);
                }

                if (isFixedHeight)
                {
                    this.intrinsicHeight = new(preferredSize.y);
                }
            }
        }

        protected void UpdateTextTransform()
        {
            if (TryGetComponent(out RectTransform rectTransform))
            {
                rectTransform.SetSizeWithCurrentAnchors(RectTransform.Axis.Horizontal, Frame.size.x);
                rectTransform.SetSizeWithCurrentAnchors(RectTransform.Axis.Vertical, Frame.size.y);
            }
        }

        public override void OnUpdate(TimeSlice time)
        {
            base.OnUpdate(time);

            CheckText();
        }

        protected void CheckText()
        {
            var textMesh = TextMesh;
            if (null == textMesh) { return; }
            if (null == cachedText || textMesh.text != cachedText.value)
            {
                OnTextChange();
            }
        }

#if UNITY_EDITOR
        protected override void OnValidate()
        {
            // Update cached values before we let base ApplyLayout (always update in editor)
            OnTextChange();

            base.OnValidate();
        }
#endif
    }
}
