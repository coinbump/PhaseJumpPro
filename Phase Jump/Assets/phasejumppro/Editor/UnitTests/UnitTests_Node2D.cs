﻿using UnityEngine;
using System.Collections;
using NUnit.Framework;

namespace PJ
{
	public class UnitTests_Node2D
	{
		[Test]
		public void UnitTests()
		{
			var test = new Node2D();
			test.RotationAngle = 180.0f;
			Assert.AreEqual(test.RotationNormal, .5f);
			test.RotationAngle += 360.0f;
			Assert.AreEqual(test.RotationNormal, .5f);
		}
	}
}