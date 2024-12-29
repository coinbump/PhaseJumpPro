#pragma once

#include "FilePath.h"
#include "OrderedMap.h"
#include "ResourceCatalog.h"
#include "ResourceInfo.h"
#include "Result.h"
#include "SomeOperation.h"
#include "Tags.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 8/11/24
 */
namespace PJ {
    class ResourceRepositoryModel;

    using Exception = std::runtime_error;

    /// Loads N resources with arguments and places it in result
    class SomeLoadResourcesOperation : public SomeOperation {
    public:
        struct Success {
            using LoadOperationsList = VectorList<SP<SomeLoadResourcesOperation>>;

            VectorList<ResourceModel> resources;
            LoadOperationsList loadOperations;

            Success() {}

            Success(
                VectorList<ResourceModel> const& resources,
                VectorList<SP<SomeLoadResourcesOperation>> loadOperations
            ) :
                resources(resources),
                loadOperations(loadOperations) {}

            Success(ResourceModel model) :
                resources{ model } {}

            void Add(ResourceModel loadedResource) {
                resources.push_back(loadedResource);
            }

            size_t size() const {
                return resources.size();
            }
        };

        // VectorList for easy N-element access
        using Failure = std::exception;
        using Result = PJ::Result<Success, Failure>;

        // MARK: Input

        ResourceInfo const info;
        ResourceRepositoryModel& repoModel;

        // MARK: Output

        std::optional<Result> result;

        SomeLoadResourcesOperation(ResourceInfo info, ResourceRepositoryModel& repoModel) :
            info(info),
            repoModel(repoModel) {}

        /// @return Returns loaded resources if the operation was a success
        VectorList<ResourceModel> Resources() const {
            VectorList<ResourceModel> result;

            GUARDR(this->result, result);
            GUARDR(this->result.value().IsSuccess(), result)

            auto successValue = this->result->SuccessValue();
            if (successValue) {
                result = successValue->resources;
            }

            return result;
        }

        /// @return Returns new load operations if the operation was a success
        SomeLoadResourcesOperation::Success::LoadOperationsList LoadOperations() const {
            SomeLoadResourcesOperation::Success::LoadOperationsList result;

            GUARDR(this->result, result);
            GUARDR(this->result.value().IsSuccess(), result)

            auto successValue = this->result->SuccessValue();
            if (successValue) {
                result = successValue->loadOperations;
            }

            return result;
        }

        /// @return Returns the first loaded resource of the specified type if it exists
        template <class Type>
        SP<Type> TypeSuccessFirst() const {
            auto resources = Resources();
            GUARDR(!IsEmpty(resources), {})

            auto result = DCAST<Type>(resources[0].resource);
            return result;
        }

        virtual Result LoadResources() = 0;

        // MARK: SomeOperation

        void Run() override {
            result = LoadResources();
        }
    };
} // namespace PJ
