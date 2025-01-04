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
            WP<WorldNode> node;

            InspectNodeModel(SP<WorldNode> node) :
                node(node) {
                GUARD(node)
            }
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

        PJ::Storage<String> storage;

        // MARK:  Actions

        /// Add the node to the specified parent (with undo/redo)
        void Add(SP<WorldNode> node, WorldNode* parent, size_t index);

        /// Delete the node (with undo/redo)
        void Delete(SP<WorldNode> node);

        /// Current menus
        VectorList<UP<Menu>> menus;

        EditorWorldSystem(int commandHistorySize = 100);
    };
} // namespace PJ
