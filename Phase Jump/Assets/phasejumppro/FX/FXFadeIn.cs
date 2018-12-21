using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 4 stars. Simple FX, could use interpolate customization.
 * CODE REVIEW: 12/19/18
 */
namespace PJ
{
	/// <summary>
	/// Fades in over N duration time.
	/// </summary>
	public class FXFadeIn : MonoBehaviour
	{
		public float duration = 1.0f;

		protected Timer timer = new Timer();

		protected void Awake()
		{
			timer.duration = duration;

			SpriteRenderer spriteRenderer = GetComponent<SpriteRenderer>();
			if (null == spriteRenderer) { return; }
			Color color = spriteRenderer.color;
			color.a = 0;
			spriteRenderer.color = color;
		}

		// Start is called before the first frame update
		void Start()
		{
		}

		// Update is called once per frame
		void Update()
		{
			timer.EvtUpdate(new TimeSlice(Time.deltaTime));

			SpriteRenderer spriteRenderer = GetComponent<SpriteRenderer>();
			if (null == spriteRenderer) { return; }

			var interpolate = new InterpolateOutSquared();	// FUTURE: add support to customize this if needed.

			Color color = spriteRenderer.color;
			color.a = interpolate.Transform(timer.GetProgress());

			if (timer.IsFinished)
			{
				color.a = 1.0f;
				enabled = false;
			}

			spriteRenderer.color = color;
		}
	}

}
