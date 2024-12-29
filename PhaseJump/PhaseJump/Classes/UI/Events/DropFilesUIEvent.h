#pragma once

#include "FilePath.h"
#include "List.h"
#include "SomeUIEvent.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/1/24
 */
namespace PJ {
    /// Sent when a user drags files into the app window
    class DropFilesUIEvent : public SomeSignal {
    public:
        VectorList<FilePath> filePaths;
        ScreenPosition position;

        DropFilesUIEvent(VectorList<FilePath> const& filePaths, ScreenPosition position) :
            filePaths(filePaths),
            position(position) {}
    };
} // namespace PJ
