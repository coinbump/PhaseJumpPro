using System;
using System.Collections.Generic;
using UnityEngine;
using System.Linq;

namespace PJ
{
    public class FocusablePriorityComparer : IComparer<FocusHandler>
    {
        public int Compare(FocusHandler lhs, FocusHandler rhs)
        {
            if (lhs.focusPriority == rhs.focusPriority) { return 0; }
            return lhs.focusPriority < rhs.focusPriority ? 1 : -1;
        }
    }

    public class FocusableMoveFilter
    {
        public Vector3 referencePosition;
        public Angle referenceAngle;

        public FocusableMoveFilter(Vector3 referencePosition, FocusDirection focusDirection)
        {
            this.referencePosition = referencePosition;

            Angle focusAngle = Angle.DegreesAngle(0);
            switch (focusDirection)
            {
                case FocusDirection.Up:
                    break;
                case FocusDirection.Right:
                    focusAngle = Angle.DegreesAngle(90.0f);
                    break;
                case FocusDirection.Down:
                    focusAngle = Angle.DegreesAngle(180.0f);
                    break;
                case FocusDirection.Left:
                    focusAngle = Angle.DegreesAngle(270.0f);
                    break;
            }

            this.referenceAngle = focusAngle;
        }

        public bool Filter(FocusHandler lhs)
        {
            var angleDeltaRange = Angle.DegreesAngle(360.0f / 8.0f);

            var lhsDelta = lhs.transform.position - referencePosition;
            var lhsAngle = new Angle(new Vector2(lhsDelta.x, lhsDelta.y));
            var isLHSInAngleRange = MathF.Abs(lhsAngle.MinAngleTo(referenceAngle).Degrees) <= angleDeltaRange.Degrees;
            if (!isLHSInAngleRange) { return false; }

            return true;
        }
    }

    /// <summary>
    /// Compares angle, distance, and priority of focusables to determine focus movement
    /// </summary>
    public class FocusableMoveComparer : IComparer<FocusHandler>
    {
        public Vector3 referencePosition;
        public FocusDirection focusDirection;

        public FocusableMoveComparer(Vector3 referencePosition, FocusDirection focusDirection)
        {
            this.referencePosition = referencePosition;
            this.focusDirection = focusDirection;
        }

        public int Compare(FocusHandler lhs, FocusHandler rhs)
        {
            var lhsDelta = lhs.transform.position - referencePosition;
            var rhsDelta = rhs.transform.position - referencePosition;

            if (lhsDelta.magnitude == rhsDelta.magnitude)
            {
                return new FocusablePriorityComparer().Compare(lhs, rhs);
            }
            return lhsDelta.magnitude < rhsDelta.magnitude ? -1 : 1;
        }
    }

    /// <summary>
    /// The default focus coordinator implementation
    /// </summary>
    public class FocusCoordinator : SomeFocusCoordinator
    {
        UISystem uiSystem;

        protected override void Awake()
        {
            base.Awake();

            uiSystem = FindObjectOfType<UISystem>();
        }

        public override void MoveFocus(FocusDirection direction)
        {
            if (null == uiSystem) { return; }
            var focusables = new List<FocusHandler>(GameObject.FindObjectsOfType<FocusHandler>());
            if (focusables.Count == 0) { return; }

            var activeFocus = uiSystem.ActiveFocus;
            if (null == activeFocus)
            {
                focusables.Sort(new FocusablePriorityComparer());
                focusables[0].HasFocus = true;
                return;
            }

            // Look for hints on focus
            if (activeFocus.TryGetComponent(out SomeNode node))
            {
                switch (direction)
                {
                    case FocusDirection.Right:
                        if (MoveFocusToTag(node.tags.SafeValue<String>("focus.move.right", () => "")))
                        {
                            return;
                        }
                        break;
                    case FocusDirection.Left:
                        if (MoveFocusToTag(node.tags.SafeValue<String>("focus.move.left", () => "")))
                        {
                            return;
                        }
                        break;
                }
            }

            var filter = new FocusableMoveFilter(activeFocus.transform.position, direction);
            focusables.Remove(activeFocus);
            focusables = focusables.Where(focusable => filter.Filter(focusable)).ToList();
            if (focusables.Count == 0) { return; }

            focusables.Sort(new FocusableMoveComparer(activeFocus.transform.position, direction));
            focusables[0].HasFocus = true;
        }

        public bool MoveFocusToTag(String tag)
        {
            var nodes = FindObjectsOfType<SomeNode>();
            foreach (var node in nodes)
            {
                if (node.tags.SafeValue<String>("focus.tag", () => "") == tag)
                {
                    if (node.TryGetComponent(out FocusHandler focusable))
                    {
                        focusable.HasFocus = true;
                        return true;
                    }
                }
            }

            return true;
        }
    }
}
