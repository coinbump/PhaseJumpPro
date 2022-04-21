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

		[SerializeField]
		protected float floatValue = 0;

		[SerializeField]
		protected int intValue = 0;

		protected List<GameObject> emptySegments = new List<GameObject>();
		protected List<SpriteRenderer> emptySpriteRenderers = new List<SpriteRenderer>();
		protected List<GameObject> filledSegments = new List<GameObject>();
		protected List<SpriteRenderer> filledSpriteRenderers = new List<SpriteRenderer>();

		public float FloatValue
        {
			get => floatValue;
			set
            {
				if (floatValue != value)
				{
					floatValue = value;
					if (ValueType.Normal == valueType)
					{
						UpdateSegments();
					}
				}
            }
        }

		public int IntValue
		{
			get => intValue;
			set
			{
				if (intValue != value)
				{
					intValue = value;
					if (ValueType.Int == valueType)
					{
						UpdateSegments();
					}
				}
			}
		}

		void Start()
		{
			if (null == emptySegment || null == filledSegment)
            {
				Debug.Log("Error. Missing segment object for progress bar");
				return;
            }

			BuildSegments();
			UpdateSegments();
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
			var halfSize = size / 2.0f;
			var x = -halfSize;

			for (int i = 0; i < segmentCount; i++)
            {
				var emptySegment = Instantiate(this.emptySegment, Vector3.zero, Quaternion.identity);
				var filledSegment = Instantiate(this.filledSegment, Vector3.zero, Quaternion.identity);

				emptySegment.transform.parent = transform;
				filledSegment.transform.parent = transform;

				emptySegments.Add(emptySegment);
                emptySpriteRenderers.Add(emptySegment.GetComponent<SpriteRenderer>());
				filledSegments.Add(filledSegment);
				filledSpriteRenderers.Add(filledSegment.GetComponent<SpriteRenderer>());

				var position = new Vector3(x, 0, 0);

				emptySegment.transform.localPosition = position;
				filledSegment.transform.localPosition = position;

				x += spacing;
			}
		}

		protected void UpdateSegments()
		{
			int numSegmentsFilled = 0;

			switch (valueType)
            {
				case ValueType.Int:
					numSegmentsFilled = Math.Min(filledSegments.Count, intValue);
					break;
				case ValueType.Normal:
					numSegmentsFilled = (int)(filledSegments.Count * floatValue);
					break;
			}

			for (int i = 0; i < filledSegments.Count; i++)
            {
				var isEmpty = (i + 1) > numSegmentsFilled;

				emptySpriteRenderers[i].enabled = isEmpty;
				filledSpriteRenderers[i].enabled = !isEmpty;
			}
        }
	}
}
