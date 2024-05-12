#ifndef PJSOMEWINDOW_H
#define PJSOMEWINDOW_H

#include "Base.h"
#include "Class.h"
#include "Vector2.h"

/*
 RATING: 4 stars
 Simple interface, no functionality (yet)
 CODE REVIEW: 5/8/23
 */
namespace PJ
{
    class SomeWindow : public Base {
    public:
        Vector2Int size;

        SomeWindow(Vector2Int size) : size(size)
        {
        }
    };
}

#endif
