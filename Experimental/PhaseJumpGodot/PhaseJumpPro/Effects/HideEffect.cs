using Godot;

/*
RATING: 5 STARS
Tested and works
GODOT CODE REVIEW: 12/30/23
*/
namespace PJ
{
    public partial class HideEffect : SomeEffect
    {
        [Export]
        public bool isOffVisible = true;

        public override void UpdateEffectProperties()
        {
            GD.Print("UpdateEffectProperties");
            base.UpdateEffectProperties();

            var value = isOffVisible ? !IsOn : IsOn;
            var target = Target;
            if (null == target) { return; }

            if (target is CanvasItem canvasItem)
            {
                canvasItem.Visible = value;
            }
        }
    }
}
