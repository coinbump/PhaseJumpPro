#pragma once

#include "FilePath.h"
#include "LoadedResources.h"
#include "LoadResourceInfo.h"
#include "OrderedMap.h"
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
            // TODO: SP-Audit
            using LoadOperationsList = List<SP<SomeLoadResourcesOperation>>;

            VectorList<LoadedResource> loadedResources;
            LoadOperationsList loadOperations;

            Success() {}

            Success(
                VectorList<LoadedResource> const& loadedResources,
                List<SP<SomeLoadResourcesOperation>> loadOperations
            ) :
                loadedResources(loadedResources),
                loadOperations(loadOperations) {}

            void Add(LoadedResource loadedResource) {
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

        LoadResourceInfo const info;
        LoadResourcesModel& loadResourcesModel;

        // MARK: Output

        std::optional<Result> result;

        SomeLoadResourcesOperation(LoadResourceInfo info, LoadResourcesModel& loadResourcesModel) :
            info(info),
            loadResourcesModel(loadResourcesModel) {}

        /// Returns loaded resources if the operation was a success
        VectorList<LoadedResource> LoadedResources() const {
            VectorList<LoadedResource> result;

            GUARDR(this->result, result);
            GUARDR(this->result.value().IsSuccess(), result)

            auto successValue = this->result->SuccessValue();
            if (successValue) {
                result = successValue->loadedResources;
            }

            return result;
        }

        /// Returns new load operations if the operation was a success
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
