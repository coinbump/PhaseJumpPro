#pragma once
#include "Alignment2D.h"

#if FALSE

// Experimental
namespace PJ {
    struct GridViewItem {
        enum class SizeType {
            /// Fixed size column/row, based on .size
            Fixed = 0,

            // FUTURE: To support flexible with min-max, we need a constraints
            // solver
            /// Fits available width
            Flexible = 1
        };

        SizeType sizeType = SizeType::Fixed;
        float size = 0;
        Alignment2D alignment;
        float spacing = 0;

        // FUTURE: for flexible (requires constraints solver)
        // float minSize;
        // float maxSize;

        GridViewItem(SizeType sizeType, float size, Alignment alignment, float spacing = 0) :
            sizeType(sizeType),
            size(size),
            spacing(spacing),
            alignment(alignment) {}
    };
} // namespace PJ

#endif
