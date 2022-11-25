using System;
using UnityEngine;

namespace PJ {
    namespace View {
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
        }
    }
}
