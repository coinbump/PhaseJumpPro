using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// TODO: needs to be modernized (See FXFadeIn)
namespace PJ
{
	/// <summary>
	/// Fades in over N duration time.
	/// </summary>
	public class FXFadeScaleIn : MonoBehaviour
	{
		public float duration = 1.0f;
		public float startScale = 1.0f;
		public bool isGhostWhileRunning;	// If true, make this non colliding kinematic while scaling.

		protected Timer timer = new Timer();
		protected bool wasKinematic;
		protected bool wasColliderEnabled;

		protected void Awake()
		{
			timer.duration = duration;

			SpriteRenderer spriteRenderer = GetComponent<SpriteRenderer>();
			if (null == spriteRenderer) { return; }
			Color color = spriteRenderer.color;
			color.a = 0;
			spriteRenderer.color = color;

			if (isGhostWhileRunning)
			{
				var r = GetComponent<Rigidbody>();
				if (r != null)
				{
					wasKinematic = r.isKinematic;
					r.isKinematic = true;
				}
				var c = GetComponent<Collider2D>();
				if (null != c)
				{
					wasColliderEnabled = c.enabled;
					c.enabled = false;
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

			SpriteRenderer spriteRenderer = GetComponent<SpriteRenderer>();
			if (null == spriteRenderer) { return; }

			Interpolate interpolate = new InterpolateOutSquared();	// FUTURE: add support to customize this if needed.

			Color color = spriteRenderer.color;
			float progress = timer.GetProgress();
			color.a = interpolate.Evaluate(progress);

			interpolate = new InterpolateCubed();

 			float scale = 1.0f + (startScale - 1.0f) * interpolate.Evaluate(1.0f-progress);
			transform.localScale = new Vector3(scale, scale, scale);

			if (timer.IsFinished)
			{
				color.a = 1.0f;
				enabled = false;

				if (isGhostWhileRunning)
				{
					var r = GetComponent<Rigidbody>();
					if (r != null)
					{
						r.isKinematic = wasKinematic;
					}
					var c = GetComponent<Collider2D>();
					if (null != c)
					{
						c.enabled = wasColliderEnabled;
					}
				}
			}

			spriteRenderer.color = color;
		}
	}

}
