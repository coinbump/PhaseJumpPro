using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 12/29/22
 */
namespace PJ
{
    /// <summary>
    /// Spawn a new object every N seconds
    /// </summary>
    // FUTURE: add unit tests if needed
    public class Emitter : WorldComponent
    {
        public SpawnTable spawnTable = new();

        /// <summary>
        /// (OPTIONAL) The parent object to spawn the new objects into.
        /// </summary>
        public GameObject spawnParent;

        /// <summary>
        /// (OPTIONAL) Max # of emitted objects alive
        /// </summary>
        public int maxAlive;

        public SomeEmitsBuilder emitsBuilder;

        protected WeakGameObjectSet aliveEmits = new WeakGameObjectSet();

        /// <summary>
        /// Timer sends OnFinish to owner.
        /// </summary>
        protected class EmitFireTimer : Timer
        {
            protected WeakReference<Emitter> owner;

            public EmitFireTimer(Emitter owner, float duration) : base(duration, SomeRunner.RunType.KeepRunning)
            {
                this.owner = new WeakReference<Emitter>(owner);
            }

            protected override void OnFinish()
            {
                base.OnFinish();

                if (owner.TryGetTarget(out Emitter spawner))
                {
                    spawner.OnEmitTimerFinish();
                }
            }
        }

        /// <summary>
        /// (Optional) action when game object is spawned
        /// </summary>
        public Action<GameObject> spawnAction;

        [SerializeField]
        protected float emitTime = 0;

        protected List<Emit> delayedEmits = new();

        public Timer EmitTimer
        {
            get
            {
                if (null == emitTimer)
                {
                    emitTimer = new EmitFireTimer(this, emitTime);
                }
                return emitTimer;
            }
        }

        public float EmitTime
        {
            get => emitTime;
            set
            {
                emitTime = value;
                EmitTimer.duration = emitTime;
            }
        }

        protected Timer emitTimer;

        public override void OnUpdate(TimeSlice time)
        {
            base.OnUpdate(time);

            List<Emit> finishedEmits = new();
            List<Emit> unfinishedEmits = new();
            foreach (var delayedEmit in delayedEmits)
            {
                delayedEmit.delay -= time.delta;

                if (delayedEmit.delay <= 0)
                {
                    finishedEmits.Add(delayedEmit);
                }
                else
                {
                    unfinishedEmits.Add(delayedEmit);
                }
            }
            delayedEmits = unfinishedEmits;
            EmitWithEmits(finishedEmits);

            if (emitTime == 0) { return; }
            EmitTimer.OnUpdate(time);
        }

        public virtual bool CanSpawn()
        {
            if (maxAlive > 0)
            {
                aliveEmits.Refresh();
                if (aliveEmits.Count >= maxAlive)
                {
                    return false;
                }
            }

            return true;
        }

        public virtual List<GameObject> Emit()
        {
            var emitsBuilder = this.emitsBuilder != null ? this.emitsBuilder : new BasicEmitsBuilder();

            var result = EmitWithBuilder(emitsBuilder);
            OnEmit(result);
            return result;
        }

        protected virtual List<GameObject> EmitWithBuilder(SomeEmitsBuilder emitsBuilder)
        {
            if (null == emitsBuilder) { return new(); }

            var emits = emitsBuilder.BuildEmits();
            return EmitWithEmits(emits);
        }

        protected virtual List<GameObject> EmitWithEmits(List<Emit> emits)
        {
            List<GameObject> result = new();

            foreach (var emit in emits)
            {
                if (emit.delay > 0)
                {
                    delayedEmits.Add(emit);
                    continue;
                }

                var spawn = Spawn();
                if (!spawn) { break; }

                var velocity = EmitVelocity(emit);

                if (spawn.TryGetComponent(out Rigidbody2D rigidbody2D))
                {
                    rigidbody2D.velocity = velocity;
                    rigidbody2D.transform.localPosition += emit.offset;
                }
                else if (spawn.TryGetComponent(out Rigidbody rigidbody))
                {
                    rigidbody.velocity = velocity;
                    rigidbody.transform.localPosition += emit.offset;
                }

                result.Add(spawn);
            }

            return result;
        }

        protected virtual Vector3 EmitVelocity(Emit emit)
        {
            return emit.velocity;
        }

        protected virtual GameObject NewSpawn()
        {
            var template = spawnTable.NextSpawn();
            if (null == template) { return null; }
            return Instantiate(template, Vector3.zero, Quaternion.identity);
        }

        public virtual GameObject Spawn()
        {
            if (!CanSpawn()) { return null; }

            GameObject newSpawn = NewSpawn();
            if (null == newSpawn) { return null; }

            var spawnPosition = SpawnPositionFor(newSpawn);
            //Debug.Log("Spawn Position: " + spawnPosition.ToString());

            newSpawn.transform.parent = transform;
            newSpawn.transform.localPosition = spawnPosition;

            if (null == spawnParent)
            {
                newSpawn.transform.parent = null;
            }
            else
            {
                newSpawn.transform.parent = spawnParent.transform;
            }

            if (null != newSpawn && maxAlive > 0)
            {
                aliveEmits.Add(new HashedWeakReference<GameObject>(newSpawn));
            }

            OnSpawn(newSpawn);

            if (null != spawnAction)
            {
                spawnAction(newSpawn);
            }

            return newSpawn;
        }

        protected virtual void OnEmitTimerFinish()
        {
            Emit();
        }

        /// <summary>
        /// Spawn position for new spawn
        /// </summary>
        /// <param name="gameObject"></param>
        /// <returns>Returns local position to spawn</returns>
        protected virtual Vector3 SpawnPositionFor(GameObject gameObject) { return Vector3.zero; }

        protected virtual void OnSpawn(GameObject gameObject) { }
        protected virtual void OnEmit(List<GameObject> gameObjects) { }
    }
}
