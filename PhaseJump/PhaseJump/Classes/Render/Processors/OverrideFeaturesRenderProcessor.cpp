#include "OverrideFeaturesRenderProcessor.h"
#include "RenderWorldSystem.h"

using namespace std;
using namespace PJ;

void OverrideFeaturesRenderProcessor::Process(CameraRenderModel& cameraModel) {
// #define LOG
#ifdef LOG
    for (auto& feature : map) {
        cout << "Override feature: " << feature.first << std::endl;
    }
#endif

    for (auto& model : cameraModel.models) {
        auto modelMaterial = model.Material();
        GUARD_CONTINUE(modelMaterial)

        for (auto& feature : map) {
            // No override needed
            try {
                if (feature.second == modelMaterial->Features().at(feature.first)) {
                    continue;
                }
            } catch (...) {}

            auto overrideMaterial = cameraModel.MakeOverrideMaterial(model);
            GUARD_CONTINUE(overrideMaterial)

            overrideMaterial->EnableFeature(
                feature.first, feature.second == RenderFeatureState::Enable
            );
        }

        auto overrideMaterial = cameraModel.OverrideMaterial(model);
        if (overrideMaterial) {
            model.SetOverrideMaterial(overrideMaterial.get());
        }
    }
}
