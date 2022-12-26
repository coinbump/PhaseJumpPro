﻿using UnityEngine;
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
				var i = new LinearInterpolate();
				Assert.LessOrEqual(i.Transform(1.0f), 1.0f);
			}
			{
				var i = new EaseInSquared();
				Assert.LessOrEqual(i.Transform(.25f), .25f * .25f);
			}
			{
				var i = new EaseInCubed();
				Assert.LessOrEqual(i.Transform(.25f), (.25f * .25f * .25f));
			}
			// No more unit tests needed
		}
	}
}
