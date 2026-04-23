#pragma once

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 12/15/24
 */
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
        auto constexpr Tertiary = "tertiary";
        auto constexpr OnTertiary = "on.tertiary";

        auto constexpr PrimaryContainer = "primary.container";
        auto constexpr OnPrimaryContainer = "on.primary.container";
        auto constexpr SecondaryContainer = "secondary.container";
        auto constexpr OnSecondaryContainer = "on.secondary.container";
        auto constexpr TertiaryContainer = "tertiary.container";
        auto constexpr OnTertiaryContainer = "on.tertiary.container";

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
        auto constexpr OutlineControlGroup = "outline.control.group";

        auto constexpr SliderTrack = "slider.track";
        auto constexpr SliderThumb = "slider.thumb";
        auto constexpr SliderVerticalTrack = "slider.vertical.track";
        auto constexpr SliderVerticalThumb = "slider.vertical.thumb";

        auto constexpr SurfaceBanner = "surface.banner";
        auto constexpr OnSurfaceBanner = "on.surface.banner";

        /// Drag handles (for resizing/altering objects)
        auto constexpr DragHandle = "drag.handle";

        // App background (behind all surfaces)
        auto constexpr Background = "background";
        auto constexpr OnBackground = "on.background";

        // Semantic status colors
        auto constexpr Error = "error";
        auto constexpr OnError = "on.error";
        auto constexpr ErrorContainer = "error.container";
        auto constexpr OnErrorContainer = "on.error.container";

        auto constexpr Success = "success";
        auto constexpr OnSuccess = "on.success";
        auto constexpr SuccessContainer = "success.container";
        auto constexpr OnSuccessContainer = "on.success.container";

        auto constexpr Warning = "warning";
        auto constexpr OnWarning = "on.warning";
        auto constexpr WarningContainer = "warning.container";
        auto constexpr OnWarningContainer = "on.warning.container";

        auto constexpr Info = "info";
        auto constexpr OnInfo = "on.info";
        auto constexpr InfoContainer = "info.container";
        auto constexpr OnInfoContainer = "on.info.container";

        // Outlines / separators
        auto constexpr Outline = "outline";
        auto constexpr Separator = "separator";

        // Inverse / high-contrast variants (e.g. dark-on-light inside a light app)
        auto constexpr InverseSurface = "inverse.surface";
        auto constexpr OnInverseSurface = "on.inverse.surface";
        auto constexpr InversePrimary = "inverse.primary";

        // Overlays
        auto constexpr Scrim = "scrim";
        auto constexpr Shadow = "shadow";

        // Focus ring / keyboard focus indicator
        auto constexpr Focus = "focus";
        auto constexpr FocusRing = "focus.ring";

        // Disabled state tinting
        auto constexpr SurfaceDisabled = "surface.disabled";
        auto constexpr OnSurfaceDisabled = "on.surface.disabled";

        // Selected row / item (list, tree, table selection)
        auto constexpr SurfaceSelected = "surface.selected";
        auto constexpr OnSurfaceSelected = "on.surface.selected";

        // Hyperlink text
        auto constexpr Link = "link";
        auto constexpr LinkVisited = "link.visited";
        auto constexpr LinkHover = "link.hover";

        // Text-editing affordances
        auto constexpr TextCaret = "text.caret";
        auto constexpr TextSelection = "text.selection";
        auto constexpr OnTextSelection = "on.text.selection";
        auto constexpr TextPlaceholder = "text.placeholder";

        // Tooltip
        auto constexpr SurfaceToolTip = "surface.tooltip";
        auto constexpr OnSurfaceToolTip = "on.surface.tooltip";

        // Badge
        auto constexpr SurfaceBadge = "surface.badge";
        auto constexpr OnSurfaceBadge = "on.surface.badge";

        // Chip / tag
        auto constexpr SurfaceChip = "surface.chip";
        auto constexpr OnSurfaceChip = "on.surface.chip";

        // Progress indicators
        auto constexpr ProgressTrack = "progress.track";
        auto constexpr ProgressFill = "progress.fill";

        // Scrollbar
        auto constexpr ScrollbarTrack = "scrollbar.track";
        auto constexpr ScrollbarThumb = "scrollbar.thumb";
        auto constexpr ScrollbarThumbHover = "scrollbar.thumb.hover";
        auto constexpr ScrollbarThumbPress = "scrollbar.thumb.press";
    } // namespace UIElementId

    /// Ids of common UI components
    namespace UIItemId {
        /// Main surface behind themed objects
        auto constexpr Surface = "surface";

        /// Pressable button, with touch-track interactivity
        auto constexpr Button = "button";

        /// Slider with thumb control
        auto constexpr Slider = "slider ";

        /// Collapsing header. When expanded it shows child content
        auto constexpr CollapsingHeader = "header.collapsing";

        /// Toggle button inside  a segment picker
        auto constexpr ToggleSegment = "toggle.segment";

        /// Toggle button with image
        auto constexpr ToggleImage = "toggle.image";

        /// Switch style toggle button
        auto constexpr ToggleSwitch = "toggle.switch";

        /// Checkbox style toggle button
        auto constexpr ButtonCheck = "button.check";

        /// Radio style button (usually inside a radio button group)
        auto constexpr ButtonRadio = "button.radio";

        /// Text label
        auto constexpr Label = "label";

        /// Banner toast
        auto constexpr Toast = "toast";

        /// Tooltip that gives contextual popover information for a view
        auto constexpr ToolTip = "toolTip";

        /// Audio mixer style dial
        auto constexpr Dial = "dial";

        /// Shows progress
        auto constexpr ProgressBar = "progress.bar";

        /// Shows progress in a circle, along the frame of the circle
        auto constexpr ProgressCircle = "progress.circle";

        /// HStack of toggle buttons with options to pick from
        auto constexpr PickerSegment = "picker.segment";

        /// Vertical scrolling list of item views
        auto constexpr List = "list";

        /// Vertical scrolling list of selectable item views
        auto constexpr ListSelect = "list.select";

        /// Alert-style dialog with information for user
        auto constexpr Alert = "alert";

        /// Tab-style button
        auto constexpr ButtonTab = "button.tab";

        /// Single-line text input
        auto constexpr TextField = "text.field";

        /// Multi-line text input
        auto constexpr TextArea = "text.area";

        /// Search input (text field with search affordance)
        auto constexpr SearchField = "search.field";

        /// Password input (text field with masked characters)
        auto constexpr PasswordField = "password.field";

        /// Dropdown / combo box selector
        auto constexpr Dropdown = "dropdown";

        /// Numeric stepper with increment/decrement controls
        auto constexpr Stepper = "stepper";

        /// Color picker
        auto constexpr PickerColor = "picker.color";

        /// Date picker
        auto constexpr PickerDate = "picker.date";

        /// File picker / chooser
        auto constexpr PickerFile = "picker.file";

        /// Image view
        auto constexpr Image = "image";

        /// Clickable hyperlink
        auto constexpr Link = "link";

        /// Grouped content container with elevation
        auto constexpr Card = "card";

        /// Panel / group box container
        auto constexpr Panel = "panel";

        /// Scrolling content container
        auto constexpr ScrollView = "scroll.view";

        /// Indeterminate loading indicator
        auto constexpr Spinner = "spinner";

        /// Divider / separator line
        auto constexpr Separator = "separator";

        /// Tab strip container (holds ButtonTab items)
        auto constexpr TabBar = "tab.bar";

        /// Toolbar container for actions
        auto constexpr Toolbar = "toolbar";

        /// Application / window menu bar
        auto constexpr MenuBar = "menu.bar";

        /// Menu container (for menu bar, context menu, dropdown)
        auto constexpr Menu = "menu";

        /// Selectable item inside a menu
        auto constexpr MenuItem = "menu.item";

        /// Context / right-click menu
        auto constexpr ContextMenu = "menu.context";

        /// Modal dialog window
        auto constexpr Dialog = "dialog";

        /// Popover / anchored floating panel
        auto constexpr Popover = "popover";

        /// Hierarchical tree view
        auto constexpr Tree = "tree";

        /// Tabular grid of rows and columns
        auto constexpr Table = "table";

        /// Grid layout of items
        auto constexpr Grid = "grid";

        /// Drag handle between split views
        auto constexpr Splitter = "splitter";
    } // namespace UIItemId

    /// Custom property names for common UI elements
    namespace UITag {
        /// Slice points for sliced texture renderer
        auto constexpr Slice9Model = "slice9.model";

        /// Size of end caps for UI objects that have one (Example: slider)
        auto constexpr EndCapSize = "endCap.size";
    } // namespace UITag

} // namespace PJ
