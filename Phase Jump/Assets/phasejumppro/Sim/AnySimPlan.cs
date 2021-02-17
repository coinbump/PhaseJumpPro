using System.Collections.Generic;

namespace PJ
{
	/// <summary>
	/// Any plan for building/crafting something in the sim
	/// </summary>

	public interface AnySimPlan
	{
		string Title { get; }
		string Description { get; }
		Dictionary<string, double> Components { get; }

		// Time value (duration) or work value (# of work elements required to complete)
		double BuildTime { get; }

		SimNode NewNode();
	}
}
