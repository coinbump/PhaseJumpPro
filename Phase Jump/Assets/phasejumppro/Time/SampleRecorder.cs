using System;
using System.Collections.Generic;

/*
 * RATING: 5 stars. Has unit test
 * CODE REVIEW: 2.17.21
 */
namespace PJ
{
	/// <summary>
	/// Samples variables via a lambda at specific intervals and records the value.
	/// Useful for animation (EXAMPLE: motion trail).
    /// NOTE: must call EvtUpdate for this to work.
	/// </summary>
	/// <typeparam name="Type"></typeparam>
	public class SampleRecorder<Type>
	{
		public Func<Type> getSample;
		public int maxSamplesCount = 1;
		public float sampleRate = 1.0f;

		protected float sampleTimer = 0;

		public SampleRecorder(Func<Type> getSample, float sampleRate, int maxSamplesCount)
		{
			this.getSample = getSample;
			this.maxSamplesCount = maxSamplesCount;
			this.sampleRate = sampleRate;
		}

		public class Sample
		{
			public Type value;
			public float lifeTime;

			public Sample(Type value)
			{
				this.value = value;
			}
		}
		public List<Sample> samples { get; protected set; } = new List<Sample>();

		public virtual void EvtUpdate(TimeSlice time)
		{
			foreach (Sample sample in samples)
			{
				sample.lifeTime += time.delta;
			}

			sampleTimer += time.delta;
			while (sampleTimer >= sampleRate)
			{
				sampleTimer -= sampleRate;

				samples.Add(NewSample());
			}

			while (samples.Count > maxSamplesCount)
			{
				samples.RemoveAt(0);
			}
		}

		public void DiscardSamples()
        {
			samples.Clear();
        }

		protected Type GetSampleValue()
		{
			if (null == getSample) { return default; }
			return getSample();
		}

		protected Sample NewSample()
		{
			return new Sample(GetSampleValue());
		}
	}
}
