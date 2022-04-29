using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace PJ
{
	public enum UpdateType
	{
		Default, Fixed
	}

	/// <summary>
	/// Provides utility methods for simplifying common game scenarios
	/// </summary>
	public abstract class SomeNode : MonoBehaviour, SomeStateListener<string>, SomeListener, Identifiable
	{
		public enum CullType
		{
			Invisible, ZeroAlpha
		}

		public string identifier;

		[NonSerialized]
		public Tags tags = new Tags();

		public string initialState;

		[Serializable]
		public struct NodeTag
		{
			public string name;
			public string value;    // Type is inferred from value
		}

		public string Identifier { get => identifier; set => identifier = value; }
		public abstract bool IsKinematic { get; }

		/// <summary>
		/// Defines key-value pairs for object ("health: 10", "energy: 20", etc.)
		/// </summary>
		public List<NodeTag> valueTags = new List<NodeTag>();

		/// <summary>
        /// Defines type values for object ("enemy", "hero", "bullet", etc.)
        /// </summary>
		public List<string> typeTags = new List<string>();

		public List<CullType> cullTypes = new List<CullType>();

		/// <summary>
		/// Updatables are sent update events (for timers, animations, etc.)
		/// </summary>
		protected HashSet<Updatable> updatables = new HashSet<Updatable>();

		/// <summary>
		/// Handles state machine.
		/// </summary>
		protected class Core<StateType> : PJ.Core<StateType>
		{
			public WeakReference<SomeStateListener<StateType>> Owner { get; }

			public Core(SomeStateListener<StateType> owner)
			{
				Owner = new WeakReference<SomeStateListener<StateType>>(owner);
			}

			protected override void OnStateChange(StateMachine<StateType> inStateMachine)
			{
				base.OnStateChange(inStateMachine);

				if (!Owner.TryGetTarget(out SomeStateListener<StateType> owner)) { return; }
				if (null == owner) { return; }

				owner.OnStateChange(inStateMachine);
			}

			protected override void OnStateFinish(StateMachine<StateType> inStateMachine)
			{
				base.OnStateFinish(inStateMachine);

				if (!Owner.TryGetTarget(out SomeStateListener<StateType> owner)) { return; }
				if (null == owner) { return; }

				owner.OnStateFinish(inStateMachine);
			}
		}

		/// <summary>
        /// String type states are used by default, but if you prefer enums, create your own core
        /// </summary>
		protected SomeCore core;

		public MultiRenderer multiRenderer;

		protected virtual void Awake()
		{
			core = new Core<string>(this);
			tags = new Tags();
			multiRenderer = new MultiRenderer(gameObject);
		}

		protected virtual void Start()
        {
			foreach (NodeTag tag in valueTags)
			{
				tags.Add(tag.name, tag.value);
			}

			var stringCore = core as Core<string>;
			if (null != stringCore)
			{
				stringCore.State = initialState;
			}
		}

		protected virtual void OnValidate()
		{
		}

		protected virtual void Update()
		{
			if (cullTypes.Contains(CullType.ZeroAlpha) && multiRenderer.Color.a == 0)
			{
				Destroy(gameObject);
				return;
			}

			UpdateNode(UpdateType.Default);

			var timeSlice = new TimeSlice(Time.deltaTime);
			var iterUpdatables = new HashSet<Updatable>(updatables);
			foreach (Updatable updatable in iterUpdatables)
            {
				updatable.OnUpdate(timeSlice);
            }
			core.OnUpdate(timeSlice);
		}

		protected virtual void FixedUpdate()
		{
			UpdateNode(UpdateType.Fixed);
		}

		protected virtual void UpdateNode(UpdateType updateType)
		{
			switch (updateType)
            {
				case UpdateType.Default:
					OnUpdate(new TimeSlice(Time.deltaTime));
					break;
				case UpdateType.Fixed:
					break;
            }
		}

		protected virtual void OnUpdate(TimeSlice time)
        {
        }

		/// <summary>
        /// Auto-cull off-camera objects if needed
        /// </summary>
		protected virtual void OnBecameInvisible()
		{
			if (cullTypes.Contains(CullType.Invisible)) {
				Destroy(gameObject);
			}
		}

		public bool HasTypeTag(string name) => typeTags.Contains(name);

		public virtual void OnStateChange(StateMachine<string> state)
		{
			// Implemented by subclass
		}

		public virtual void OnStateFinish(StateMachine<string> state)
		{
			// Implemented by subclass
		}

        public virtual void OnListen(Event theEvent)
        {
        }

		public abstract void MoveToPosition(Vector3 position, bool force = false);
	}
}
