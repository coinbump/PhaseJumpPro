using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 4/27/22
 */
namespace PJ
{
    /// <summary>
    /// Base class for emitters. Provides a spawn table and max alive limiter
    /// </summary>
    public abstract class SomeEmitter : SpawnTable
    {
        /// <summary>
        /// Max # of emitted objects alive
        /// </summary>
        public int maxAlive;

        protected WeakGameObjectSet aliveEmits = new WeakGameObjectSet();

        protected virtual void Awake()
        {
        }

        protected override void Start()
        {
            base.Start();
        }

        protected virtual void Update()
        {
            OnUpdate(new TimeSlice(Time.deltaTime));
        }

        protected virtual void OnUpdate(TimeSlice time)
        {
        }

        public virtual GameObject Spawn()
        {
            GameObject spawn = NextSpawn();
            if (null == spawn) { return null; }

            GameObject result = Instantiate(spawn, transform.position, Quaternion.identity);

            return result;
        }

        virtual protected bool CanEmit()
        {
            aliveEmits.Refresh();
            if (maxAlive > 0 && aliveEmits.Count >= maxAlive)
            {
                return false;
            }

            return true;
        }

        public virtual GameObject Emit()
        {
            if (!CanEmit()) {
                //Debug.Log("Can't Emit Alive Emits: " + aliveEmits.Count.ToString());
                return null;
            }

            var result = Spawn();
            if (null != result && maxAlive > 0)
            {
                aliveEmits.Add(new HashedWeakReference<GameObject>(result));
            }

            return result;
        }
    }
}
