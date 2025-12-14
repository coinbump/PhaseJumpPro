#pragma once
#include "TreeNode.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 10/22/24
 */
namespace PJ {
    /// Paints on demand. Example: immediate mode rendering
    class SomePainter {
    public:
        virtual ~SomePainter() {};

        /// Draws this node and its children
        virtual void Paint() = 0;
    };

    /// Protocol for an object that renders imGui UI elements
    /// Used to build a tree of imGui elements and re-arrange them as needed
    class SomeImGuiPainter : public Treeable<SomeImGuiPainter>, public SomePainter {
    public:
        using This = SomeImGuiPainter;
        using Func = std::function<void(This&)>;
        using TreeNode = PJ::TreeNode<This, UP<This>>;

        /// Stores child nodes (example: parent is a scroll view with children)
        TreeNode tree;

        /// Called to start rendering the immediate UI
        Func beginFunc;

        /// Called to render the immediate UI
        Func drawFunc;

        /// Called to finish rendering the immediate UI
        Func endFunc;

        SomeImGuiPainter() :
            tree(*this) {}

        ~SomeImGuiPainter() override {};

        // MARK: SomePainter

        /// Draws this node and its children
        void Paint() override;

        // MARK: Treeable

        This* Parent() const override {
            return tree.Parent();
        }

        void SetParent(This* value) override {
            tree.SetParent(value);
        }

    protected:
        virtual void Begin();
        virtual void Draw();
        virtual void DrawChildren();
        virtual void End();
    };

    // MARK: - Common painters

    /// Paints an imGui window
    class ImGuiWindowPainter : public SomeImGuiPainter {
    public:
        String name;
        bool* isVisible{};

        ImGuiWindowPainter(String name, bool* _isVisible);
    };

    /// Paints an imGui child area inside a window
    class ImGuiChildPainter : public SomeImGuiPainter {
    public:
        String name;

        ImGuiChildPainter(String name);
    };
} // namespace PJ
