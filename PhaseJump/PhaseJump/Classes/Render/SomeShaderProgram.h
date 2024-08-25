#pragma once

#include "StringUtils.h"
#include "UnorderedMap.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 8/8/24
 */
namespace PJ {
    class SomeShaderProgram {
    public:
        String id;

        virtual ~SomeShaderProgram() {}

        /// Store loaded programs here for access later
        static UnorderedMap<String, SP<SomeShaderProgram>> registry;
    };
} // namespace PJ
