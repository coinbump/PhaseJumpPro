#pragma once

#include "DuckDesignSystem.h"
#include "ViewBuilder.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/8/24
 */
namespace PJ {
    /// Simplifies building them UIs using a design system and view builder
    class ThemeBuilder {
    protected:
        DesignSystem& designSystem;
        ViewBuilder& vb;

        DuckDesignSystem fallbackDesignSystem;

    public:
        using This = ThemeBuilder;
        using SurfaceConfig = DesignSystem::SurfaceConfig;
        using ButtonConfig = DesignSystem::ButtonConfig;
        using ToggleButtonConfig = DesignSystem::ToggleButtonConfig;
        using ProgressBarConfig = DesignSystem::ProgressBarConfig;
        using LabelConfig = DesignSystem::LabelConfig;

        ThemeBuilder(DesignSystem& designSystem, ViewBuilder& vb);
        ThemeBuilder(ViewBuilder& vb);

        This& Surface(SurfaceConfig config = {}) {
            designSystem.BuildView(UIItemId::Surface, config, vb);
            return *this;
        }

        This& Button(ButtonConfig config) {
            designSystem.BuildView(UIItemId::Button, config, vb);
            return *this;
        }

        This& SegmentButton(ToggleButtonConfig config) {
            designSystem.BuildView(UIItemId::SegmentButton, config, vb);
            return *this;
        }

        This& ProgressBar(ProgressBarConfig config) {
            designSystem.BuildView(UIItemId::ProgressBar, config, vb);
            return *this;
        }

        This& Label(LabelConfig config) {
            designSystem.BuildView(UIItemId::Label, config, vb);
            return *this;
        }

        This& CheckButton(ToggleButtonConfig config) {
            designSystem.BuildView(UIItemId::CheckButton, config, vb);
            return *this;
        }

        This& Toast(LabelConfig config) {
            designSystem.BuildView(UIItemId::Toast, config, vb);
            return *this;
        }
    };
} // namespace PJ
