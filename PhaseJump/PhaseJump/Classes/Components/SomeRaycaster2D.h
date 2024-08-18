#ifndef PJSOMERAYCASTER2D_H
#define PJSOMERAYCASTER2D_H

#include "SomeRaycaster.h"

namespace PJ {
    struct RaycastHit2D {
        // TODO: WP?
        SP<WorldNode> node;

        RaycastHit2D(SP<WorldNode> node) :
            node(node) {}
    };

    /// Handles 2D Raycast tests
    class SomeRaycaster2D : public SomeRaycaster {
    public:
        virtual std::optional<RaycastHit2D> Raycast(Vector2 origin, Vector2 direction) = 0;
    };
} // namespace PJ

#endif
