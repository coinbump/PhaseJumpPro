#include "ExampleLifeMatrixRenderer.h"
#include "ExampleLifeAgent.h"
#include "ExampleLifeAgentGroup.h"
#include <stdio.h>

using namespace Example::Life;

MatrixRenderer::MatrixRenderer(Vector3 worldSize) :
    Base(worldSize),
    matrix({ 300, 300 }) {

    auto material = MAKE<RenderMaterial>(RenderMaterial::Config{ .shaderId = ShaderId::ColorVary });
    model.material = material;

    model.SetBuildMeshFunc([this](auto& model) {
        Mesh mesh;

        auto worldSize = model.WorldSize();
        Vector3 cellSize{ worldSize.x / matrix.Size().x, worldSize.y / matrix.Size().y, 0 };
        QuadMeshBuilder meshBuilder(cellSize);

        for (int y = 0; y < matrix.Size().y; y++) {
            for (int x = 0; x < matrix.Size().x; x++) {
                Vector2Int location(x, y);

                auto qm = meshBuilder.BuildMesh();
                qm.Offset(Vector2{ (worldSize.x / 2.0f) * vecLeft +
                                       ((cellSize.x / 2.0f + cellSize.x * x) * vecRight),
                                   (worldSize.y / 2.0f) * vecUp +
                                       ((cellSize.y / 2.0f + cellSize.y * y) * vecDown) });
                mesh += qm;
            }
        }

        // Optimize: fast bounds calculate
        mesh.calculateBoundsFunc = [=](auto& mesh) {
            return Bounds({}, Vector3(worldSize.x / 2.0f, worldSize.y / 2.0f, 0));
        };

        return mesh;
    });

    model.SetBuildVertexColorsFunc([this](auto const& model, auto& colors) {
        colors.resize(matrix.Size().x * matrix.Size().y * 4);

        size_t i = 0;
        for (int y = 0; y < matrix.Size().y; y++) {
            for (int x = 0; x < matrix.Size().x; x++) {
                Vector2Int location(x, y);

                Color color = Color::white;

                try {
                    auto cell = matrix.CellAt(location);
                    color = cell->core.isAlive ? Color::blue : Color::white;
                } catch (...) {}

                colors[i++] = color;
                colors[i++] = color;
                colors[i++] = color;
                colors[i++] = color;
            }
        }
    });
}

void MatrixRenderer::Awake() {
    Base::Awake();

    // Create a group for the agents
    group = MAKE<AgentGroup>(this);
    system.Add(group);

    auto random = StandardRandom();

    // Populate the simulation with agents
    for (int y = 0; y < matrix.Size().y; y++) {
        for (int x = 0; x < matrix.Size().x; x++) {
            auto agent = group->Add();
            GUARD(agent)

            agent->onStepFunc = LifeAgents::MakeOnStepFunc();
            agent->core.location = Vector2Int(x, y);

            // Add some random live cells
            if (random.Value() < 0.2f) {
                agent->core.isAlive = true;
            }

            matrix.SetCell(agent->core.location, agent);
        }
    }
}

void MatrixRenderer::OnUpdate(TimeSlice time) {
    Base::OnUpdate(time);
    system.OnUpdate(time);
}
