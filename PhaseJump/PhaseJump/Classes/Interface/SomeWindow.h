#ifndef PJSOMEWINDOW_H
#define PJSOMEWINDOW_H

#include "Base.h"
#include "Class.h"
#include "Vector2.h"

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
