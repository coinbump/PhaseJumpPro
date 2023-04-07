using UnityEngine;
using System.Collections;
using NUnit.Framework;
using System.Collections.Generic;
using PJ;

namespace PJ
{
#if _PJ_SQL_
    public class UnitTests_SQLStatement
    {
        [Test]
        public void TestAppendString()
        {
            SQLStatement sut = new("a");
            sut.AppendString("b");
            Assert.AreEqual("ab", sut.value);
        }

        [Test]
        public void TestAppendValue()
        {
            SQLStatement sut = new("a");
            sut.Append(new SQLValue(SQLValueType.Text, "b"));
            Assert.AreEqual("a'b'", sut.value);
        }

        [Test]
        public void TestAppendIdentifier()
        {
            SQLStatement sut = new("a");
            sut.AppendIdentifier("b");
            Assert.AreEqual("a[b]", sut.value);
        }
    }
#endif
}
