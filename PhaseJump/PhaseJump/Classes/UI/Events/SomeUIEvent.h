#ifndef PJSOMEUIEVENT_H
#define PJSOMEUIEVENT_H

#include "Event.h"
#include "FilePath.h"
#include "SomePosition.h"
#include "List.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 6/17/23
 */
namespace PJ {
    /// A user input event object
    class SomeUIEvent : public Event {
    public:
        virtual ~SomeUIEvent() {}
    };

    class DropFilesUIEvent : public SomeUIEvent {
    public:
        List<FilePath> filePaths;

        DropFilesUIEvent(List<FilePath> const& filePaths) : filePaths(filePaths) {
        }
    };
}

#endif
