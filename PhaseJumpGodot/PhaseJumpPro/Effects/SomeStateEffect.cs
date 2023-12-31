using System.Collections.Generic;
using System;
using Godot;

namespace PJ
{
    /// <summary>
    /// Applies an effect in response to a state change
    /// </summary>
    public abstract partial class SomeStateEffect<StateType, ValueType> : SomeStateHandler<StateType>
    {
        protected MeshMaterialType meshMaterialType = MeshMaterialType.Copy;
        private RendererTool rendererTool;

        public Node Target => GetParent();

        protected RendererTool RendererTool
        {
            get
            {
                if (null == rendererTool)
                {
                    rendererTool = new(Target);
                    rendererTool.meshMaterialType = meshMaterialType;
                }
                return rendererTool;
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

        protected ValueType Value
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

    // public abstract partial class SomeSpriteStateEffect<StateType> : SomeStateEffect<StateType, Sprite>
    // {
    // }

    public abstract partial class SomeColorStateEffect<StateType> : SomeStateEffect<StateType, Color32>
    {
    }

    public abstract partial class SomeTransformStateEffect<StateType> : SomeStateEffect<StateType, GeoTransform>
    {
    }
}
