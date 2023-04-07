using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using NUnit.Framework;

namespace PJ
{
    public class UnitTests_Bank
    {
        private class TestBank : Bank
        {
        }

        [Test]
        public void UnitTests()
        {
            var test = new TestBank();
            Assert.AreEqual(0, test.Balance(CurrencyTypes.Game));
            Assert.AreEqual(0, test.Balance(CurrencyTypes.Paid));

            test.Deposit(new Price(CurrencyTypes.Game, 10));
            Assert.AreEqual(10, test.Balance(CurrencyTypes.Game));
            Assert.IsFalse(test.CanWithdraw(new Price(CurrencyTypes.Game, 20)));
            Assert.IsFalse(test.Withdraw(new Price(CurrencyTypes.Game, 20)));

            Assert.AreEqual(10, test.Balance(CurrencyTypes.Game));

            Assert.IsTrue(test.Withdraw(new Price(CurrencyTypes.Game, 5)));
            Assert.AreEqual(5, test.Balance(CurrencyTypes.Game));

            test.Clear(CurrencyTypes.Game);
            Assert.AreEqual(0, test.Balance(CurrencyTypes.Game));
        }
    }
}

