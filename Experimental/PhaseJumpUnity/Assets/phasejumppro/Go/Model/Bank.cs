using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 4/21/22
 */
namespace PJ
{
	using AccountMap = Dictionary<string, PublishedValue<int>>;

	public struct Price
	{
		public string currency;
		public int amount;

		public Price(string currency, int amount)
		{
			this.currency = currency;
			this.amount = amount;
		}
	}
		
	public static class CurrencyTypes {
		public const string Game = "currency_game";
		public const string Paid = "currency_paid";
	}

	/// <summary>
	/// Stores currency values
	/// 
	/// Freemium apps generally have 2 currency types:
	/// 1. Game. Achieved through in-game quests/tasks
	/// 2. Paid. Purchased via real world $, sometimes given for specific quests/tasks
	/// </summary>
	public class Bank
	{
		private AccountMap accounts = new AccountMap();
		public Broadcaster broadcaster = new Broadcaster();

		public int Balance(string currency) {
			if (accounts.TryGetValue(currency, out PublishedValue<int> amount)) {
				return amount.Value;
			}
			return 0;
		}

		public bool Withdraw(Price price) {
			if (!CanWithdraw(price)) { return false; }

			if (accounts.TryGetValue(price.currency, out PublishedValue<int> amount))
			{
				amount.Value = Math.Max(0, amount.Value - price.amount);
			}

			return true;
		}

		public bool CanWithdraw(Price price)
		{
			if (accounts.TryGetValue(price.currency, out PublishedValue<int> amount))
			{
				return price.amount > 0 && price.amount <= amount.Value;
			}
			return false;
		}

		public void Deposit(Price price) {
			if (price.amount < 0) { return; }

			if (accounts.TryGetValue(price.currency, out PublishedValue<int> amount))
			{
				amount.Value = amount.Value + price.amount;
			}
			else {
				var value = new PublishedValue<int>();
				value.Value = price.amount;
				accounts.Add(price.currency, value);
			}
		}

		public void Clear(string currency)
		{
			if (accounts.TryGetValue(currency, out PublishedValue<int> amount)) {
				amount.Value = 0;
			}
		}
	}
}
