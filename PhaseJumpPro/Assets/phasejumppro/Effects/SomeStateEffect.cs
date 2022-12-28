using System.Collections.Generic;
using System;
using UnityEngine;

namespace PJ
{
    /// <summary>
    /// Handles state changes
    /// </summary>
    public abstract class SomeStateHandler<StateType> : WorldComponent
    {
        public abstract void OnStateChange(StateType state);
    }

    /// <summary>
    /// Applies an effect in response to a state change
    /// </summary>
    public abstract class SomeStateEffect<StateType, ValueType> : SomeStateHandler<StateType>
    {
        protected MeshMaterialType meshMaterialType = MeshMaterialType.Copy;
        private MultiRenderer multiRenderer;

        protected MultiRenderer MultiRenderer
        {
            get
            {
                if (null == multiRenderer)
                {
                    multiRenderer = new(gameObject);
                    multiRenderer.meshMaterialType = meshMaterialType;
                }
                return multiRenderer;
            }
        }

        /// <summary>
        /// For serialization only
        /// </summary>
        [Serializable]
        public struct Item
        {
            public StateType state;
            public ValueType value;
        }

        public ValueType defaultValue;

        /// <summary>
        /// Items list for serialization (EDITOR ONLY)
        /// </summary>
        public List<Item> _items = new();

        /// <summary>
        /// Items list (PLAY MODE)
        /// </summary>
        protected Dictionary<StateType, ValueType> items = new();

        protected ValueType value;

        public ValueType Value
        {
            get => value;
            set
            {
                if (this.value.Equals(value)) { return; }
                this.value = value;
                OnValueChange();
            }
        }

        protected override void Awake()
        {
            base.Awake();

            foreach (var item in _items)
            {
                items[item.state] = item.value;
            }
            _items.Clear();

            value = defaultValue;
            OnValueChange();
        }

        public override void OnStateChange(StateType state)
        {
            if (items.TryGetValue(state, out ValueType value))
            {
                Value = value;
            }
            else
            {
                Value = defaultValue;
            }
        }

        protected virtual void OnValueChange() { }
    }

    public abstract class SomeSpriteStateEffect<StateType> : SomeStateEffect<StateType, Sprite>
    {
    }

    public abstract class SomeColorStateEffect<StateType> : SomeStateEffect<StateType, Color32>
    {
    }
}