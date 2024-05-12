#ifndef PJHORIZONTALALIGNMENT_H
#define PJHORIZONTALALIGNMENT_H

#include "SomeAligner.h"
#include "Macros.h"
#include <memory>

/*
 RATING: 5 stars
 Hase unit tests
 CODE REVIEW: 4/14/24
 */
namespace PJ
{
    /// <summary>
    /// Horizontal alignment in a horizontal flow
    /// </summary>
    struct HorizontalAlignment
    {
        SP<SomeAligner> aligner;

        HorizontalAlignment(SP<SomeAligner> aligner) : aligner(aligner)
        {
        }

        static const HorizontalAlignment leading;
        static const HorizontalAlignment center;
        static const HorizontalAlignment trailing;
    };

    struct LeadingAligner : public SomeAligner
    {
        float AlignedOrigin(float layoutSize, float frameSize) override
        {
            return 0;
        }
    };

    struct TrailingAligner : public SomeAligner
    {
        float AlignedOrigin(float layoutSize, float frameSize) override
        {
            return layoutSize - frameSize;
        }
    };
}

#endif
