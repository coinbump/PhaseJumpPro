#pragma once

#include "Color.h"
#include "DragHandler2D.h"
#include "WorldNode.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 9/9/24
 */
// TODO: needs unit tests for And/pop
namespace PJ {
    class DesignSystem;
    class SomeTexture;

    /// Convenience methods for adding common components
    /// Use to quickly build scenes for rapid prototyping
    struct QuickBuild {
    public:
        using This = QuickBuild;
        using OnDragUpdateFunc = std::function<void(DragHandler2D&)>;

    protected:
        VectorList<SomeWorldComponent*> components;

        void AddSlider(
            World& world, WorldNode& parent, DesignSystem& designSystem,
            SP<SomeTexture> trackTexture, SP<SomeTexture> thumbTexture, String name,
            Vector2 worldSize, Axis2D axis, std::function<void(float)> valueFunc
        );

        This& SliderVertical(String name, Vector2 worldSize, std::function<void(float)> valueFunc);

        WorldNode& Node() {
            return nodes[nodes.size() - 1];
        }

    public:
        VectorList<std::reference_wrapper<WorldNode>> nodes;

        QuickBuild(WorldNode& node) {
            nodes.push_back(node);
        }

        /// Modify all components of this type
        template <class T>
        This& ModifyAll(std::function<void(T& component, size_t index)> func) {
            size_t index = 0;

            std::for_each(components.cbegin(), components.cend(), [&](auto component) {
                auto concrete = dynamic_cast<T*>(component);
                GUARD(concrete)

                func(*concrete, index);
                index++;
            });

            return *this;
        }

        /// Modify the last created component of this type
        template <class T>
        This& ModifyLatest(std::function<void(T& component)> func) {
            for (auto i = components.crbegin(); i != components.crend(); i++) {
                auto concrete = dynamic_cast<T*>(*i);
                GUARD_CONTINUE(concrete)

                func(*concrete);
                break;
            };

            return *this;
        }

        This& AndPrefab(String id);

        // TODO: add 'With' prefix to make it clear that these add components?
        This& Circle(float radius, Color color = Color::gray);
        This& Drag(OnDragUpdateFunc onDragUpdateFunc = [](auto&) {});
        This& SquareCollider(float size);
        This& RectCollider(Vector2 size);
        This& CircleCollider(float radius);
        This& Slider(String name, Vector2 worldSize, std::function<void(float)> valueFunc);

        This& Named(String name) {
            Node().name = name;
            return *this;
        }

        This& SetLocalPosition(Vector3 position) {
            Node().SetLocalPosition(position);
            return *this;
        }

        This& SetWorldPosition(Vector3 position) {
            Node().SetWorldPosition(position);
            return *this;
        }

        constexpr This& Push(WorldNode& node) {
            nodes.push_back(node);
            return *this;
        }

        constexpr This& Pop() {
            // Can't pop the first node
            GUARDR(nodes.size() > 1, *this)
            nodes.erase(nodes.begin() + (nodes.size() - 1));
            return *this;
        }

        constexpr This& PopToRoot() {
            while (nodes.size() > 1) {
                Pop();
            }
            return *this;
        }

        constexpr This& ToParent() {
            GUARDR(nodes.size() > 0 && nodes[0].get().Parent(), *this)
            Push(*nodes[0].get().Parent());
            return *this;
        }

        constexpr This& operator++() {
            return And();
        }

        constexpr This& operator++(int) {
            return And();
        }

        constexpr This& operator--() {
            return Pop();
        }

        constexpr This& operator--(int) {
            return Pop();
        }

        template <typename... Arguments>
        constexpr This& AddNode(Arguments... args) {
            auto& result = Node().AddNode(args...);
            nodes.push_back(result);
            return *this;
        }

        template <typename... Arguments>
        constexpr This& And(Arguments... args) {
            return AddNode(args...);
        }

        template <class T, typename... Arguments>
        constexpr This& With(Arguments... args) {
            components.push_back(&Node().AddComponent<T>(args...));
            return *this;
        }

        // FUTURE:
        //        This& AddCircleHandle(
        //            float radius, Color color = Color::gray, float strokeWidth = 4,
        //            Color strokeColor = Color::black
        //        );
        //        This& AddCircleHandle(float radius, float strokeWidth = 4, Theme* theme =
        //        nullptr);
    };

    using QB = QuickBuild;

} // namespace PJ
