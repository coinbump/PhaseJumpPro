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
	public class Node : MonoBehaviour
	{
		public enum CullType
		{
			None, OnInvisible
		}

		[HideInInspector]
		public Tags tags = new Tags();

		public string initialState;

		[Serializable]
		public struct NodeTag
		{
			public string name;
			public string value;    // Type is inferred from value
		}
		public List<NodeTag> nodeTags = new List<NodeTag>();

		public CullType cullType = CullType.None;

		/// <summary>
		/// Handles state machine.
		/// </summary>
		protected class Core : PJ.Core
		{
			public WeakReference<Node> Owner { get; }

			public Core(Node owner)
			{
				Owner = new WeakReference<Node>(owner);
				SetStateMachine(owner.state);
			}

			protected override void EvtStateChanged(SomeStateMachine state)
			{
				base.EvtStateChanged(state);

				if (!Owner.TryGetTarget(out Node owner)) { return; }
				if (null == owner) { return; }

				owner.EvtStateChanged(state);
			}

			protected override void EvtStateFinished(SomeStateMachine state)
			{
				base.EvtStateFinished(state);

				if (!Owner.TryGetTarget(out Node owner)) { return; }
				if (null == owner) { return; }

				owner.EvtStateFinished(state);
			}
		}

		protected Core core;

		// State
		public string NodeState
		{
			get
			{
				return state.State;
			}
			set
			{
				state.State = value;
			}
		}

		protected StateMachine<string> state = new StateMachine<string>();

		protected virtual void Awake()
		{
			core = new Core(this);
			tags = new Tags();
		}

		protected virtual void Start()
        {
			foreach (NodeTag tag in nodeTags)
			{
				tags.Add(tag.name, tag.value);
			}

			state.State = initialState;
		}

		protected virtual void OnValidate()
		{
		}

		protected virtual void Update()
		{
			UpdateNode(UpdateType.Default);
		}

		protected virtual void FixedUpdate()
		{
			UpdateNode(UpdateType.Fixed);
		}

		protected virtual void UpdateNode(UpdateType updateType)
		{
		}

		protected virtual void OnBecameInvisible()
		{
			switch (cullType)
			{
				case CullType.OnInvisible:
					Destroy(this.gameObject);
					break;
			}
		}

		public bool HasTag(string name) => tags.ContainsKey(name) || gameObject.CompareTag(name);

		protected virtual void EvtStateChanged(SomeStateMachine state)
		{
			// Implemented by subclass
		}

		protected virtual void EvtStateFinished(SomeStateMachine state)
		{
			// Implemented by subclass
		}
	}
}