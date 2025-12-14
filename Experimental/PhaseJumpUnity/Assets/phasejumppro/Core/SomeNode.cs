using System;
using System.Collections.Generic;
using UnityEngine;

/*
RATING: 5 stars
Solid foundation
CODE REVIEW: 12/20/22
*/
namespace PJ
{
    public enum UpdateType
    {
        Default, Fixed
    }

    public enum NodeCullType
    {
        Invisible, ZeroAlpha
    }

    /// <summary>
    /// Provides utility methods for simplifying common game scenarios
    /// </summary>
    public abstract class SomeNode : WorldComponent, SomeListener, StringIdentifiable
    {
        public string id;

        /// <summary>
        /// Optionally used by subclass to determine initial state
        /// </summary>
        public string initialState;

        public string Id { get => id; set => id = value; }
        public abstract bool IsKinematic { get; }

        /// <summary>
        /// Defines key-value pairs for custom properties ("health: 10", "energy: 20", etc.)
        /// (EDITOR ONLY: values are moved on Awake)
        /// </summary>
        public List<TagValue> _tags = new();
        [NonSerialized]
        public Tags tags = new();

        /// <summary>
        /// Defines type values for object ("enemy", "hero", "bullet", etc.)
        /// (EDITOR ONLY: values are moved on Awake)
        /// </summary>
        public List<string> _typeTags = new();
        [NonSerialized]
        public HashSet<string> typeTags = new();

        /// <summary>
        /// Defines state values for object ("focused", "selected", "dragged", etc.)
        /// (EDITOR ONLY: values are moved on Awake)
        /// </summary>
        public List<string> _stateTags = new();
        [NonSerialized]
        public HashSet<string> stateTags = new();

        /// <summary>
        /// Tags for ways this object can be culled: "invisible", "zeroAlpha", etc.
        /// (EDITOR ONLY: values are moved on Awake)
        /// </summary>
        public List<NodeCullType> _cullTypes = new();
        [NonSerialized]
        public HashSet<NodeCullType> cullTypes = new();

        public RendererTool rendererTool;

        protected override void Awake()
        {
            base.Awake();

            UpdateFromSerializedProperties(false);

            // EDITOR ONLY:
            _tags.Clear();
            _typeTags.Clear();
            _stateTags.Clear();
            _cullTypes.Clear();

            rendererTool = new RendererTool(gameObject);
        }

        public virtual void UpdateFromSerializedProperties(bool forceUpdate)
        {
            tags = new(_tags);
            stateTags = new(_stateTags);
            typeTags = new(_typeTags);
            cullTypes = new(_cullTypes);
        }

        protected virtual void OnValidate()
        {
            tags.Clear();

            UpdateFromSerializedProperties(true);
        }

        protected override void Update()
        {
            base.Update();

            if (null != rendererTool && rendererTool.Color.a == 0 && cullTypes.Contains(NodeCullType.ZeroAlpha))
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
            if (cullTypes.Contains(NodeCullType.Invisible))
            {
                Destroy(gameObject);
            }
        }

        public bool HasTypeTag(string name) => typeTags.Contains(name);
        public bool HasStateTag(string name) => stateTags.Contains(name);

        public virtual void OnEvent(Event theEvent) { }

        public abstract void MoveToPosition(Vector3 position, bool force = false);
    }
}
