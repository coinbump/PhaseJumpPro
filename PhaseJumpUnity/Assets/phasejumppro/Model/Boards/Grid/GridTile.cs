using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;

namespace PJ
{
	/// <summary>
	/// A tile that can be placed in a grid (EXAMPLE: diamond block in Bejeweled)
	/// </summary>
	public class GridTile : Updatable
	{
		public Vector2Int position;
		public Vector2Int size;

		public WeakReference<SomeGridBoard> board;  // Owner. null if not in a board.

		public bool IsFinished { get => false; }
		
        public virtual void OnUpdate(TimeSlice time)
        {
        }
    }
}
