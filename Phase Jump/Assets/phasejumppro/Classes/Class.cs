using System;
using UnityEngine;

namespace PJ
{
	/// <summary>
	/// Class objects are useful for defining modular behavior based on object type
	/// Example: all objects receive encoding and display behavior from their embedded class
	/// </summary>
	public class Class
	{
		public string Identifier { get; set; }
		public AnyFactory Factory { get; set; }
		public TypeTags TypeTags { get; set; } = new TypeTags();
		public Tags Tags { get; set; } = new Tags();

		// FUTURE: add functionality as needed
		public Class(string identifier, AnyFactory factory, TypeTags typeTags)
		{
			Identifier = identifier;
			Factory = factory;
			TypeTags = typeTags;
		}
	}
}
