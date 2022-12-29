#ifndef PJSTANDARDEDGEMODEL_H
#define PJSTANDARDEDGEMODEL_H

#include "_String.h"
#include "Tags.h"
#include <memory>

/*
 RATING: 5 stars
 Simple model
 CODE REVIEW: 12/10/22
 */
namespace PJ
{
    /// <summary>
    /// Default implementation of an edge model. Or you can create your own
    /// </summary>
    struct StandardEdgeModel
    {
        String id; // Example: north, west, left, right
        String type; // Example: input, output
        float weight = 0;
        std::shared_ptr<Tags> tags;

        StandardEdgeModel(String id = "",
                          String type = "",
                          float weight = 1.0f,
                          std::shared_ptr<Tags> tags = std::make_shared<Tags>()) :
        id(id),
        type(type),
        weight(weight),
        tags(tags)
        {
        }
    };
}

#endif
