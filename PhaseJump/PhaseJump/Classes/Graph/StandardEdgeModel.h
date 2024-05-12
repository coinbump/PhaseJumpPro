#ifndef PJSTANDARDEDGEMODEL_H
#define PJSTANDARDEDGEMODEL_H

#include "_String.h"
#include "Tags.h"
#include "Macros.h"
#include <memory>

/*
 RATING: 5 stars
 Simple model
 CODE REVIEW: 4/14/24
 */
namespace PJ
{
    /// <summary>
    /// Default implementation of an edge model for a graph. Or you can create your own
    /// </summary>
    struct StandardEdgeModel
    {
        String id; // Example: north, west, left, right
        String type; // Example: input, output
        float weight = 0;
        Tags tags;

        StandardEdgeModel(String id = "",
                          String type = "",
                          float weight = 1.0f,
                          Tags const& tags = Tags()) :
        id(id),
        type(type),
        weight(weight),
        tags(tags)
        {
        }
    };
}

#endif
