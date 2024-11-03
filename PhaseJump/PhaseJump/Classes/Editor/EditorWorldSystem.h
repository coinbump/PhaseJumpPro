#pragma once

#include "CommandHistory.h"
#include "ImGuiPlanPainter.h"
#include "Menu.h"
#include "Scene.h"
#include "SomeWorldSystem.h"
#include "UIPlan.h"
#include "UnorderedMap.h"
#include "WorldNode.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 10/22/24
 */
namespace PJ {
    /// Standard properties and functionality for an ECS editor-based app
    class EditorWorldSystem : public SomeWorldSystem {
    public:
        /// Model for node inspection
        struct InspectNodeModel {
            /// The currently inspected node, if any
            WorldNode* node{};

            /// UI plans for components inside the inspected node
            UnorderedMap<SomeWorldComponent*, UP<UIPlan>> componentUIPlans;

            /// imGui storage for UI plans
            ImGuiPlanPainter::Storage imGuiStorage;

            /// Keep inspected components in memory, because ImGui needs them to render the latest
            /// pass and the world node might remove them
            WorldNode::ComponentList components;

            InspectNodeModel(WorldNode* node) :
                node(node) {
                GUARD(node)

                // TODO: this fixes the character controllers crash, but doesn't
                // update the inspector when new components are added
                components = node->Components();
            }
        };

        /// Model for painting render engine info and render processors
        struct RenderEnginePaintModel {
            bool optimizeStateSwitches = true;

            // Don't use bool vector, it is specialized by C++
            VectorList<char> renderProcessorSwitches;

            RenderEnginePaintModel() {}
        };

        /// If true, the editor imGui UI is visible
        bool isUIVisible = true;

        /// Register scene classes here for the scenes painter
        SceneClassRegistry sceneClasses;

        /// Command history for undo/redo
        CommandHistory commands;

        /// Model for node inspection
        UP<InspectNodeModel> inspectNodeModel;

        /// UI plan for the current scene
        UP<UIPlan> sceneUIPlan;

        /// imGui storage for UI plans
        ImGuiPlanPainter::Storage imGuiStorage;

        // MARK:  Actions

        /// Add the node to the specified parent (with undo/redo)
        void Add(SP<WorldNode> node, WorldNode* parent, size_t index);

        /// Delete the node (with undo/redo)
        void Delete(SP<WorldNode> node);

        /// Current menus
        VectorList<UP<Menu>> menus;

        RenderEnginePaintModel renderEnginePaintModel;

        EditorWorldSystem(int commandHistorySize = 100);
    };
} // namespace PJ
