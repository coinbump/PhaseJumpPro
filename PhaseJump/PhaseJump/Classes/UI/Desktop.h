#pragma once

#include "ObservedValue.h"
#include "Vector2.h"
#include "VectorList.h"
#include "Window.h"
#include "WorldComponent.h"
#include "WorldSizeable.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 4/18/26
 */
namespace PJ {
    // TODO: needs unit tests
    class WorldNode;

    /// Class that defines a window level via semantics
    class WindowLevel {
    public:
        using This = WindowLevel;

        virtual ~WindowLevel() {}

        virtual int Z() const = 0;
    };

    /// Normal window level is the default
    class NormalWindowLevel : public WindowLevel {
    public:
        int Z() const override {
            return 0;
        }
    };

    /// These float above normal windows
    class FloatingWindowLevel : public WindowLevel {
    public:
        int Z() const override {
            return 10;
        }
    };

    /// Alerts appear above floating windows
    class AlertWindowLevel : public WindowLevel {
    public:
        int Z() const override {
            return 20;
        }
    };

    /// For background windows
    class DesktopWindowLevel : public WindowLevel {
    public:
        int Z() const override {
            return -10;
        }
    };

    class WindowStyle {
    public:
        virtual ~WindowStyle() {}

        virtual WindowModel Model() const = 0;
    };

    /// The default window style
    class NormalWindowStyle : public WindowStyle {
    public:
        WindowModel Model() const override {
            return { .hasTitleBar = true,
                     .hasCloseButton = true,
                     .hasMaximizeButton = true,
                     .hasFrame = true };
        }
    };

    /// Borderless window with no title bar
    class PlainWindowStyle : public WindowStyle {
    public:
        WindowModel Model() const override {
            return { .hasTitleBar = false,
                     .hasCloseButton = false,
                     .hasMaximizeButton = false,
                     .hasFrame = false };
        }
    };

    /// Hosts a set of child windows in a desktop-style layout
    class Desktop : public WorldComponent, public WorldSizeable {
    public:
        using Base = WorldComponent;
        using This = Desktop;

        /// Config for creating a window on a desktop
        struct WindowConfig {
            /// Window origin in view reading coordinates (top-left is {0, 0}, +y is down)
            Vector2 origin;

            /// Window world size
            Vector2 worldSize;

            /// Called with the new window's content node after the window is built
            std::function<void(WorldNode&)> buildContentFunc;

            /// Optional close behavior. If unset, use Desktop::Close
            std::function<void(Window&)> closeFunc;

            /// Window level for the window
            UP<WindowLevel> windowLevel = NEW<NormalWindowLevel>();

            /// Window style for the window
            UP<WindowStyle> windowStyle = NEW<NormalWindowStyle>();
        };

        /// Desktop world size.
        ObservedValue<Vector2> worldSize;

        Desktop();

        /// @return Window components attached to the desktop's child nodes
        VectorList<SP<Window>> Windows() const;

        /// Creates a new child node with a Window component
        SP<Window> NewWindow(WindowConfig const& config);

        /// Brings window to the front of its z-level group and activates it.
        /// Windows in other z-level groups are unaffected; activation is exclusive
        /// only within the activated window's level.
        void Activate(Window& window);

        /// Destroys window and activates the next frontmost window in the same z-level group.
        void Close(Window& window);

        /// @return The active window in `windowLevel`'s z group, or nullptr if none is active.
        Window* ActiveWindow(WindowLevel const& windowLevel) const;

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
            return "Desktop";
        }
    };
} // namespace PJ
