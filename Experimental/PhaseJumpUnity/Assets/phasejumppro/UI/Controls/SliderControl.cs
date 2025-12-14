using System;
using UnityEngine;
using UnityEngine.EventSystems;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 6/10/23
PORTED TO: C++
*/
namespace PJ
{
    public class SliderControl : UIControl2D, SomeListener
    {
        public class ViewModifier2D : SpriteViewModifier2D
        {
            protected override Optional<Vector2> SpriteRendererSize(View2D view, SpriteRenderer spriteRenderer)
            {
                var result = base.SpriteRendererSize(view, spriteRenderer);
                if (null != result)
                {
                    var sliderControl = view as SliderControl;
                    return new(new(result.value.x, sliderControl.trackSize));
                }

                return result;
            }
        }

        public class EndDragThumbEvent : Event
        {
        }

        [Header("SLIDER PROPERTIES")]

        /// <summary>
        /// The object for the thumb (if null, the first child object is used)
        /// </summary>
        public GameObject thumb;

        /// <summary>
        /// Height of the track
        /// </summary>
        public float trackSize = 0.5f;

        /// <summary>
        /// Size of the end caps where the thumb can't reach
        /// </summary>
        public float endCapSize;

        public float minValue = 0;
        public float maxValue = 1.0f;

        public Broadcaster broadcaster = new();

        protected PublishedValue<float> value;

        protected WorldPosition dragStartInputPosition;
        protected Vector3 thumbStartLocalPosition;
        protected ThumbDragHandler thumbDragHandler;

        public override Optional<float> IntrinsicHeight
        {
            get
            {
                var thumb = Thumb;
                if (null != thumb)
                {
                    return new(RendererSize(thumb).y);
                }

                // FUTURE: support more complex layouts, like a thumb that is below the track
                return base.IntrinsicHeight;
            }
        }

        public PublishedValue<float> Value
        {
            get
            {
                if (null == this.value)
                {
                    this.value = new(minValue);
                    this.value.AddListener(this);
                    UpdateThumbPositionForValue(this.value.Value);
                }
                return value;
            }
        }

        protected GameObject Thumb
        {
            get
            {
                if (null != thumb) { return thumb; }
                foreach (Transform childTransform in transform)
                {
                    // Return the first child as the slider thumb
                    return childTransform.gameObject;
                }
                return null;
            }
        }

        /// <summary>
        /// Sends thumb drag events to track
        /// </summary>
        protected class ThumbDragHandler : SomeDragHandler
        {
            public SliderControl target;

            public override void OnDragStart(WorldPosition inputPosition)
            {
                base.OnDragStart(inputPosition);

                target.OnDragThumbStart(gameObject, inputPosition);
            }

            public override void OnDragEnd()
            {
                base.OnDragEnd();

                target.OnDragThumbEnd();
            }

            public override void OnDragUpdate(WorldPosition inputPosition)
            {
                target.OnDragThumbUpdate(gameObject, inputPosition);
            }
        }

        public SliderControl()
        {
            viewModifier = new ViewModifier2D();
        }

        protected override void Awake()
        {
            base.Awake();

            var thumb = Thumb;
            if (null == thumb)
            {
                Debug.Log("ERROR. Missing Thumb.");
                return;
            }

            thumbDragHandler = thumb.AddComponent<ThumbDragHandler>();
            thumbDragHandler.target = this;
        }

        public override void OnEvent(PJ.Event _event)
        {
            base.OnEvent(_event);

            Value.OnValueChange(_event, UpdateThumbPositionForValue);
        }

        protected void UpdateThumbPositionForValue(float value)
        {
            // Don't move the thumb while the user is dragging it.
            if (null != thumbDragHandler && thumbDragHandler.IsDragging)
            {
                return;
            }

            var thumb = Thumb;
            var minThumbX = MinThumbX(thumb);
            var maxThumbX = MaxThumbX(thumb);

            value = Mathf.Clamp(value, minValue, maxValue);
            float position = (value - minValue) / (maxValue - minValue);

            var localPosition = thumb.transform.localPosition;
            thumb.transform.localPosition = new Vector3(minThumbX + position * (maxThumbX - minThumbX), localPosition.y, localPosition.z);
        }

        public Vector2 TrackSize() => RendererSize(gameObject);

        public Vector2 RendererSize(GameObject target)
        {
            RendererTool rt = new(target);
            var worldSize = rt.WorldSize;

            if (null != worldSize)
            {
                var ws = rt.WorldSize.value;
                return new Vector2(ws.x * transform.localScale.x, ws.y * transform.localScale.y);
            }

            return Vector2.zero;
        }

        public void OnDragThumbStart(GameObject thumb, WorldPosition inputPosition)
        {
            dragStartInputPosition = inputPosition;
            thumbStartLocalPosition = thumb.transform.localPosition;
        }

        public void OnDragThumbEnd()
        {
            broadcaster.Broadcast(new EndDragThumbEvent());
        }

        protected float HalfTrackWidth(GameObject thumb)
        {
            var trackWidth = TrackSize().x;
            var thumbWidth = RendererSize(thumb).x;

            var halfTrackWidth = trackWidth / 2.0f - endCapSize - thumbWidth / 2.0f;
            return halfTrackWidth;
        }

        protected float MinThumbX(GameObject thumb) => -HalfTrackWidth(thumb);
        protected float MaxThumbX(GameObject thumb) => HalfTrackWidth(thumb);

        public void OnDragThumbUpdate(GameObject thumb, WorldPosition inputPosition)
        {
            var x = thumbStartLocalPosition.x + (inputPosition.x - dragStartInputPosition.x);
            var minThumbX = MinThumbX(thumb);
            var maxThumbX = MaxThumbX(thumb);
            x = Mathf.Clamp(x, minThumbX, maxThumbX);

            var newLocalPosition = new Vector3(x, thumb.transform.localPosition.y, thumb.transform.localPosition.z);
            thumb.transform.localPosition = newLocalPosition;

            var newValue = (newLocalPosition.x - minThumbX) / (maxThumbX - minThumbX);
            Value.Value = newValue;
        }

#if UNITY_EDITOR
        protected override void RenderGizmos(EditorUtils.RenderState renderState)
        {
            var trackSize = TrackSize();
            var centerLocal = new Vector3(-trackSize.x / 2.0f + endCapSize / 2.0f, 0, 0);
            var centerWorld = transform.TransformPoint(centerLocal);
            EditorUtils.DrawRect(centerWorld, endCapSize, trackSize.y, renderState);

            var centerLocal2 = new Vector3(trackSize.x / 2.0f - endCapSize / 2.0f, 0, 0);
            var centerWorld2 = transform.TransformPoint(centerLocal2);
            EditorUtils.DrawRect(centerWorld2, endCapSize, trackSize.y, renderState);
        }
#endif
    }
}
