using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using UnityEngine;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 4/24/22
 */
namespace PJ
{
	/// <summary>
	/// Uses ObservableCollection to notify when changes in a list occur (analagous to List<T>)
	/// </summary>
	/// <typeparam name="T"></typeparam>
	public class PublishedList<T> : PublishedEntity<ObservableCollection<T>>
    {
		public PublishedList()
		{
			value = new ObservableCollection<T>();
			value.CollectionChanged += OnListChange;
		}

		protected void OnListChange(object sender, NotifyCollectionChangedEventArgs args)
		{
			OnValueChange();
		}
	}
}
