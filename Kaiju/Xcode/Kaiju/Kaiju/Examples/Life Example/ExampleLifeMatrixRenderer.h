#pragma once

#if DEVELOPMENT
#include <PhaseJump-Dev/PhaseJump-Dev.h>
#else
#include <PhaseJump/PhaseJump.h>
#endif

#include "ExampleLifeAgent.h"

using namespace PJ;

namespace ExampleLife {
    class AgentGroup;

    /// Runs the simulation and renders it
    class MatrixRenderer : public SomeRenderer
    {
    public:
        using Base = SomeRenderer;

        AgentSystem system;
        float updateSystemCountdown = 0;
        SP<AgentGroup> group;

        enum class CellState {
            Dead,

            Alive
        };

        Matrix<CellState> matrix;

    public:
        MatrixRenderer();

        void Awake() override;
        void OnUpdate(TimeSlice time) override;
        void UpdateMatrixForAgent(Agent const& agent);

        void PostStep() {
            model.SetVertexColorsNeedsBuild();
        }

        // MARK: SomeWorldComponent

        String TypeName() const override { return "EampleLife-MatrixRenderer"; }
    };
}
