using Godot;

/*
RATING: 5 STARS
Tested and works
GODOT CODE REVIEW: 12/30/23
*/
namespace PJ
{
    public partial class AlphaEffect : SomeAnimatedEffect
    {
        [Export]
        public float offAlpha = 1.0f;
        [Export]
        public float onAlpha = 0.5f;

        public override void UpdateEffectProperties()
        {
            base.UpdateEffectProperties();

            var valveState = valve.ValveState;
            Color color = RendererTool.Color;
            color.A = new FloatValueInterpolator().ValueAt(offAlpha, onAlpha, valveState);
            RendererTool.Color = color;
        }
    }
}
