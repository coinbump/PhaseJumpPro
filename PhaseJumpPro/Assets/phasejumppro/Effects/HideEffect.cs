using UnityEngine;

/*
RATING: 5 STARS
Tested and works
CODE REVIEW: 12/26/22
*/
namespace PJ
{
    public class HideEffect : SomeEffect
    {
        public bool isOffVisible = true;

        public override void UpdateEffectProperties()
        {
            base.UpdateEffectProperties();

            MultiRenderer.Enabled = isOffVisible ? !IsOn : IsOn;
        }
    }
}
