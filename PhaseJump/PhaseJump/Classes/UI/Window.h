#pragma once

#include "ObservedValue.h"
#include "Vector2.h"
#include "WorldComponent.h"
#include "WorldSizeable.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 4/17/26
 */
namespace PJ {
    class WorldNode;

    /// Sent from window to content node when the window's size changes.
    class ContainerResizeEvent final : public SomeSignal {
    public:
        Vector2 worldSize;

        ContainerResizeEvent(Vector2 worldSize) :
            worldSize(worldSize) {}
    };

    /// Window configuration
    class WindowModel {
    public:
        bool hasTitleBar = true;
        bool hasCloseButton = true;
        bool hasMaximizeButton = true;
        bool hasFrame = true;
    };

    /// Determines how content is resized inside a container when the container changes size
    enum class ContentResizeType {
        Resize,

        Scale
    };

    /// Desktop-style draggable window with a title frame and a viewport-backed content area
    class Window : public WorldComponent, public WorldSizeable {
    protected:
        /// Higher z windows always stay above lower z windows
        int levelZ{};

    public:
        using Base = WorldComponent;
        using This = Window;

        using BuildContentFunc = std::function<void(WorldNode&)>;
        using CloseFunc = std::function<void(Window&)>;

        enum class State { Default, Maximized, Minimized };

        /// Horizontal resize axis for a resize child node
        enum class HResize { None, Left, Right };

        /// Vertical resize axis for a resize child node
        enum class VResize { None, Top, Bottom };

        /// Window world size. Changing resizes the frame and the content viewport.
        ObservedValue<Vector2> worldSize;

        ObservedValue<State> state{ State::Default };

        /// True when the window is the active (frontmost) window on its desktop.
        /// While true, the activate-overlay node is disabled so input passes through.
        ObservedValue<bool> isActive{ false };

        /// Height of the title frame along the top of the window.
        float titleBarHeight = 40.0f;

        /// Thickness of the edge/corner resize child nodes
        float frameExtent = 12.0f;

        /// Minimum world size the window can be resized to
        Vector2 minWindowSize{ 100, 100 };

        /// When > 0, resize maintains aspect ratio (defined as width / height)
        /// Example 16:9 aspect ratio is 16 / 9
        float aspectRatio = 0.0f;

        /// Controls how the content node reacts to window size changes.
        ContentResizeType contentResizeType = ContentResizeType::Resize;

        /// World size to restore when the window exits its Maximized state
        Vector2 defaultWorldSize{ 0, 0 };

        /// Called at the end of Awake with the newly-created content node
        BuildContentFunc buildContentFunc;

        /// Invoked by the close button instead of destroying the window directly.
        /// Defaulted by Desktop::NewWindow to call Desktop::Close.
        CloseFunc closeFunc;

        /// Configuration
        WindowModel model;

        Window();

        WorldNode* TitleBarNode() const {
            return titleBarNode.lock().get();
        }

        WorldNode* ContentNode() const {
            return contentNode.lock().get();
        }

        int LevelZ() const {
            return levelZ;
        }

        void SetLevelZ(int value) {
            levelZ = value;
        }

        // MARK: WorldSizeable

        Vector3 WorldSize() const override {
            auto size = worldSize.Value();
            return Vector3(size.x, size.y, 0);
        }

        void SetWorldSize(Vector3 value) override {
            worldSize = Vector2(value.x, value.y);
        }

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "Window";
        }

        void Awake() override;

        /// Re-applies the current layout
        void ApplyLayout() {
            ApplyLayout(worldSize.Value());
        }

    protected:
        WP<WorldNode> titleBarNode;
        WP<WorldNode> titleBarViewNode;
        WP<WorldNode> contentNode;
        WP<WorldNode> frameOverlayNode;

        WP<WorldNode> resizeLeftEdgeNode;
        WP<WorldNode> resizeRightEdgeNode;
        WP<WorldNode> resizeTopEdgeNode;
        WP<WorldNode> resizeBottomEdgeNode;
        WP<WorldNode> resizeTopLeftCornerNode;
        WP<WorldNode> resizeTopRightCornerNode;
        WP<WorldNode> resizeBottomLeftCornerNode;
        WP<WorldNode> resizeBottomRightCornerNode;
        WP<WorldNode> activateNode;

        /// Local position to restore when the window exits its Maximized state
        Vector3 defaultLocalPosition{};

        /// Content size captured after the initial layout. Zero until the first layout pass
        /// completes.
        Vector2 baseContentSize{};

        /// Builds a resize handle child node that drives a window resize along the given axes
        SP<WorldNode> BuildResizeNode(String name, HResize hResize, VResize vResize);

        /// Applies the current worldSize to the frame and content nodes
        void ApplyLayout(Vector2 size);

        /// Toggles the window between Default and Maximized states
        void ToggleMaximize();
    };
} // namespace PJ
