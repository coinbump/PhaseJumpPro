using System;
using NUnit.Framework;

namespace PJ
{
	public class UnitTests_SampleRecorder
	{
		private class TestSampleSource
		{
			public int value;
		}

		[Test]
		public void UnitTests()
		{
			var sampleSource = new TestSampleSource();
			sampleSource.value = 3;

            Func<int> getSample = delegate () { return sampleSource.value; };

            var recorder = new SampleRecorder<int>(getSample: getSample, sampleRate: 1.0f, maxSamplesCount: 2);
			Assert.AreEqual(recorder.samples.Count, 0);

			recorder.OnUpdate(new TimeSlice(1.0f));
			Assert.AreEqual(recorder.samples.Count, 1);

			sampleSource.value = 4;
			recorder.OnUpdate(new TimeSlice(1.0f));
			Assert.AreEqual(recorder.samples.Count, 2);
			Assert.AreEqual(recorder.samples[0].value, 3);
			Assert.AreEqual(recorder.samples[1].value, 4);

			sampleSource.value = 5;
			recorder.OnUpdate(new TimeSlice(1.0f));
			Assert.AreEqual(recorder.samples.Count, 2);
			Assert.AreEqual(recorder.samples[0].value, 4);
			Assert.AreEqual(recorder.samples[1].value, 5);
		}
	}
}
