using System.Collections;
using System.Collections.Generic;
using NUnit.Framework;

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

	public class UnitTests_LogicOperation {

		[Test]
		public void UnitTests() {
			Assert.AreEqual(false, new LogicOrOperation().Transform(false, false));
			Assert.AreEqual(true, new LogicOrOperation().Transform(true, false));
			Assert.AreEqual(false, new LogicAndOperation().Transform(true, false));
			Assert.AreEqual(true, new LogicAndOperation().Transform(true, true));
			Assert.AreEqual(false, new LogicXorOperation().Transform(false, false));
			Assert.AreEqual(true, new LogicXorOperation().Transform(true, false));
			Assert.AreEqual(false, new LogicXorOperation().Transform(true, true));
		}
	}

}
