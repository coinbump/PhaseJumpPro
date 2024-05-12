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
    enum class VerticalAlignmentType
    {
        Center = 0, Top = 1, Bottom = 2
    };

    /// <summary>
    /// Vertical alignment in a horizontal flow
    /// </summary>
    struct VerticalAlignment
    {
        struct Builder
        {
            VerticalAlignment VerticalAlignmentFrom(VerticalAlignmentType type);
        };

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
    public:
        virtual ~TopAligner() {}

        float AlignedOrigin(float layoutSize, float frameSize) override
        {
            return 0;
        }
    };

    struct BottomAligner : public SomeAligner
    {
    public:
        virtual ~BottomAligner() {}

        float AlignedOrigin(float layoutSize, float frameSize) override
        {
            return layoutSize - frameSize;
        }
    };
}

#endif
