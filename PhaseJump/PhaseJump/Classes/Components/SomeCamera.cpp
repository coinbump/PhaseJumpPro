#include "SomeCamera.h"
#include "RenderProcessor.h"
#include "UIPlanner.h"

using namespace std;
using namespace PJ;

SomeCamera::SomeCamera() {
    PlanUIFunc planUIFunc = [this](auto args) {
        if (!IsEmpty(processingModel.Processors())) {
            args.planner.Text({ .text = "Render Processors" });

            for (auto& processor : processingModel.Processors()) {
                GUARD(!IsEmpty(processor->name));

                args.planner.InputBool(
                    { .label = processor->name,
                      .binding = { [processor]() { return processor->IsEnabled(); },
                                   [processor](auto& value) { processor->Enable(value); } } }
                );
            }
        }
    };
    Override(planUIFuncs[UIContextId::Inspector], planUIFunc);
}
