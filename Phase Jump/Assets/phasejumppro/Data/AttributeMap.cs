using System;
using System.Collections.Generic;
using NUnit.Framework;

/*
 * RATING: 5 stars. Modern, with Unit Tests
 * CODE REVIEW: 4.1.18
 */
namespace PJ {

	public class AttributeMap : Dictionary<string, Attribute> {
		void AddAttribute(string key, string value) {
			this.Add(key, new Attribute(key, value));
		}

		bool GetBoolVal(string key, bool defaultVal)
		{
			var attribute = this[key];
			if (null == attribute) { return defaultVal; }

			return attribute.value == "1" || attribute.value == "true" || attribute.value == "t";
		}

		int GetIntVal(string key, int defaultVal)
		{
			var attribute = this[key];
			if (null == attribute) { return defaultVal; }

			var result = Convert.ToInt32(attribute.value);
			return result;
		}

		Int64 GetInt64Val(string key, Int64 defaultVal)
		{
			var attribute = this[key];
			if (null == attribute) { return defaultVal; }

			var result = Convert.ToInt64(attribute.value);
			return result;
		}

		float GetFloatVal(string key, float defaultVal)
		{
			var attribute = this[key];
			if (null == attribute) { return defaultVal; }

			var result = (float)Convert.ToDouble(attribute.value);
			return result;
		}

		UInt32 GetUIntVal(string key, UInt32 defaultVal)
		{
			var attribute = this[key];
			if (null == attribute) { return defaultVal; }

			var result = Convert.ToUInt32(attribute.value);
			return result;
		}

		string GetStrVal(string key, string defaultVal)
		{
			var attribute = this[key];
			if (null == attribute) { return defaultVal; }

			var result = attribute.value;
			return result;
		}

		[Test]
		public void UnitTests()
		{
			var am = new AttributeMap();
			am.AddAttribute("a", "10");

			Assert.AreEqual(am.GetIntVal("a", 0), 10);
			Assert.AreEqual(am.GetInt64Val("a", 0), 10);
			Assert.AreEqual(am.GetUIntVal("a", 0), 10);
			Assert.AreEqual(am.GetStrVal("a", ""), "10");
			Assert.AreEqual(am.GetBoolVal("a", false), false);

			am.AddAttribute("f", "1.1");
			Assert.LessOrEqual(Math.Abs(am.GetFloatVal("f", 0) - 1.1f), .001f);

			am.AddAttribute("b", "t");
			Assert.AreEqual(am.GetBoolVal("b", false), true);
		}
	}

}
