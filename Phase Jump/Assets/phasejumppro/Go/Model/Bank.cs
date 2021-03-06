﻿using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace PJ
{
	using AccountMap = Dictionary<string, ObservedValue<int>>;

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
			ObservedValue<int> amount;
			if (accounts.TryGetValue(currency, out amount)) {
				return amount.Value;
			}
			return 0;
		}

		public bool Withdraw(Price price) {
			if (!CanWithdraw(price)) { return false; }

			Price withdrawal = price;
			withdrawal.amount = -withdrawal.amount;
			Deposit(withdrawal);
			return true;
		}

		public bool CanWithdraw(Price price)
		{
			ObservedValue<int> amount;
			if (accounts.TryGetValue(price.currency, out amount))
			{
				return price.amount <= amount.Value;
			}
			return false;
		}

		public void Deposit(Price price) {
			ObservedValue<int> amount;
			if (accounts.TryGetValue(price.currency, out amount))
			{
				amount.Value = Math.Max(0, amount.Value + price.amount);
			}
			else if (price.amount > 0) {
				var value = new ObservedValue<int>();
				value.Value = price.amount;
				accounts.Add(price.currency, value);
			}
		}

		public void Clear(string currency)
		{
			ObservedValue<int> amount;
			if (accounts.TryGetValue(currency, out amount)) {
				amount.Value = 0;
			}
		}
	}
}

