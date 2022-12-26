using System;
using TMPro;
using UnityEngine;

namespace PJ {
    /// <summary>
    /// Allows TextMeshPro components to be placed in a view layout
    /// </summary>
    public class TextView : View2D
    {
        protected override void OnFrameChange() {
            base.OnFrameChange();

            UpdateTextMesh();
        }

        protected void UpdateTextMesh() {
            if (TryGetComponent(out RectTransform rectTransform)) {
                rectTransform.SetSizeWithCurrentAnchors(RectTransform.Axis.Horizontal, Frame.size.x);
                rectTransform.SetSizeWithCurrentAnchors(RectTransform.Axis.Vertical, Frame.size.y);
            }
        }

        protected override void Awake() {
            base.Awake();

            UpdateTextMesh();
        }



        // TODO: this hasn't been tested.
        // TODO: support min, max, fixed height
    //     public override float PreferredHeight(float layoutSize)
    //     {
    //         if (TryGetComponent(out TextMeshPro textMesh)) {
    //             if (TryGetComponent(out RectTransform rectTransform)) {
    //                 var oldSize = rectTransform.rect.size;
    //                 rectTransform.SetSizeWithCurrentAnchors(RectTransform.Axis.Horizontal, layoutSize);

    //                 textMesh.ForceMeshUpdate();
    //                 var result = textMesh.bounds.size.y;

    //                 rectTransform.SetSizeWithCurrentAnchors(RectTransform.Axis.Horizontal, oldSize.x);

    //                 return result;
    //             }
    //         }
    //         return base.PreferredHeight(layoutSize);
    //     }
    }
}
