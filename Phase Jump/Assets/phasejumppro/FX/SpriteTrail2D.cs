using UnityEngine;
using System.Collections.Generic;
using System;

/*
 * RATING: 5 stars.
 * CODE REVIEW: 2.19.21
 */
namespace PJ {
	/// <summary>
    /// Generates a trail of sprites that fade then cull behind a moving sprite
    /// Sprite trail objects will only be generated if the object's position changes
    /// </summary>
	public class SpriteTrail2D : MonoBehaviour
	{
		[Tooltip("Rate at which a new trail object is spawned")]
		public float spawnRate = .01f;

		[Tooltip("Rate at which a position samples are recorded")]
		public float sampleRate = 0.01f;

		[Tooltip("Max number of samples to record")]
		public int maxSamplesCount = 30;
		public float trailStartAlpha = 0.3f;
		public float fadeOutTime = 0.1f;

		protected class Sample
        {
			public Sprite sprite;
			public bool flipX;
			public bool flipY;
			public Vector3 position;
			public Vector3 localScale;
			public Quaternion rotation;

			public Sample(SpriteRenderer spriteRenderer, Transform transform)
            {
				sprite = spriteRenderer.sprite;
				flipX = spriteRenderer.flipX;
				flipY = spriteRenderer.flipY;
				position = transform.position;
				localScale = transform.localScale;
				rotation = transform.rotation;
			}
        };
		protected SampleRecorder<Sample> recorder;

		protected float spawnTimer = 0;

        void Awake()
        {
			Func<Sample> getSample = delegate () { return new Sample(GetComponent<SpriteRenderer>(), transform); };

			recorder = new SampleRecorder<Sample>(getSample: getSample, maxSamplesCount: maxSamplesCount, sampleRate: sampleRate);
		}

        void Update()
		{
			// Record next sample.
			recorder.EvtUpdate(new TimeSlice(Time.deltaTime));

			spawnTimer += Time.deltaTime;
			if (spawnTimer >= spawnRate)
            {
				spawnTimer = 0;

				var samples = recorder.samples;
				samples.Reverse();

				foreach (SampleRecorder<Sample>.Sample recorderSample in samples)
                {
					var sample = recorderSample.value;
					if (sample.position == transform.position) { continue; }

					SpawnSample(sample);

					// We just need the last sample before the character moved.
					recorder.DiscardSamples();

					break;
                }
            }
		}

		protected void SpawnSample(Sample sample)
        {
			var newObject = new GameObject();
			newObject.name = "Sprite Trail (" + gameObject.name + ")";
			newObject.transform.position = sample.position;
            newObject.transform.rotation = sample.rotation;
            newObject.transform.localScale = sample.localScale;

            var spriteRenderer = newObject.AddComponent<SpriteRenderer>();
			spriteRenderer.sprite = sample.sprite;
			spriteRenderer.flipX = sample.flipX;
			spriteRenderer.flipY = sample.flipY;

			var fxFade = newObject.AddComponent<FXFade>();
			fxFade.cullAtFadeOut = true;
			fxFade.duration = fadeOutTime;
			fxFade.startAlpha = trailStartAlpha;
			fxFade.endAlpha = 0.0f;
		}
	}
}
