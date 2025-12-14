using UnityEngine;
using System.Collections;
using NUnit.Framework;
using Newtonsoft.Json;
using System.Collections.Generic;

namespace PJ
{
    public class UnitTests_Int64ToInt32JsonConverter
    {
        public class TestCodable
        {
            public Dictionary<string, object> tags = new();
        }

        [Test]
        public void TestConverter()
        {
            var test = new TestCodable();
            test.tags["test"] = 5;

            var jsonString = JsonConvert.SerializeObject(test);

            JsonSerializerSettings settings = new JsonSerializerSettings();

            // Convert any unrecognized ints parsed as Int64 to Int32
            settings.Converters.Add(new Int64ToInt32JsonConverter());

            var sut = JsonConvert.DeserializeObject<TestCodable>(jsonString, settings);

            Assert.AreEqual(5, (int)sut.tags["test"]);
            Assert.IsTrue(sut.tags["test"] is int);
        }
    }
}
