using PJ;
using System;
using System.Collections.Generic;
using Godot;
using System.Numerics;

namespace PJ
{
    /// <summary>
    /// Distribute children along a path
    /// </summary>
    public abstract partial class SomePathLayout : SomeLayout
    {
        [Export]
        public Godot.Vector3 offset = Godot.Vector3.Zero;

        /// <summary>
        /// LIMITATION: This only supports 2D path orientation for now
        /// </summary>
        [Export]
        public bool orientToPath = true;

        [Export]
        public float orientDegreeAngle = -90.0f;

        // Optional normalized positions (0-1.0), otherwise default will be used."
        public List<float> positions = new();

        public override void ApplyLayout()
        {
            var path = BuildPath();

            int childCount = GetChildCount();
            float normalOffset = childCount > 1 ? 1.0f / (float)(childCount - 1) : 0;

            var index = 0;
            foreach (var child in GetChildren())
            {
                var childObject = child;
                var normalPosition = index * normalOffset;

                if (index < positions.Count)
                {
                    normalPosition = positions[index];
                }

                var position = path.PositionAt(normalPosition);
                position += offset;

                //GD.Print("Apply layout Position: " + position.ToString() + " Normal: " + normalPosition.ToString());
                var transformTool = new TransformTool(childObject);
                transformTool.SetLocalPosition(position);

                // TODO: add support for orientToPath
                if (orientToPath)
                {
                    var prevPosition = normalPosition;
                    var nextPosition = normalPosition;
                    var orientDelta = .001f;
                    if (normalPosition == 0)
                    {
                        nextPosition += orientDelta;
                    }
                    else if (normalPosition == 1.0f)
                    {
                        prevPosition -= orientDelta;
                    }
                    else
                    {
                        nextPosition = Mathf.Min(1.0f, nextPosition + orientDelta);
                    }

                    var positionDelta = path.PositionAt(nextPosition) - path.PositionAt(prevPosition);

                    // var rotationDegreeAngle = new Angle(new Godot.Vector2(positionDelta.X, positionDelta.Y)).Degrees;
                    // rotationDegreeAngle += orientDegreeAngle;
                    // childObject.transform.localEulerAngles = new Godot.Vector3(0, 0, -rotationDegreeAngle);
                }
                else
                {
                    // childObject.transform.localEulerAngles = new Vector3(0, 0, 0);
                }

                index++;
            }
        }

        protected abstract SomePath BuildPath();
    }
}
