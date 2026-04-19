#pragma once

#include "ExampleLifeAgent.h"
#include "ExampleLifeAgentGroup.h"
#include "SDKIncludes.h"

using namespace PJ;

namespace Example {
    namespace Life {
        class AgentGroup;

        /// Runs the simulation and renders it
        class MatrixRenderer : public Renderer {
        public:
            using Base = Renderer;
            using This = MatrixRenderer;

            MaterialRendererCore core;

            AgentSystem system;
            SP<AgentGroup> group;

            Matrix<Agent*> matrix;

        public:
            MatrixRenderer(Vector3 worldSize);

            void Awake() override;
            FinishType OnUpdate(TimeSlice time) override;

            void PostStep() {
                core.model.SetVertexColorsNeedsBuild();
            }

            // MARK: Renderer

            VectorList<RenderModel> RenderModels() override {
                return core.RenderModels();
            }

            Color GetColor() const override {
                return core.GetColor();
            }

            void SetColor(Color color) override {
                core.SetColor(color);
            }

            // MARK: WorldSizeable

            Vector3 WorldSize() const override {
                return core.model.WorldSize();
            }

            void SetWorldSize(Vector3 value) override {
                core.model.SetWorldSize(value);
            }

            // MARK: SomeWorldComponent

            String TypeName() const override {
                return "ExampleLife-MatrixRenderer";
            }
        };
    } // namespace Life
} // namespace Example
