using System.Collections;
using System.Collections.Generic;

namespace PJ
{
	/// <summary>
	/// Logic operation
	/// </summary>
	public abstract class LogicOperation {
		public abstract bool Transform(bool v1, bool v2);
	}

	public class LogicOrOperation : LogicOperation {
		public override bool Transform(bool v1, bool v2) {
			return v1 || v2;
		}
	}

	public class LogicAndOperation : LogicOperation
	{
		public override bool Transform(bool v1, bool v2)
		{
			return v1 && v2;
		}
	}

	public class LogicXorOperation : LogicOperation
	{
		public override bool Transform(bool v1, bool v2)
		{
			return v1 ^ v2;
		}
	}

}
