#ifndef PJVERTICALALIGNMENT_H
#define PJVERTICALALIGNMENT_H

#include "SomeAligner.h"
#include "Utils.h"
#include <memory>

/*
 RATING: 5 stars
 Utility
 CODE REVIEW: 7/5/24
 */
namespace PJ {
    enum class VerticalAlignmentType { Center = 0, Top = 1, Bottom = 2 };

    /// Vertical alignment in a horizontal flow
    struct VerticalAlignment {
        struct Builder {
            VerticalAlignment VerticalAlignmentFrom(VerticalAlignmentType type);
        };

        AlignFunc aligner;

        VerticalAlignment(AlignFunc aligner) :
            aligner(aligner) {}

        static const VerticalAlignment top;
        static const VerticalAlignment center;
        static const VerticalAlignment bottom;
    };
} // namespace PJ

#endif
