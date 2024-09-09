#include <stdio.h>

#include "ExampleLifeMatrixRenderer.h"
#include "ExampleLifeAgent.h"
#include "ExampleLifeAgentGroup.h"

ExampleLifeMatrixRenderer::ExampleLifeMatrixRenderer() : Base({600, 600, 0}), matrix({100, 100}) {
    auto program = SomeShaderProgram::registry.find("color.vary");
    GUARD(program != SomeShaderProgram::registry.end())

    auto material = MAKE<RenderMaterial>();
    material->SetShaderProgram(program->second);

    model.material = material;

    model.SetMeshBuilderFunc([this](auto& model) {
        Mesh mesh;

        Vector3 cellSize{model.WorldSize().x / matrix.Size().x, model.WorldSize().y / matrix.Size().y, 0};
        QuadMeshBuilder meshBuilder(cellSize);

        for (int y = 0; y < matrix.Size().y; y++) {
            for (int x = 0; x < matrix.Size().x; x++) {
                Vector2Int location(x, y);

                auto qm = meshBuilder.BuildMesh();
                qm.OffsetBy({(model.WorldSize().x / 2.0f) * vecLeft + ((cellSize.x / 2.0f + cellSize.x * x) * vecRight), (model.WorldSize().y / 2.0f) * vecUp + ((cellSize.y / 2.0f + cellSize.y * y) * vecDown)});
                mesh += qm;
            }
        }

        return mesh;
    });
}

void ExampleLifeMatrixRenderer::Awake()
{
    Base::Awake();

    // Create a group for the agents
    group = MAKE<ExampleLifeAgentGroup>(this);
    system.Add(group);

    auto random = StandardRandom();
    
    // Populate the simulation with agents
    for (int y = 0; y < matrix.Size().y; y++)
    {
        for (int x = 0; x < matrix.Size().x; x++)
        {
            auto agent = group->Add();
            GUARD(agent)

            agent->onStepFunc = ExampleLifeAgents::MakeOnStepFunc();
            agent->core.location = Vector2Int(x, y);

            // Add some random live cells
            if (random.Value() < 0.2f)
            {
                agent->core.isAlive = true;
                UpdateMatrixForAgent(*agent);
            }
        }
    }
}

void ExampleLifeMatrixRenderer::OnUpdate(TimeSlice time)
{
    Base::OnUpdate(time);

    // After user taps in view to change cell, pause the simulation for N seconds (not implemented)
//    if (updateSystemCountdown > 0)
//    {
//        updateSystemCountdown -= time.delta;
//        GUARD(updateSystemCountdown <= 0)
//    }

    system.OnUpdate(time);
}

void ExampleLifeMatrixRenderer::UpdateMatrixForAgent(ExampleLifeAgent const& agent)
{
    auto location = agent.core.location;
    matrix.SetCell(location, agent.core.isAlive ? CellState::Alive : CellState::Dead);
}

ExampleLifeAgent* ExampleLifeMatrixRenderer::AgentAt(Vector2Int location)
{
    // This is slow, but good enough for a demo
    for (size_t i = 0; i < group->Agents().PoolSize(); i++) {
        try {
            ExampleLifeAgent& agent = const_cast<ExampleLifeAgent&>(group->Agents().at(i));
            if (agent.core.location == location) {
                return &agent;
            }
        } catch (...) {
            return nullptr;
        }
    }

    return nullptr;
}

VectorList<RenderModel> ExampleLifeMatrixRenderer::MakeRenderModels() {
    VectorList<RenderModel> result;

    auto material = model.material;
    if (nullptr == material) {
        PJLog("ERROR. Missing material.");
        return result;
    }
    
    VectorList<RenderColor> colors;

    // OPTIMIZABLE: since this is a step-based simulation, we really only have to rebuild
    // colors after each step. Not every frame
    for (int y = 0; y < matrix.Size().y; y++) {
        for (int x = 0; x < matrix.Size().x; x++) {
            Vector2Int location(x, y);

            Color color = Color::white;

            try {
                color = matrix.CellAt(location) == CellState::Alive ? Color::blue : Color::white;
            } catch (...) {
            }

            colors.push_back(color);
            colors.push_back(color);
            colors.push_back(color);
            colors.push_back(color);
        }
    }

    RenderModelBuilder builder;
    VectorList<SomeTexture*> textures;
    auto renderModel = builder.Build(*this, model.Mesh(), *material, textures);

    renderModel.colors = colors;

    Add(result, renderModel);
    return result;
}
