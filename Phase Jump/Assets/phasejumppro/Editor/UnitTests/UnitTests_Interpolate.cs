using UnityEngine;
using System.Collections;
using NUnit.Framework;

namespace PJ
{
	public class UnitTests_Interpolate
	{
		[Test]
		public void UnitTests()
		{
			{
				var i = new InterpolateLinear();
				Assert.LessOrEqual(i.Evaluate(1.0f), 1.0f);
			}
			{
				var i = new InterpolateSquared();
				Assert.LessOrEqual(i.Evaluate(.25f), .25f * .25f);
			}
			{
				var i = new InterpolateCubed();
				Assert.LessOrEqual(i.Evaluate(.25f), (.25f * .25f * .25f));
			}
			// No more unit tests needed
		}
	}
}
