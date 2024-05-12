#ifndef PJUILAYER_H
#define PJUILAYER_H

#include "Macros.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 6/18/23
 */
namespace PJ
{
    class WorldNode;
    
    /// <summary>
    /// Specifies which objects are the parent layers for Z-sorting
    /// (Sort Layers only work with sprites)
    /// </summary>
    struct UILayer
    {
        String id;
        SP<WorldNode> parent;

        /// <summary>
        /// If > 0, used for zStep to next layer
        /// </summary>
        float zStep;

        /// <summary>
        /// (OPTIONAL) Scene identifier
        /// Used when we load a scene as a UILayer
        /// </summary>
        String sceneId;
    }
}

#endif
