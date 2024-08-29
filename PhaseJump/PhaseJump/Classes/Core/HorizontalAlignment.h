#ifndef PJHORIZONTALALIGNMENT_H
#define PJHORIZONTALALIGNMENT_H

#include "SomeAligner.h"
#include "Utils.h"
#include <memory>

/*
 RATING: 5 stars
 Hase unit tests
 CODE REVIEW: 4/14/24
 */
namespace PJ {
    /// Horizontal alignment in a horizontal flow
    struct HorizontalAlignment {
        AlignFunc aligner;

        HorizontalAlignment(AlignFunc aligner) :
            aligner(aligner) {}

        static const HorizontalAlignment leading;
        static const HorizontalAlignment center;
        static const HorizontalAlignment trailing;
    };
} // namespace PJ

#endif
