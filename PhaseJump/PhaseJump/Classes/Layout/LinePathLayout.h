#pragma once

#include "LinePath.h"
#include "SomePathLayout.h"
#include "Vector3.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 9/14/24
 */
namespace PJ {
    /// Distribute objects along a line path
    class LinePathLayout : public SomePathLayout {
    protected:
        Vector3 start;
        Vector3 end;

    public:
        void SetStart(Vector3 value) {
            GUARD(start != value)
            start = value;
            SetNeedsLayout();
        }

        void SetEnd(Vector3 value) {
            GUARD(end != value)
            end = value;
            SetNeedsLayout();
        }

        UP<SomePath> BuildPath() override {
            return NEW<LinePath>(start, end);
        }

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "LinePathLayout";
        }
    };
} // namespace PJ
