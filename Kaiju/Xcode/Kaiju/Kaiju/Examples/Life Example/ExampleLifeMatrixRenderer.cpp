#include <stdio.h>

#include "ExampleLifeMatrixRenderer.h"
#include "ExampleLifeAgent.h"
#include "ExampleLifeAgentGroup.h"

using namespace ExampleLife;

MatrixRenderer::MatrixRenderer() : Base({1200, 1200, 0}), matrix({300, 300}) {
    auto program = SomeShaderProgram::registry.find("color.vary");
    GUARD(program != SomeShaderProgram::registry.end())

    auto material = MAKE<RenderMaterial>();
    material->SetShaderProgram(program->second);

    model.material = material;

    model.SetBuildMeshFunc([this](auto& model) {
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

    model.SetBuildVertexColorsFunc([this](auto const& model, auto& colors) {
        colors.clear();

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
    });
}

void MatrixRenderer::Awake()
{
    Base::Awake();

    // Create a group for the agents
    group = MAKE<AgentGroup>(this);
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

void MatrixRenderer::OnUpdate(TimeSlice time)
{
    Base::OnUpdate(time);
    system.OnUpdate(time);
}

void MatrixRenderer::UpdateMatrixForAgent(Agent const& agent)
{
    auto location = agent.core.location;
    matrix.SetCell(location, agent.core.isAlive ? CellState::Alive : CellState::Dead);
}
