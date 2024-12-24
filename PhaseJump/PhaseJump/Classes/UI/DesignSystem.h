#pragma once

#include "ButtonControl.h"
#include "Color.h"
#include "Font.h"
#include "LayoutTypes.h"
#include "StringUtils.h"
#include "Tags.h"
#include "Theme.h"
#include "UITypes.h"
#include "UnorderedMap.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/8/24
 */
namespace PJ {
    class SomeTexture;
    class ViewBuilder;

    /// Defines properties and methods for building UI
    class DesignSystem {
    public:
        using This = DesignSystem;
        using BuildViewFunc = std::function<void(ViewBuilder&)>;
        using ModifyViewFunc = std::function<void(View2D&)>;
        using BuildConfigViewFunc = std::function<void(void* config, ViewBuilder&)>;

        /// Surface config
        struct SurfaceConfig {
            String colorId = UIElementId::Surface;

            /// Surface color (takes priority over colorId)
            std::optional<Color> color;
        };

        /// Button config
        struct ButtonConfig {
            String label = "##Button##";
            ButtonControl::OnPressFunc onPressFunc;
            ModifyViewFunc modifyViewFunc;
        };

        /// Collapsing header config
        struct CollapsingHeaderConfig {
            String label = "##Collapsing Header##";
            Binding<bool> isCollapsedBinding;
        };

        struct SliderConfig {
            Axis2D axis = Axis2D::X;

            float minValue{};
            float maxValue = 1.0f;
            Binding<float> valueBinding;
        };

        /// Toggle button config
        struct ToggleButtonConfig {
            String label = "##Toggle##";
            Binding<bool> isOnBinding;

            String imageId;
            std::optional<Color> imageColor;
            std::optional<Vector2> size;
            ModifyViewFunc modifyViewFunc;
        };

        /// Progress bar config
        struct ProgressBarConfig {
            /// Returns normalized progress value (0-1)
            GetFunc<float> valueFunc;

            std::optional<Color> progressColor;
        };

        /// Dial control config
        struct DialConfig {
            Binding<float> valueBinding;

            std::optional<Color> surfaceColor;
            std::optional<Color> onSurfaceColor;
        };

        /// Label config
        struct LabelConfig {
            String text = "##Text##";

            String colorId = UIElementId::OnSurface;

            /// Surface color (takes priority over colorId)
            std::optional<Color> color;
        };

        /// ToolTip config
        struct ToolTipConfig {
            String text = "##Tip##";
            float width = 400;

            /// Anchor of the presenting view for presentation
            LayoutAnchor2D presentingAnchor = LayoutAnchor2D::bottom;

            /// Anchor of the tip view for presentation
            LayoutAnchor2D anchor = LayoutAnchor2D::top;

            Vector2 offset;

            View2D** result{};
        };

    protected:
        /// Maps UI element ids to specific textures
        UnorderedMap<String, BuildConfigViewFunc> buildViewFuncs;

        /// Maps UI element ids to specific textures
        UnorderedMap<String, SP<SomeTexture>> elementTextures;

    public:
        struct Config {
            SP<Theme> theme = MAKE<Theme>(Theme::Config{});
        };

        SP<Theme> theme;

        DesignSystem(Config config);

        virtual ~DesignSystem() {}

        /// Adds a texture for a UI element
        void AddElementTexture(String elementId, SP<SomeTexture> texture) {
            elementTextures.insert_or_assign(elementId, texture);
        }

        /// @return Returns a texture for a UI element
        SP<SomeTexture> Texture(String elementId) const {
            try {
                return elementTextures.at(elementId);
            } catch (...) {}

            return nullptr;
        }

        template <class Config>
        This& BuildView(String elementId, Config config, ViewBuilder& vb) {
            GUARDR_LOG(theme, *this, "ERROR: Missing theme")

            try {
                auto& buildViewFunc = buildViewFuncs.at(elementId);
                buildViewFunc(&config, vb);
            } catch (...) {
                PJ::Log("ERROR. Design not supported:", elementId);
            }

            return *this;
        }
    };
} // namespace PJ
