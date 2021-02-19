using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars. Simple FX.
 * CODE REVIEW: 2.17.21
 */
namespace PJ
{
	/// <summary>
	/// Fades in over N duration time.
	/// </summary>
	public class FXFade : MonoBehaviour
	{
		public float duration = 1.0f;

		[Range(0, 1.0f)]
		public float startAlpha = 0.0f;

		[Range(0, 1.0f)]
		public float endAlpha = 1.0f;

		public bool isGhostWhileRunning = true; // If true, don't do collisions while fading
		public string interpolateType = "";

		[Tooltip("If true, remove the object when alpha reaches 0")]
		public bool cullAtFadeOut = false;

		protected Timer timer = new Timer();
		protected bool wasColliderEnabled = true;
		protected Interpolate interpolate = new InterpolateOutSquared();

		public FXFade()
        {
        }

		public FXFade(float duration, float startAlpha, float endAlpha)
        {
			this.duration = duration;
			this.startAlpha = startAlpha;
			this.endAlpha = endAlpha;
        }

		protected void Awake()
		{
			timer.duration = duration;

			var customInterpolate = Interpolate.factory.New(interpolateType);
			if (customInterpolate != null)
            {
				interpolate = customInterpolate;
            }

			var spriteRenderer = GetComponent<SpriteRenderer>();
			if (null == spriteRenderer) { return; }
			Color color = spriteRenderer.color;
			color.a = startAlpha;
			spriteRenderer.color = color;

			if (isGhostWhileRunning)
			{
				var collider = GetComponent<Collider2D>();
				if (null != collider)
				{
					wasColliderEnabled = collider.enabled;
					collider.enabled = false;
				}
			}
		}

		// Start is called before the first frame update
		void Start()
		{
		}

		// Update is called once per frame
		void Update()
		{
			timer.EvtUpdate(new TimeSlice(Time.deltaTime));

			var spriteRenderer = GetComponent<SpriteRenderer>();
			if (null == spriteRenderer) { return; }

			Color color = spriteRenderer.color;
			color.a = startAlpha + (endAlpha-startAlpha)*interpolate.Evaluate(timer.GetProgress());

			if (timer.IsFinished)
			{
				color.a = endAlpha;
				enabled = false;

				if (isGhostWhileRunning)
                {
					var collider = GetComponent<Collider2D>();
					if (null != collider)
					{
						collider.enabled = wasColliderEnabled;
					}
				}

				if (cullAtFadeOut && endAlpha == 0)
                {
					Destroy(this.gameObject);
					return;
                }
			}

			spriteRenderer.color = color;
		}
	}
}
