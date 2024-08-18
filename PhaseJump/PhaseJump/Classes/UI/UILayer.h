#ifndef PJUILAYER_H
#define PJUILAYER_H

#include "Utils.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 6/18/23
 */
namespace PJ {
    class WorldNode;

    /// Specifies which objects are the parent layers for Z-sorting
    /// (Sort Layers only work with sprites)
    struct UILayer {
        String id;
        SP<WorldNode> parent;

        /// If > 0, used for zStep to next layer
        float zStep;

        /// (OPTIONAL) Scene identifier
        /// Used when we load a scene as a UILayer
        String sceneId;
    }
} // namespace PJ

#endif
