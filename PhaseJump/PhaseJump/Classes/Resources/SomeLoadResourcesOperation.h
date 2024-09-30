#pragma once

#include "FilePath.h"
#include "OrderedMap.h"
#include "ResourceInfo.h"
#include "ResourceModels.h"
#include "Result.h"
#include "SomeOperation.h"
#include "Tags.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Utility type
 CODE REVIEW: 8/11/24
 */
namespace PJ {
    class LoadResourcesModel;

    using Exception = std::runtime_error;

    /// Loads N resources with arguments and places it in result
    class SomeLoadResourcesOperation : public SomeOperation {
    public:
        struct Success {
            // TODO: Should be UP
            using LoadOperationsList = List<SP<SomeLoadResourcesOperation>>;

            VectorList<ResourceModel> loadedResources;
            LoadOperationsList loadOperations;

            Success() {}

            Success(
                VectorList<ResourceModel> const& loadedResources,
                List<SP<SomeLoadResourcesOperation>> loadOperations
            ) :
                loadedResources(loadedResources),
                loadOperations(loadOperations) {}

            void Add(ResourceModel loadedResource) {
                loadedResources.push_back(loadedResource);
            }

            size_t size() const {
                return loadedResources.size();
            }
        };

        // VectorList for easy N-element access
        using Failure = std::exception;
        using Result = PJ::Result<Success, Failure>;

        // MARK: Input

        ResourceInfo const info;
        LoadResourcesModel& loadResourcesModel;

        // MARK: Output

        std::optional<Result> result;

        SomeLoadResourcesOperation(ResourceInfo info, LoadResourcesModel& loadResourcesModel) :
            info(info),
            loadResourcesModel(loadResourcesModel) {}

        /// @return Returns loaded resources if the operation was a success
        VectorList<ResourceModel> LoadedResources() const {
            VectorList<ResourceModel> result;

            GUARDR(this->result, result);
            GUARDR(this->result.value().IsSuccess(), result)

            auto successValue = this->result->SuccessValue();
            if (successValue) {
                result = successValue->loadedResources;
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

        virtual Result LoadResources() = 0;

        // MARK: SomeOperation

        void Run() override {
            result = LoadResources();
        }
    };
} // namespace PJ
