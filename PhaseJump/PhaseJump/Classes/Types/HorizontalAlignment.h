#ifndef PJHORIZONTALALIGNMENT_H
#define PJHORIZONTALALIGNMENT_H

#include "SomeAligner.h"
#include <memory>

/*
 RATING: 5 stars
 Utility
 CODE REVIEW: 12/7/22
 */
namespace PJ
{
    /// <summary>
    /// Horizontal alignment in a horizontal flow
    /// </summary>
    struct HorizontalAlignment
    {
        std::shared_ptr<SomeAligner> aligner;

        HorizontalAlignment(std::shared_ptr<SomeAligner> aligner) : aligner(aligner)
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
