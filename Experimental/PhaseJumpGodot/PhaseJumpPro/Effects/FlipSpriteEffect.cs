using Godot;

/*
RATING: 5 STARS
Tested and works
GODOT CODE REVIEW: 12/30/23
*/
namespace PJ
{
    /// <summary>
    /// Flip the sprite
    /// </summary>
    public partial class FlipSpriteEffect : SomeEffect
    {
        // FUTURE: Convert tooltips if Godot adds C# support
        [Tooltip("If true, effect flips sprite horizontally")]
        [Export]
        public bool modifyFlipX = true;

        [Tooltip("If true, effect flips sprite vertically")]
        [Export]
        public bool modifyFlipY = false;

        [Export]
        public bool offFlipX = false;
        [Export]
        public bool offFlipY = false;

        public override void UpdateEffectProperties()
        {
            base.UpdateEffectProperties();

            if (Target is Sprite2D sprite2D)
            {
                if (modifyFlipX)
                {
                    sprite2D.FlipH = offFlipX ? !IsOn : IsOn;
                }
                if (modifyFlipY)
                {
                    sprite2D.FlipV = offFlipY ? !IsOn : IsOn;
                }
            }
        }
    }
}
