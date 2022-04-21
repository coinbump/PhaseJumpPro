using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 4/13/22
 */
namespace PJ
{
	public class HueAnimation : SomeAnimation
	{
		/// <summary>
        /// Time for a full cyle across all hues
        /// </summary>
		public float cycleTime = 1.0f;

		[Range(0, 1.0f)]
		public float hue = 1.0f;

		[Range(0, 1.0f)]
		public float saturation = 1.0f;

		[Range(0, 1.0f)]
		public float value = 1.0f;

		protected override void Start()
		{
			base.Start();
		}

        protected override void OnValidate()
        {
			base.OnValidate();

			UpdateColor();
        }

        protected override void Update()
		{
			base.Update();

			//Debug.Log("Hue Animation Update");

			//Debug.Log("Hue Animation Calculate");
			
			var newHue = hue;
			newHue += Time.deltaTime / cycleTime;
			newHue %= 1.0f;

			//Debug.Log("Hue before: " + hue.ToString() + " After: " + newHue.ToString());

			hue = newHue;
			UpdateColor();
		}

		protected void UpdateColor()
		{
			var hueColor = Color.HSVToRGB(hue, saturation, value);
			multiRenderer.Color = hueColor;
		}
	}
}
