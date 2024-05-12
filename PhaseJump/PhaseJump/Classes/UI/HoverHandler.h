#ifndef PJHOVERHANDLER_H
#define PJHOVERHANDLER_H

#include "SomeHoverGestureHandler.h"
#include "VectorList.h"
#include "SomeEffect.h"

/*
 RATING: 5 stars
 Direct port
 CODE REVIEW: 6/18/23
 */
namespace PJ
{
    /// <summary>
    /// Applies an effect when the pointer is over the object
    /// </summary>
    class HoverHandler : public SomeHoverGestureHandler
    {
    public:
        SP<SomeEffect> hoverEffect;

        VectorList<SP<SomeEffect>> moreHoverEffects;

        VectorList<SP<SomeEffect>> HoverEffects() const
        {
            auto result = moreHoverEffects;
            if (hoverEffect) {
                result.Add(hoverEffect);
            }
            return result;
        }

        void SetIsHovering(bool value) override
        {
            for (auto hoverEffect : HoverEffects())
            {
                hoverEffect->SetIsOn(value);
            }
        }
    };
}

#endif
