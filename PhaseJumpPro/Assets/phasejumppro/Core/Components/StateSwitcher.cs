using System;
using System.Collections.Generic;
using UnityEngine;

namespace PJ
{
    /// <summary>
    /// Turns switches on and off based on states
    /// This is useful when we have multiple separate states that trigger the same effect
    /// Example: dragged and selected
    /// </summary>
    public class StateSwitcher : WorldComponent
    {
        [Serializable]
        public class Item
        {
            public List<string> states = new();
            public List<SwitchHandler> switchables = new();
        }

        public List<Item> items = new();

        protected HashSet<string> states = new();

        public override void OnUpdate(TimeSlice time)
        {
            base.OnUpdate(time);

            UpdateSwitches();
        }

        public void UpdateSwitches()
        {
            if (!TryGetComponent(out SomeNode node)) { return; }

            this.states = node.stateTags;

            foreach (var item in items)
            {
                foreach (var switchable in item.switchables)
                {
                    bool didSet = false;

                    foreach (var itemState in item.states)
                    {
                        var isOn = states.Contains(itemState);
                        if (isOn)
                        {
                            didSet = true;
                            switchable.IsOn = true;
                            break;
                        }
                    }

                    if (!didSet)
                    {
                        switchable.IsOn = false;
                    }
                }
            }
        }

        public void OnValidate()
        {
            UpdateSwitches();
        }
    }
}
