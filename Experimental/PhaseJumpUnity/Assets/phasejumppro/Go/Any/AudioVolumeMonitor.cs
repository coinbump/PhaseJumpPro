using System;
using UnityEngine;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 11/19/22
*/
namespace PJ
{
    /// <summary>
    /// Monitors audio volume at intervals and sets a value with the average volume over time
    /// </summary>
    public class AudioVolumeMonitor : WorldComponent
    {
        public AudioSource audioSource;

        public float updateDeltaTime = 0.1f;
        public int sampleDataSize = 1024;

        protected float accumulatedTime = 0f;

        /// <summary>
        /// Value is 0-1.0f
        /// </summary>
        protected float volume;

        protected float[] clipSampleData;

        public float Volume {
            get => volume;
        }

        protected override void Awake()
        {
            base.Awake();

            if (!audioSource)
            {
                Debug.LogError(GetType() + ".Awake: AudioSource is required.");
            }
            clipSampleData = new float[sampleDataSize];
        }

        public override void OnUpdate(TimeSlice time)
        {
            base.OnUpdate(time);

            accumulatedTime += time.delta;
            if (accumulatedTime >= updateDeltaTime)
            {
                accumulatedTime = 0f;

                audioSource.clip.GetData(clipSampleData, audioSource.timeSamples);

                var volume = 0f;
                foreach (var sample in clipSampleData)
                {
                    volume += Mathf.Abs(sample);
                }

                volume /= sampleDataSize;
                this.volume = volume;
            }
        }
    }
}
