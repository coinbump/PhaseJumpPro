#pragma once

#include <PhaseJump/PhaseJump.h>

#include "ExampleLifeAgent.h"

using namespace PJ;

class ExampleLifeAgentGroup;

/// Runs the simulation and renders it
class ExampleLifeMatrixRenderer : public SomeRenderer
{
public:
    using Base = SomeRenderer;

    AgentSystem system;
    float updateSystemCountdown = 0;
    SP<ExampleLifeAgentGroup> group;

    enum class CellState {
        Dead,

        Alive
    };

    Matrix<CellState> matrix;

public:
    ExampleLifeMatrixRenderer();

    void Awake() override;
    void OnUpdate(TimeSlice time) override;
    void UpdateMatrixForAgent(ExampleLifeAgent const& agent);
    ExampleLifeAgent* AgentAt(Vector2Int location);

    // MARK: SomeRenderer

    VectorList<RenderModel> MakeRenderModels() override;
   
    // MARK: SomeWorldComponent

    String TypeName() const override { return "ExampleLifeMatrixRenderer"; }
};
