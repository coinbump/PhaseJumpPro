using UnityEngine;
using System.Collections.Generic;
using System;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 4/13/22
 */
namespace PJ
{
	/// <summary>
    /// Displays progress as a series of empty/filled segments
    /// </summary>
    // FUTURE: support editor building of segments if needed
	public class SegmentProgressBar2D : MonoBehaviour
	{
		public enum ValueType
        {
			// Normal float for progress
			Normal,

			// Integer value for progress
			Int
        }

		public ValueType valueType = ValueType.Normal;
		public int segmentCount = 3;
		public GameObject emptySegment;
		public GameObject filledSegment;

		public float spacing = 0.1f;

		// <summary>
        // If > 0, layout the progress segments in rows
        // </summary>
		public int maxRowWidth = 0;

        // TODO: NOT YET IMPLEMENTED!
        public float rowSpacing = 0.1f;

        [SerializeField]
		protected float value = 0;

		protected List<GameObject> emptySegments = new List<GameObject>();
		protected List<SpriteRenderer> emptySpriteRenderers = new List<SpriteRenderer>();
		protected List<GameObject> filledSegments = new List<GameObject>();
		protected List<SpriteRenderer> filledSpriteRenderers = new List<SpriteRenderer>();

		public float Value
        {
			get => value;
			set
            {
				if (this.value != value)
				{
					this.value = value;
                    UpdateSegments();
				}
            }
        }

		/// <summary>
        /// Width of the node centers (not including node bounds)
        /// </summary>
		public float InnerWidth
        {
			get
			{
				var columnCount = segmentCount;
				if (maxRowWidth > 0)
				{
					columnCount = maxRowWidth;
				}

				var result = spacing * (columnCount - 1);
				return result;
			}
        }

		protected override void Start()
		{
			if (null == filledSegment)
            {
				Debug.Log("Error. Missing segment object for progress bar");
				return;
            }

			BuildSegments();
			UpdateSegments();
		}

		protected Vector3 PositionFor(int index)
        {
			var column = index;

			float y = 0;
			if (maxRowWidth > 0)
			{
				column = index % maxRowWidth;

				var row = Mathf.FloorToInt((float)index / (float)maxRowWidth);
				y = (row * rowSpacing) * Vector2.down.y;
			}

			var halfSize = InnerWidth / 2.0f;
			var x = -halfSize;
			x += column * spacing;

			return new Vector3(x, y, 0);
		}

		protected void BuildSegments()
        {
			// Remove old segments
			foreach (Transform childTransform in transform)
            {
				Destroy(childTransform.gameObject);
            }

			emptySegments.Clear();
			emptySpriteRenderers.Clear();
			filledSegments.Clear();
			filledSpriteRenderers.Clear();

			var size = spacing * (segmentCount - 1);

			for (int i = 0; i < segmentCount; i++)
            {
				var position = PositionFor(i);

				var filledSegment = Instantiate(this.filledSegment, transform);
				filledSegments.Add(filledSegment);
				filledSpriteRenderers.Add(filledSegment.GetComponent<SpriteRenderer>());
				filledSegment.transform.localPosition = position;

				if (null != this.emptySegment)
				{
					var emptySegment = Instantiate(this.emptySegment, transform);
					emptySegments.Add(emptySegment);
					emptySpriteRenderers.Add(emptySegment.GetComponent<SpriteRenderer>());
					emptySegment.transform.localPosition = position;
				}
			}
		}

		protected void UpdateSegments()
		{
			int numSegmentsFilled = 0;

			switch (valueType)
            {
				case ValueType.Int:
					numSegmentsFilled = Math.Min(filledSegments.Count, (int)value);
					break;
				case ValueType.Normal:
					numSegmentsFilled = (int)(filledSegments.Count * value);
					break;
			}

			for (int i = 0; i < filledSegments.Count; i++)
            {
				var isEmpty = (i + 1) > numSegmentsFilled;

				if (i < emptySpriteRenderers.Count)
				{
					emptySpriteRenderers[i].enabled = isEmpty;
				}

				if (i < filledSpriteRenderers.Count)
				{
					filledSpriteRenderers[i].enabled = !isEmpty;
				}
			}
        }
	}
}
