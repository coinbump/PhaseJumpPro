#pragma once

#include "FilePath.h"
#include "List.h"
#include "SomeUIEvent.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 7/21/24
 */
namespace PJ {
    class DropFilesUIEvent : public SomeUIEvent {
    public:
        List<FilePath> filePaths;
        ScreenPosition position;

        DropFilesUIEvent(List<FilePath> const& filePaths, ScreenPosition position) :
            filePaths(filePaths),
            position(position) {}
    };
} // namespace PJ
