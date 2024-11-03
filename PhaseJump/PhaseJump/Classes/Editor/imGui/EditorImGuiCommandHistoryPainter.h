#pragma once

#include "SomeImGuiPainter.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 10/20/24
 */
namespace PJ {
    class CommandHistory;

    /// Draws command history
    class EditorImGuiCommandHistoryPainter : public SomeImGuiPainter {
    public:
        CommandHistory& commandHistory;

        EditorImGuiCommandHistoryPainter(CommandHistory& commandHistory);
    };
} // namespace PJ
