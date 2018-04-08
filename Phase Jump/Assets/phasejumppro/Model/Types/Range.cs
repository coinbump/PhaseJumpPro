using System.Collections;
using System.Collections.Generic;

/*
 * RATING: 5 stars. Simple class
 * CODE REVIEW: 4.7.18
 */
namespace PJ
{
	using IntRange = Range<int>;
	using FloatRange = Range<float>;

	/// <summary>
	/// Specifies a range of values
	/// </summary>
	public class Range<T>
	{
		T minValue;
		T maxValue;
	}
}
