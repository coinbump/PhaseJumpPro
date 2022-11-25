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
	public abstract class SomeNode : MonoBehaviour, SomeStateListener<string>, SomeListener, StringIdentifiable
	{
		public enum CullType
		{
			Invisible, ZeroAlpha
		}

		public string id;

		[NonSerialized]
		public Tags tags = new();

		public string initialState;

		public string Id { get => id; set => id = value; }
		public abstract bool IsKinematic { get; }

		/// <summary>
		/// Defines key-value pairs for custom properties ("health: 10", "energy: 20", etc.)
        /// (EDITOR ONLY: values are moved into `tags` on Awake)
		/// </summary>
		public List<TagValue> valueTags = new List<TagValue>();

		/// <summary>
        /// Defines type values for object ("enemy", "hero", "bullet", etc.)
        /// </summary>
		public List<string> typeTags = new List<string>();

		/// <summary>
		/// Tags for ways this object can be culled: "invisible", "zeroAlpha", etc.
		/// </summary>
		public List<CullType> cullTypes = new List<CullType>();

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

		protected override void Awake()
		{
            CopyInValueTags();

            valueTags.Clear();
            valueTags.Add(new("TEMP", "EDITOR ONLY"));

            core = new Core<string>(this);
			multiRenderer = new MultiRenderer(gameObject);
		}

		public virtual void CopyInValueTags()
		{
			foreach (TagValue tag in valueTags)
			{
				try
				{
                    var floatValue = float.Parse(tag.value);
					tags.Add(tag.name, floatValue);
                    continue;
                }
				catch
				{
				}

				tags.Add(tag.name, tag.value);
			}
		}

		protected override void Start()
        {
			var stringCore = core as Core<string>;
			if (null != stringCore)
			{
				stringCore.State = initialState;
			}
		}

		protected virtual void OnValidate()
		{
            tags.Clear();
            CopyInValueTags();
        }

		protected override void Update()
		{
			base.Update();

			if (cullTypes.Contains(CullType.ZeroAlpha) && multiRenderer.Color.a == 0)
			{
				Destroy(gameObject);
				return;
			}

			UpdateNode(UpdateType.Default);
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
