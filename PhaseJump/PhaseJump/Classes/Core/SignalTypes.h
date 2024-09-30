#pragma once

namespace PJ {
    namespace SignalId {
        auto constexpr PointerDown = "pointer.down";
        auto constexpr PointerUp = "pointer.up";
        auto constexpr PointerEnter = "pointer.enter";
        auto constexpr PointerExit = "pointer.exit";

        // Hover is different from pointer enter/exit, because it is managed by the UI system
        // and only 1 node can be in hover state at a time
        auto constexpr Hover = "hover";

        auto constexpr KeyDown = "key.down";
        auto constexpr InputAction = "input.action";

        auto constexpr DragEnter = "drag.enter";
        auto constexpr DragExit = "drag.exit";
        auto constexpr DropFiles = "drop.files";

        /// A child node was added to the relevant node
        auto constexpr AddChildNode = "add.childNode";
    } // namespace SignalId
} // namespace PJ
