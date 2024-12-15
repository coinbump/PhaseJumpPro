#pragma once

// REVIEW: /23
namespace PJ {
    namespace UIContextId {
        /// UI shown in the editor (always visible)
        auto constexpr Editor = "editor";

        /// UI shown in the inspector UI
        auto constexpr Inspector = "inspector";
    }; // namespace UIContextId

    /// Ids of common UI elements
    namespace UIElementId {
        auto constexpr Primary = "primary";
        auto constexpr OnPrimary = "on.primary";
        auto constexpr Secondary = "secondary";
        auto constexpr OnSecondary = "on.secondary";

        auto constexpr PrimaryContainer = "primary.container";
        auto constexpr OnPrimaryContainer = "on.primary.container";
        auto constexpr SecondaryContainer = "secondary.container";
        auto constexpr OnSecondaryContainer = "on.secondary.container";

        auto constexpr Surface = "surface";
        auto constexpr OnSurface = "on.surface";
        auto constexpr SurfaceContainer = "surface.container";
        auto constexpr OnSurfaceContainer = "on.surface.container";

        auto constexpr SurfaceContainerLowSecondary = "surface.container.low.secondary";
        auto constexpr OnSurfaceContainerLowSecondary = "on.surface.container.low.secondary";
        auto constexpr SurfaceContainerLowTertiary = "surface.container.low.tertiary";
        auto constexpr OnSurfaceContainerLowTertiary = "on.surface.container.low.tertiary";

        auto constexpr SurfaceContainerHighSecondary = "surface.container.high.secondary";
        auto constexpr OnSurfaceContainerHighSecondary = "on.surface.container.high.secondary";
        auto constexpr SurfaceContainerHighTertiary = "surface.container.high.tertiary";
        auto constexpr OnSurfaceContainerHighTertiary = "on.surface.container.high.tertiary";

        auto constexpr SurfaceInteractive = "surface.interactive";
        auto constexpr SurfaceInteractiveHover = "surface.interactive.hover";
        auto constexpr SurfaceInteractivePress = "surface.interactive.press";
        auto constexpr OnSurfaceInteractive = "on.surface.interactive";
        auto constexpr OnSurfaceInteractiveHover = "on.surface.interactive.hover";
        auto constexpr OnSurfaceInteractivePress = "on.surface.interactive.press";

        auto constexpr SurfaceToggleOff = "surface.toggle.off";
        auto constexpr SurfaceToggleOffHover = "surface.toggle.off.hover";
        auto constexpr SurfaceToggleOffPress = "surface.toggle.off.press";
        auto constexpr SurfaceToggleOn = "surface.toggle.on";
        auto constexpr SurfaceToggleOnHover = "surface.toggle.on.hover";
        auto constexpr SurfaceToggleOnPress = "surface.toggle.on.press";

        auto constexpr ControlFrame = "control.frame";
        auto constexpr ControlGroup = "control.group";

        auto constexpr SliderTrack = "slider.track";
        auto constexpr SliderThumb = "slider.thumb";
        auto constexpr SliderVerticalTrack = "slider.vertical.track";
        auto constexpr SliderVerticalThumb = "slider.vertical.thumb";

        auto constexpr SurfaceBanner = "surface.banner";
        auto constexpr OnSurfaceBanner = "on.surface.banner";
        auto constexpr SurfaceAlert = "surface.alert";

        // Drag handles (for resizing/altering objects)
        auto constexpr SurfaceHandle = "surface.handle";
        auto constexpr OutlineHandle = "outline.handle";
    } // namespace UIElementId

    /// Ids of common UI components
    namespace UIItemId {
        auto constexpr Surface = "surface";
        auto constexpr Button = "button";
        auto constexpr SegmentToggle = "segmentToggle";
        auto constexpr ImageToggle = "imageToggle";
        auto constexpr SwitchToggle = "switchToggle";
        auto constexpr Label = "label";
        auto constexpr Toast = "toast";
        auto constexpr Dial = "dial";
        auto constexpr ProgressBar = "progressBar";
        auto constexpr ProgressCircle = "progressCircle";
        auto constexpr CheckButton = "checkButton";
        auto constexpr RadioButton = "radioButton";
        auto constexpr SegmentedPicker = "picker.segmented";
    } // namespace UIItemId

    /// Custom property names for common UI elements
    namespace UITag {
        // TODO: get rid of model, use config
        /// Slice points for sliced texture renderer
        auto constexpr Slice9Model = "slice9.model";

        /// Size of end caps for UI objects that have one (slider)
        auto constexpr EndCapSize = "endCap.size";
    } // namespace UITag

} // namespace PJ
