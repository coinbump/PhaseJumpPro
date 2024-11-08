#pragma once

#include "ExampleLifeAgent.h"
#include "SDKIncludes.h"

using namespace PJ;

namespace Example {
    namespace Life {
        class AgentGroup;

        /// Runs the simulation and renders it
        class MatrixRenderer : public SomeRenderer {
        public:
            using Base = SomeRenderer;

            AgentSystem system;
            SP<AgentGroup> group;

            Matrix<Agent*> matrix;

        public:
            MatrixRenderer();

            void Awake() override;
            void OnUpdate(TimeSlice time) override;

            void PostStep() {
                model.SetVertexColorsNeedsBuild();
            }

            // MARK: SomeWorldComponent

            String TypeName() const override {
                return "ExampleLife-MatrixRenderer";
            }
        };
    } // namespace Life
} // namespace Example
