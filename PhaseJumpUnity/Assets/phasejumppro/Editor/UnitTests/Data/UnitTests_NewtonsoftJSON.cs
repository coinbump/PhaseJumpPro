using UnityEngine;
using System.Collections;
using NUnit.Framework;
using Newtonsoft.Json;
using System.Collections.Generic;

namespace PJ
{
    public class UnitTests_NewtonsoftJSON
    {
        public struct Test
        {
            public int intValue;
            public string stringValue;
            public float floatValue;
            public Dictionary<string, object> map;

            public Test(int test)
            {
                this.intValue = 5;
                this.stringValue = "test";
                this.floatValue = 3.5f;

                map = new();
                map["int"] = 50;
                map["string"] = "test_0";
                map["float"] = 35.5f;
            }
        }

        [Test]
        public void TestSerializeThenDeserializeDictionary()
        {
            var test = new Test(5);
            var jsonString = JsonConvert.SerializeObject(test);
            var sut = JsonConvert.DeserializeObject<Test>(jsonString);

            Assert.AreEqual(sut.intValue, test.intValue);
            Assert.AreEqual(sut.stringValue, test.stringValue);
            Assert.AreEqual(sut.floatValue, test.floatValue);

            foreach ((var key, var value) in sut.map)
            {
                Assert.AreEqual(test.map[key], value);
            }
        }

        [Test]
        public void TestDeserializeMissingFields()
        {
            // Missing intValue, stringValue
            var jsonString = "{\"floatValue\":3.5,\"map\":{\"int\":50,\"string\":\"test_0\",\"float\":35.5}}";
            var sut = JsonConvert.DeserializeObject<Test>(jsonString);

            Assert.AreEqual(sut.intValue, 0);
            Assert.AreEqual(sut.stringValue, null);
            Assert.AreEqual(sut.floatValue, 3.5f);
        }
    }
}
