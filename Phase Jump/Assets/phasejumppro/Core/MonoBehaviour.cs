using UnityEngine;
using System.Collections;

namespace PJ
{
	/// <summary>
	/// Provides common methods for a MonoBehaviour
	/// </summary>
	public class MonoBehaviour : UnityEngine.MonoBehaviour
	{
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
