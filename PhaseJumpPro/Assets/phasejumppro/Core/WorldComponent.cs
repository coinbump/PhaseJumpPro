using UnityEngine;
using System.Collections.Generic;

/*
 * RATING: 5 stars
 * Simple extension to standard Unity class
 * CODE REVIEW: 11/13/22
 */
namespace PJ
{
    public class _Component : UnityEngine.MonoBehaviour
    {
    }

    /// <summary>
    /// Provides common methods for a MonoBehaviour
    /// Named for consistency with the upcoming C++ version of PhaseJump
    /// </summary>
    public class WorldComponent : _Component
    {
        /// <summary>
        /// Updatables are sent update events (for timers, animations, etc.)
        /// </summary>
        public HashSet<Updatable> updatables = new HashSet<Updatable>();

        public T ParentComponent<T>() where T : class
        {
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

        protected virtual void LateUpdate() { }

        public virtual void OnUpdate(TimeSlice time)
        {
            // Copy because we might be adding or removing updatables during the update loop
            var iterUpdatables = new HashSet<Updatable>(updatables);

            foreach (Updatable updatable in iterUpdatables)
            {
                updatable.OnUpdate(time);
            }

            // Do a fresh pass for flush (new updatables might have been added during update loop)
            FlushFinishedUpdatables();
        }

        protected void FlushFinishedUpdatables()
        {
            var unfinishedUpdatables = new HashSet<Updatable>();

            foreach (Updatable updatable in updatables)
            {
                if (!updatable.IsFinished)
                {
                    unfinishedUpdatables.Add(updatable);
                }
            }

            updatables = unfinishedUpdatables;
        }

        public void DestroyOwner(float afterSeconds = 0)
        {
            Destroy(gameObject, afterSeconds);
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
