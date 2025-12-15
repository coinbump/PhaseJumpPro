#pragma once

#include "NodeHandler.h"
#include "RenderProcessor.h"
#include "View2D.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/14/25
 */
namespace PJ {
    /// Paints imGui window where the view is
    class ImGuiView : public View2D {
    protected:
    public:
        using Base = View2D;
        using This = ImGuiView;
        using PaintFunc = std::function<void()>;

        struct Config {
            String title;
            PaintFunc paintFunc;
        };

        /// All imGui windows must have a unique title (use ## to hide title)
        String title;

        /// Called to render imGUI
        PaintFunc paintFunc;

        ImGuiView(Config const& config) :
            title(config.title),
            paintFunc(config.paintFunc) {}

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "ImGuiView";
        }

        void Awake() override;
    };
} // namespace PJ
