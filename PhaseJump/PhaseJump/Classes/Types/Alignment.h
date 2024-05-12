#ifndef PJALIGNMENT_H
#define PJALIGNMENT_H

#include "HorizontalAlignment.h"
#include "VerticalAlignment.h"

/*
 RATING: 5 stars
 Utility wrapper
 CODE REVIEW: 4/14/24
 */
namespace PJ
{
    /// <summary>
    /// Combines horizontal and vertical alignment
    /// </summary>
    struct Alignment
    {
        HorizontalAlignment horizontalAlignment;
        VerticalAlignment verticalAlignment;

        Alignment() : horizontalAlignment(HorizontalAlignment::center), verticalAlignment(VerticalAlignment::center)
        {
        }

        Alignment(HorizontalAlignment horizontalAlignment, VerticalAlignment verticalAlignment) : horizontalAlignment(horizontalAlignment), verticalAlignment(verticalAlignment)
        {
        }

        static const Alignment center;
    };
}

#endif
