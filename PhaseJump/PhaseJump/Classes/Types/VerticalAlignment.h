#ifndef PJVERTICALALIGNMENT_H
#define PJVERTICALALIGNMENT_H

#include "SomeAligner.h"
#include "Macros.h"
#include <memory>

/*
 RATING: 5 stars
 Utility
 CODE REVIEW: 12/7/22
 */
namespace PJ
{
    /// <summary>
    /// Vertical alignment in a horizontal flow
    /// </summary>
    struct VerticalAlignment
    {
        SP<SomeAligner> aligner;

        VerticalAlignment(SP<SomeAligner> aligner) : aligner(aligner)
        {
        }

        static const VerticalAlignment top;
        static const VerticalAlignment center;
        static const VerticalAlignment bottom;
    };

    struct TopAligner : public SomeAligner
    {
        float AlignedOrigin(float layoutSize, float frameSize) override
        {
            return 0;
        }
    };

    struct BottomAligner : public SomeAligner
    {
        float AlignedOrigin(float layoutSize, float frameSize) override
        {
            return layoutSize - frameSize;
        }
    };
}

#endif
