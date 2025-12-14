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

        /// <summary>
        /// Returns only the component from the parent object
        /// (GetComponentInParent traverses the hierarchy, starting from the object itself)
        /// </summary>
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

        /// <summary>
        /// In editor mode, use DestroyImmediate, otherwise use Destroy
        /// </summary>
        public void ContextDestroy(GameObject gameObject)
        {
#if UNITY_EDITOR
            DestroyImmediate(gameObject);
#else
            Destroy(gameObject);
#endif
        }

        public virtual void OnUpdate(TimeSlice time)
        {
            // Copy because collection can be modified during loop
            var iterUpdatables = new HashSet<Updatable>(updatables);

            foreach (Updatable updatable in iterUpdatables)
            {
                updatable.OnUpdate(time);
            }

            // Don't flush until update is finished (new updatables might have been added)
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

        public List<GameObject> ChildObjects()
        {
            List<GameObject> result = new();

            foreach (Transform childTransform in transform)
            {
                result.Add(childTransform.gameObject);
            }

            return result;
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
