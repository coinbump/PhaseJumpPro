using UnityEngine;
using System.Collections.Generic;

/*
 * RATING: 5 stars
 * Simple extension to standard Unity class
 * CODE REVIEW: 3.6.22
 */
namespace PJ
{
	/// <summary>
	/// Provides common methods for a MonoBehaviour
	/// </summary>
	public class MonoBehaviour : UnityEngine.MonoBehaviour
	{
		/// <summary>
		/// Updatables are sent update events (for timers, animations, etc.)
		/// </summary>
		protected HashSet<Updatable> updatables = new HashSet<Updatable>();

#if UNITY_EDITOR
		protected virtual void RenderGizmos(EditorUtils.RenderState renderState)
		{

		}

		private void OnDrawGizmos()
		{
			RenderGizmos(EditorUtils.RenderState.Default);
		}

		private void OnDrawGizmosSelected()
		{
			RenderGizmos(EditorUtils.RenderState.Selected);
		}
#endif
	}
}
