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

        SP<SomeAligner> aligner;

        VerticalAlignment(SP<SomeAligner> aligner) :
            aligner(aligner) {}

        static const VerticalAlignment top;
        static const VerticalAlignment center;
        static const VerticalAlignment bottom;
    };

    struct TopAligner : public SomeAligner {
    public:
        virtual ~TopAligner() {}

        float AlignedOrigin(float layoutSize, float frameSize) override {
            return 0;
        }
    };

    struct BottomAligner : public SomeAligner {
    public:
        virtual ~BottomAligner() {}

        float AlignedOrigin(float layoutSize, float frameSize) override {
            return layoutSize - frameSize;
        }
    };
} // namespace PJ

#endif
