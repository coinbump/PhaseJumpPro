using System.Collections;
using System.Collections.Generic;

/*
 * RATING: 5 stars
 * Simple utility wrappers
 * CODE REVIEW: 4/13/22
 */
namespace PJ
{
	/// <summary>
	/// Logic operation
	/// </summary>
	public abstract class LogicTransform : SomeMap<(bool, bool), bool> {
		public abstract bool Transform(bool v1, bool v2);

        public override bool ValueFor((bool, bool) key)
        {
			return Transform(key.Item1, key.Item2);
		}
    }

	public class LogicOrTransform : LogicTransform {
		public override bool Transform(bool v1, bool v2) {
			return v1 || v2;
		}
	}

	public class LogAndTransform : LogicTransform
	{
		public override bool Transform(bool v1, bool v2)
		{
			return v1 && v2;
		}
	}

	public class LogicXorTransform : LogicTransform
	{
		public override bool Transform(bool v1, bool v2)
		{
			return v1 ^ v2;
		}
	}
}
