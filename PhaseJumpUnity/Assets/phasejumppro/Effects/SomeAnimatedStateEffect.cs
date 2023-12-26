using System.Collections.Generic;
using System;
using UnityEngine;

namespace PJ
{
    /// <summary>
    /// Applies an effect in response to a state change
    /// </summary>
    public abstract class SomeAnimatedStateEffect<StateType, ValueType> : SomeStateHandler<StateType>
    {
        /// <summary>
        /// Transition duration from one state to another
        /// </summary>
        public float duration = 0.3f;

        protected MeshMaterialType meshMaterialType = MeshMaterialType.Copy;
        private RendererTool rendererTool;
        protected Timer transitionTimer;

        protected RendererTool MultiRenderer
        {
            get
            {
                if (null == rendererTool)
                {
                    rendererTool = new(gameObject);
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

        protected StateMachine<StateType> stateMachine = new();
        protected ValueType value;
        protected ValueType animatedValue;
        protected Interpolator<ValueType> interpolator;

        protected abstract Interpolator<ValueType> NewInterpolator(ValueType start, ValueType end, StateType startState, StateType endState);

        protected ValueType AnimatedValue
        {
            get => animatedValue;
            set
            {
                if (this.value.Equals(animatedValue)) { return; }
                this.animatedValue = value;
                OnAnimatedValueChange();
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
            animatedValue = defaultValue;
            OnAnimatedValueChange();
        }

        public override void OnUpdate(TimeSlice time)
        {
            base.OnUpdate(time);

            if (null != transitionTimer)
            {
                transitionTimer.OnUpdate(time);

                if (transitionTimer.IsFinished)
                {
                    AnimatedValue = value;
                }
                else if (null != interpolator)
                {
                    var progress = transitionTimer.Progress;
                    AnimatedValue = interpolator.ValueAt(progress);
                }
            }
        }

        public override void OnStateChange(StateType state)
        {
            stateMachine.State = state;

            ValueType newValue = defaultValue;

            if (items.TryGetValue(state, out ValueType value))
            {
                newValue = value;
            }

            if (newValue.Equals(this.value)) { return; }
            this.value = newValue;

            var duration = StateChangeDuration(stateMachine.PrevState, stateMachine.State);
            transitionTimer = new Timer(duration, SomeRunner.RunType.RunOnce);
            interpolator = NewInterpolator(animatedValue, newValue, stateMachine.PrevState, stateMachine.State);
        }

        /// <summary>
        /// Return the animation duration for a specific state change
        /// </summary>
        protected virtual float StateChangeDuration(StateType start, StateType end)
        {
            return duration;
        }

        protected virtual void OnAnimatedValueChange()
        {
        }
    }

    public abstract class SomeTransformAnimatedStateEffect<StateType> : SomeAnimatedStateEffect<StateType, GeoTransform>
    {
        protected override Interpolator<GeoTransform> NewInterpolator(GeoTransform start, GeoTransform end, StateType startState, StateType endState)
        {
            return new GeoTransformInterpolator(start, end, new EaseOutSquared());
        }
    }
}
