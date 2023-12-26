using System;
using System.Collections.Generic;

namespace PJ
{
	/// <summary>
	/// Standard game model that suits many types of games
	/// Includes model objects for things like score (int), score (text), inventory, etc.
	/// </summary>
	public class GoStandardGameModel<StateType> : GoCore<StateType>
	{
		public PublishedValue<int> scoreValue { get; protected set; } = new PublishedValue<int>();
		public PublishedValue<string> scoreText { get; protected set; } = new PublishedValue<string>();

		public class Player
        {
			public HashSet<object> inventory = new HashSet<object>();
		}

		public List<Player> players = new List<Player>();

		public GoStandardGameModel()
        {
			players.Add(new Player());
        }
	}
}
