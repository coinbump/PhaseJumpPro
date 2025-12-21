#pragma once

#include "ImGuiPainter.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 10/20/24
 */
namespace PJ {
    class CommandHistory;

    /// Draws command history
    class EditorImGuiCommandHistoryPainter : public ImGuiPainter {
    public:
        CommandHistory& commandHistory;

        EditorImGuiCommandHistoryPainter(CommandHistory& commandHistory);
    };
} // namespace PJ
