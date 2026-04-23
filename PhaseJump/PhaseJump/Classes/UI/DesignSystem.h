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
#include <typeindex>

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/8/24
 */
namespace PJ {
    class Texture;
    class ViewBuilder;

    /// Defines properties and methods for building UI
    class DesignSystem {
    public:
        using This = DesignSystem;
        using BuildViewFunc = std::function<void(ViewBuilder&)>;
        using ModifyViewFunc = std::function<void(View2D&)>;
        using BuildConfigViewFunc = std::function<void(void const* config, ViewBuilder&)>;

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

            // FUTURE: isElastic flag
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
            GetBindingFunc<float> valueFunc;

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

        /// Splitter config — the draggable handle that sits between two SplitView panes.
        struct SplitterConfig {
            /// Axis the parent split view splits along.
            Axis2D axis = Axis2D::X;

            /// Starting first-pane ratio for the SplitterControl created by the design system.
            float initialRatio = 0.5f;
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
        /// Type-erased builder paired with the config type it expects.
        struct BuildEntry {
            std::type_index configType{ typeid(void) };
            BuildConfigViewFunc func;
        };

        /// Maps UI element ids to their typed build-view dispatchers.
        UnorderedMap<String, BuildEntry> buildViewFuncs;

        /// Maps UI element ids to specific textures
        UnorderedMap<String, SP<Texture>> elementTextures;

    public:
        struct Config {
            SP<Theme> theme = MAKE<Theme>(Theme::Config{});
        };

        SP<Theme> theme;

        DesignSystem(Config const& config);

        virtual ~DesignSystem() {}

        /// Adds a texture for a UI element
        void AddElementTexture(String elementId, SP<Texture> texture) {
            elementTextures.insert_or_assign(elementId, texture);
        }

        /// @return Returns a texture for a UI element
        SP<Texture> Texture(String elementId) const {
            try {
                return elementTextures.at(elementId);
            } catch (...) {}

            return nullptr;
        }

        template <class Config>
        This& BuildView(String elementId, Config const& config, ViewBuilder& vb) {
            GUARDR_LOG(theme, *this, "ERROR: Missing theme")

            auto i = buildViewFuncs.find(elementId);
            if (i == buildViewFuncs.end()) {
                PJ::Log("ERROR. Design not supported:", elementId);
                return *this;
            }

            auto& entry = i->second;
            if (entry.configType != std::type_index(typeid(Config))) {
                PJ::Log("ERROR. Config type mismatch for element:", elementId);
                return *this;
            }

            entry.func(&config, vb);
            return *this;
        }

        /// Registers a typed builder for a UI element.
        template <class ConfigT>
        This& Register(String elementId, std::function<void(ConfigT const&, ViewBuilder&)> func) {
            buildViewFuncs.insert_or_assign(
                elementId,
                BuildEntry{ .configType = std::type_index(typeid(ConfigT)),
                            .func = [inner = std::move(func)](
                                        void const* configP, ViewBuilder& vb
                                    ) { inner(*static_cast<ConfigT const*>(configP), vb); } }
            );
            return *this;
        }
    };
} // namespace PJ
