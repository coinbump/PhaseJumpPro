#pragma once

#include "ResourceRepositoryModel.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/20/24
 */
namespace PJ {
    /// Registers operations to load commonly used resource types
    class StandardResourceRepositoryModel : public ResourceRepositoryModel {
    public:
        StandardResourceRepositoryModel();
    };
} // namespace PJ
