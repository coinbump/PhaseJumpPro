using UnityEngine;
using System.Collections.Generic;

/*
 * RATING: 5 stars
 * Simple extension to standard Unity class
 * CODE REVIEW: 11/13/22
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
		public HashSet<Updatable> updatables = new HashSet<Updatable>();

		public T ParentComponent<T>() where T : class {
			var parent = transform.parent.gameObject;
			if (null == parent) { return null; }
            return parent.GetComponent<T>();
		}

        protected virtual void Awake()
        {
        }

        protected virtual void Start()
        {
        }

        protected virtual void Update()
        {
            var timeSlice = new TimeSlice(Time.deltaTime);
            OnUpdate(timeSlice);
        }

        public virtual void OnUpdate(TimeSlice time) {
            var iterUpdatables = new HashSet<Updatable>(updatables);
			var unfinishedUpdatables = new HashSet<Updatable>();

            foreach (Updatable updatable in iterUpdatables)
            {
                updatable.OnUpdate(time);

				if (!updatable.IsFinished) {
                    unfinishedUpdatables.Add(updatable);
                }
            }

            updatables = unfinishedUpdatables;
        }

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
