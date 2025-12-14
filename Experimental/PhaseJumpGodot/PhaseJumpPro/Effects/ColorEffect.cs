using System;
using Godot;

/*
RATING: 5 stars
Tested and works
GODOT CODE REVIEW: 12/30/23
*/
namespace PJ
{
    /// <summary>
    /// Modifies the object's main render color
    /// </summary>
    public partial class ColorEffect : SomeAnimatedEffect
    {
        [Export]
        public Color offColor = Colors.White;
        [Export]
        public Color onColor = Colors.White;

        public override void UpdateEffectProperties()
        {
            base.UpdateEffectProperties();

            var valveState = valve.ValveState;
            var lerp = new ColorInterpolator(offColor, onColor);
            RendererTool.Color = lerp.ValueAt(valveState);
        }
    }
}
