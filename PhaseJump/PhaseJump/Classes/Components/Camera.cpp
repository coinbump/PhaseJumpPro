#include "Camera.h"
#include "RenderProcessor.h"
#include "UIPlanner.h"

using namespace std;
using namespace PJ;

Camera::Camera() {
    PlanUIFunc planUIFunc = [this](auto args) {
        if (!IsEmpty(processingModel.Processors())) {
            args.planner.LabelText({ .text = "Render Processors" });

            for (auto& processor : processingModel.Processors()) {
                GUARD(!IsEmpty(processor->name));

                args.planner.InputBool(
                    { .label = processor->name,
                      .binding = Binding<bool>(
                          { .getFunc = [processor]() { return processor->IsEnabled(); },
                            .setFunc = [processor](auto& value) { processor->Enable(value); } }
                      ) }
                );
            }
        }
    };
    Override(planUIFuncs[UIContextId::Inspector], planUIFunc);
}
