#pragma once

#include "SDKIncludes.h"

namespace Example {
    namespace Checkers {
        /// Renders the 8x8 checkboard with a mesh
        class MatrixRenderer : public Renderer {
        public:
            using Base = Renderer;
            using This = MatrixRenderer;

            MaterialRendererCore core;

        public:
            MatrixRenderer(Vector3 worldSize);

            // MARK: Renderer

            RenderModelList RenderModels() override {
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
                return "ExampleCheckers-MatrixRenderer";
            }
        };
    } // namespace Checkers
} // namespace Example
