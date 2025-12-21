#pragma once

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 12/17/24
 */
namespace PJ {
    namespace SignalId {
        /// Pointer device button down. Sends PointerDownUIEvent
        auto constexpr PointerDown = "pointer.down";

        /// Pointer device button up. Sends PointerUpUIEvent
        auto constexpr PointerUp = "pointer.up";

        /// Pointer device entered node's collider. Sends PointerEnterUIEvent
        auto constexpr PointerEnter = "pointer.enter";

        /// Pointer device exited node's collider. Sends PointerExitUIEvent
        auto constexpr PointerExit = "pointer.exit";

        /// Hover is different from pointer enter/exit, because it is managed by the UI system
        /// and only 1 node can be in hover state at a time
        auto constexpr Hover = "hover";

        /// Key was pressed. Sends KeyDownUIEvent
        auto constexpr KeyDown = "key.down";

        /// Drag entered a drop target. Sends DragEnterUIEvent
        auto constexpr DragEnter = "drag.enter";

        /// Drag exited a drop target. Sends DragExitUIEvent
        auto constexpr DragExit = "drag.exit";

        /// Files were dropped in the app window. Sends DropFilesUIEvent
        auto constexpr FilesDrop = "files.drop";

        /// Child node was added. Sends AddChildNodeEvent
        auto constexpr ChildNodeAdd = "childNode.add";

        /// Child node was added. Sends AddChildNodeEvent
        auto constexpr ChildNodeRemove = "childNode.remove";

        /// Called before render. Sends empty Event
        auto constexpr RenderPrepare = "render.prepare";

        /// Called after window is resized
        auto constexpr WindowResize = "window.resize";

        /*
         UNSUPPORTED SIGNALS
         */
        /// FUTURE: support these signals in the future as needed
        /// User requested quit
        auto constexpr Quit = "quit";
        auto constexpr WillEnterBackground = "will.enterBackground";
        auto constexpr DidEnterBackground = "did.enterBackground";
        auto constexpr WillEnterForeground = "will.enterForeground";
        auto constexpr DidEnterForeground = "did.enterForeground";
        auto constexpr LocaleChange = "locale.change";
        auto constexpr SystemThemeChange = "systemTheme.change";
        auto constexpr WindowDisplayChange = "window.display.change";
        auto constexpr WindowEnterFullscreen = "window.enter.fullscreen";
        auto constexpr WindowExitFullscreen = "window.exit.fullscreen";
        auto constexpr KeyUp = "key.up";
        auto constexpr PointerWheel = "pointer.wheel";
        auto constexpr AudioDeviceAdd = "audio.device.add";
        auto constexpr AudioDeviceRemove = "audio.device.remove";

        // FUTURE: support joystick, gamepad events
        // FUTURE: support pen events, camera events
    } // namespace SignalId
} // namespace PJ
