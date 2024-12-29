#pragma once

#include "SDKIncludes.h"

namespace Example {
    /**
     Builds a bar chart graphs example
     */
    namespace Graphs {
        class SceneModel {
        public:
            class Bar {
            public:
                String name;
                float value{};
            };

            VectorList<Bar> bars;

            float minAxisValue{};
            float maxAxisValue;
        };

        class Scene : public PJ::Scene {
        public:
            using Base = PJ::Scene;
            using This = Scene;

            SceneModel sceneModel;

            Scene();

            Vector2 worldSize{ 1200, 1200 };

            // MARK: SomeWorldComponent

            void LoadInto(WorldNode& root) override;

        protected:
            void RandomizeValues(bool isAnimated);
        };
    } // namespace Graphs
} // namespace Example
