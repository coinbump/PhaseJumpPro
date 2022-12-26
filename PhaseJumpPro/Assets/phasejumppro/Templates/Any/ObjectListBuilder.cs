using System;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 4/23/22
 */
namespace PJ
{
	/// <summary>
    /// Instantiates N of the object templates and adds them as children of this object
    /// </summary>
	public class ObjectListBuilder : WorldComponent
	{
		public enum Method
        {
			// Adds objects in order
			InOrder,

			// Adds objects at random
			Random
        }

		public Method method;
		public List<GameObject> objectList;

		public SomeRandom random = new UnityRandom();

		/// <summary>
        /// Add N objects on Start
        /// </summary>
		public int buildCount;

		protected override void Awake()
        {
        }

		protected override void Start()
        {
			if (objectList.Count <= 0) { return; }

			for (int i = 0; i < buildCount; i++)
            {
				int chooseIndex = i % objectList.Count;

				switch (method)
                {
					case Method.Random:
						chooseIndex = Mathf.RoundToInt(random.Value * (float)(objectList.Count - 1));
						break;
                }

				var choice = objectList[chooseIndex];
				Instantiate(choice, transform);
            }

			// Use a layout class to arrange these new objects after we're done
			var someLayout = GetComponent<SomeGameObjectsLayout>();
			if (null != someLayout)
            {
				someLayout.ApplyLayout();
			}
        }
	}
}
