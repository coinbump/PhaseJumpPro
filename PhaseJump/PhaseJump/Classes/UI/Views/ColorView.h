#pragma once

#include "Color.h"
#include "View2D.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 11/25/24
 */
namespace PJ {
    class ColorRenderer;

    /// Color view that fills available space
    class ColorView : public View2D {
    protected:
        Color color;

        ColorRenderer* Renderer();

    public:
        using Base = View2D;
        using This = ColorView;

        ColorView(Color color = {});

        void SetColor(Color value);

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "ColorView";
        }

        void Awake() override;
    };
} // namespace PJ
