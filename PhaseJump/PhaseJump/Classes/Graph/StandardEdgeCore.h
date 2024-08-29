#pragma once

#include "StringUtils.h"
#include "Tags.h"
#include "Utils.h"
#include <memory>

/*
 RATING: 5 stars
 Simple model
 CODE REVIEW: 8/25/24
 */
namespace PJ {
    /// Default implementation of an edge model for a graph
    struct StandardEdgeCore {
        /// Edge identifier. Example: north, west, left, right
        String id;

        /// Edge type. Example: input, output
        String type;

        /// Edge weight for weighted graphs
        float weight = 0;

        /// Custom properties
        Tags tags;

        StandardEdgeCore(
            String id = "", String type = "", float weight = 1.0f, Tags const& tags = Tags()
        ) :
            id(id),
            type(type),
            weight(weight),
            tags(tags) {}
    };
} // namespace PJ
