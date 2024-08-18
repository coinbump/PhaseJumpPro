#ifndef PJLINEPATH_H
#define PJLINEPATH_H

#include "SomePath.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 6/8/24
 */
namespace PJ {
    /// Model that defines a line path
    class LinePath : public SomePath {
    public:
        Vector3 start;
        Vector3 end;

        LinePath(Vector3 start, Vector3 end) :
            start(start),
            end(end) {}

        Vector3 PositionAt(float position) override {
            return start + (end - start) * position;
        }
    };
} // namespace PJ

#endif
