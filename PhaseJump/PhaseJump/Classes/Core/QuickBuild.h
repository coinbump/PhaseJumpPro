#pragma once

#include "AnimationCycleTypes.h"
#include "Animator.h"
#include "Color.h"
#include "DragHandler2D.h"
#include "EaseFunc.h"
#include "WorldNode.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 9/9/24
 */
// TODO: needs unit tests
namespace PJ {
    class DesignSystem;
    class SomeTexture;

    /// Convenience methods for adding common components
    /// Use to quickly build scenes for rapid prototyping
    struct QuickBuild {
    public:
        using This = QuickBuild;
        using OnDragUpdateFunc = std::function<void(DragHandler2D&)>;
        using OnDropFilesFunc =
            std::function<void(std::tuple<SomeWorldComponent&, DropFilesUIEvent const&>)>;

    protected:
        /// Stores animation setting states so we can push and pop to previous states
        class AnimateStateModel {
        public:
            float duration = 0.3f;
            EaseFunc easeFunc = EaseFuncs::outSquared;
            float delay = 0;
            float valveTurnOnDuration = 0.3f;
            float valveTurnOffDuration = 0.3f;
        };

        VectorList<SomeWorldComponent*> components;
        VectorList<AnimateStateModel> animateStates{ {} };

        AnimateStateModel& AnimateState() {
            Assert(!IsEmpty(animateStates));
            return animateStates[animateStates.size() - 1];
        }

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

        /// Modify the last created component of any type
        This& ModifyLatestAny(std::function<void(SomeWorldComponent& component)> func) {
            for (auto i = components.crbegin(); i != components.crend(); i++) {
                func(**i);
                break;
            };

            return *this;
        }

        This& AndPrefab(String id);

        This& Repeat(int count, std::function<void(This&)> func);

        /// Add standard orthographic camera and associated components
        This& OrthoStandard(Color clearColor = Color::white);

        /// Add circle render component
        This& Circle(float radius, Color color = Color::gray);

        /// Add rect frame render component
        This& RectFrame(Vector2 size, Color color = Color::black, float strokeWidth = 2);

        /// Add square frame render component
        This& SquareFrame(float size, Color color = Color::black, float strokeWidth = 2);

        /// Add grid render component
        This&
        Grid(Vector2 worldSize, Vec2I gridSize, Color color = Color::black, float strokeWidth = 1);

        /// Make the current node draggable
        This& Drag(OnDragUpdateFunc onDragUpdateFunc = [](auto&) {});

        /// Make the current node draggable and snap back to original position when drag is finished
        This& DragSnapBack(OnDragUpdateFunc onDragUpdateFunc = [](auto&) {});

        /// Respond to a drop files event
        This& OnDropFiles(OnDropFilesFunc onDropFilesFunc);

        /// Add a square collider
        This& SquareCollider(float size);

        /// Add a rect collider
        This& RectCollider(Vector2 size);

        /// Add a circle collider
        This& CircleCollider(float radius);

        /// Add a slider control and thumb
        This& Slider(String name, Vector2 worldSize, std::function<void(float)> valueFunc);

        /// Rename the current node
        This& Named(String name) {
            Node().name = name;
            return *this;
        }

        /// Move the current node in local coordinates
        This& SetLocalPosition(Vector3 position) {
            Node().SetLocalPosition(position);
            return *this;
        }

        /// Move the current node in world coordinates
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

        This& With(SP<SomeWorldComponent> component) {
            GUARDR(component, *this)
            Node().Add(component);
            components.push_back(component.get());
            return *this;
        }

        // FUTURE:
        //        This& AddCircleHandle(
        //            float radius, Color color = Color::gray, float strokeWidth = 4,
        //            Color strokeColor = Color::black
        //        );
        //        This& AddCircleHandle(float radius, float strokeWidth = 4, Theme* theme =
        //        nullptr);

        // MARK: - Animate

        /// Add cycle hue effect
        This& CycleHueEffect(float cycleTime = 1);

        /// Set the animate duration for subsequent animations
        This& SetAnimateDuration(float value);

        /// Set the animate ease func for subsequent animations
        This& SetAnimateEase(EaseFunc value);

        /// Set the animate delay for subsequent animations
        This& SetAnimateDelay(float value);

        /// Set valve turn on/off durations
        This& SetValveDurations(float turnOnDuration, float turnOffDuration);

        // MARK: Hover gestures

        /// Scale node when in hover state
        This& HoverScaleTo(float endValue);

        /// Scale node when in hover state and ping pong continuously to the original value
        This& HoverScaleToPingPong(float endValue);

        template <class Type>
        This&
        AnimateStartEnd(Type startValue, Type endValue, MakeAnimatorFunc<Type, WorldNode&> maker) {
            auto& node = Node();
            auto animator = maker(startValue, endValue, node);
            node.updatables.AddDelay(std::max(0.0f, AnimateState().delay), animator);
            return *this;
        }

        template <class Type>
        This&
        AnimateCycle(Type startValue, Type endValue, MakeAnimatorFunc<Type, WorldNode&> maker) {
            auto& node = Node();
            auto animator = maker(startValue, endValue, node);
            node.updatables.AddDelay(std::max(0.0f, AnimateState().delay), animator);
            return *this;
        }

        // MARK: Move animations

        /// Duplicate the current animation settings
        This& PushAnimateSettings();

        /// Restore the previous animation settings
        This& PopAnimateSettings();

        /// Add a position animation with start and end values
        This& AnimateMove(Vector3 startValue, Vector3 endValue);

        /// Add a position animation with a start value
        This& MoveIn(Vector3 startValue);

        /// Add a position animation with an end value
        This& MoveTo(Vector3 endValue);

        // MARK: Scale animations

        /// Add uniform scale animation with start and end values
        This& AnimateScale(float startValue, float endValue);

        /// Add uniform scale animation from start value to 1
        This& ScaleIn(float startValue);

        /// Add uniform scale animation from current to end value
        This& ScaleTo(float endValue);

        /// Scale to an end value and continue in an animation cycle
        This& ScaleToPingPong(float endValue);

        // MARK: Rotate animations

        /// Add rotate animation with start and end values
        This& AnimateRotate(Angle startValue, Angle endValue);

        /// Add rotate animation from start value to 1
        This& RotateIn(Angle startValue);

        /// Add rotate animation from current to end value
        This& RotateTo(Angle endValue);

        /// Add continuous rotate animation (in angles per second)
        This& RotateContinue(Angle value);

        /// Rotate to an end value and continue in an animation cycle
        This& RotateToPingPong(Angle endValue);

        // MARK: Alpha animations

        // TODO: rename to AnimateAlpha?
        /// Adds opacity animation with start and end values
        This& AnimateOpacity(float startValue, float endValue);

        /// Add opacity animation from start value to 1
        This& OpacityIn(float startValue = 0);

        /// Add opacity animation from start value to 0
        This& OpacityOut();

        /// Add opacity animation from current to end value
        This& OpacityTo(float endValue);

        /// Destroy the current node in N seconds
        This& Destroy(float time);

        /// Destroy the current node after animation is finished
        This& DestroyAfterAnimate();
    };

    using QB = QuickBuild;

} // namespace PJ
