#pragma once

#include "AlignFunc.h"
#include "AnimatedSpriteRenderer.h"
#include "AnimationCycleTypes.h"
#include "Animator.h"
#include "CharacterController.h"
#include "Color.h"
#include "DragHandler2D.h"
#include "DropFilesUIEvent.h"
#include "EaseFunc.h"
#include "Font.h"
#include "LayoutInsets.h"
#include "NodeHandler.h"
#include "RenderProcessor.h"
#include "SimpleAnimationController.h"
#include "Switchable.h"
#include "ViewBuilder.h"
#include "WorldNode.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 9/9/24
 */
namespace PJ {
    class DropFilesUIEvent;
    class DesignSystem;
    class SomeTexture;

    /// Model to build a state transition
    struct StateTransitionModel {
        /// Source state for transition. If not defined, transition will be added for any state
        std::optional<String> state;

        /// Input that activates the transition
        String input;

        /// Destination state for input
        String toState;
    };

    /// Convenience methods for adding common components
    /// Use to quickly build scenes for rapid prototyping
    class QuickBuilder : public Base {
    public:
        using This = QuickBuilder;

        struct OnDropFilesArgs {
            SomeWorldComponent& component;
            DropFilesUIEvent event;
        };

        using OnDragUpdateFunc = std::function<void(DragHandler2D&)>;
        using OnDropFilesFunc = std::function<void(OnDropFilesArgs args)>;

        struct SliderConfig {
            Axis2D axis = Axis2D::X;
            Binding<float> valueBinding;
            Vector2 worldSize;

            /// Orthogonal size of the slider track perpendicular to the primary axis
            float trackOrthogonal = 10;
        };

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

        /// Rate for frame animations (FPS)
        float frameRate = 24;

        /// Cycle type for frame animations
        AnimationCycleType cycleType = AnimationCycleType::Loop;

        AnimateStateModel& AnimateState() {
            Assert(!IsEmpty(animateStates));
            return animateStates[animateStates.size() - 1];
        }

        void AddSlider(
            World& world, WorldNode& parent, DesignSystem& designSystem,
            SP<SomeTexture> trackTexture, SP<SomeTexture> thumbTexture, SliderConfig config
        );

        This& SliderVertical(SliderConfig config);

    public:
        VectorList<std::reference_wrapper<WorldNode>> nodes;

        QuickBuilder(WorldNode& node) {
            nodes.push_back(node);
        }

        WorldNode& Node() const {
            return nodes[nodes.size() - 1];
        }

        SP<WorldNode> NodeShared() const {
            return SCAST<WorldNode>(Node().shared_from_this());
        }

        /// Modify all components of this type
        This& ModifyNode(std::function<void(WorldNode& node)> func) {
            GUARDR(func, *this)

            func(Node());
            return *this;
        }

        /// Modify all components of this type
        template <class T>
        This& ModifyAll(std::function<void(T& component, size_t index)> func) {
            GUARDR(func, *this)

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
            GUARDR(func, *this)

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
            GUARDR(func, *this)

            for (auto i = components.crbegin(); i != components.crend(); i++) {
                func(**i);
                break;
            };

            return *this;
        }

        This& AndPrefab(String id);

        This& Repeat(int count, std::function<void(This&)> func);

        struct OrthoStandardConfig {
            Color clearColor = Color::white;
            bool isImGuiEnabled = true;
        };

        /// Add standard orthographic camera and associated components
        This& OrthoStandard(
            OrthoStandardConfig config = { .clearColor = Color::white, .isImGuiEnabled = true }
        );

        /// Scales a node with a fixed size to keep it proportional to the size of the window
        This& ScaleWithWindow(Vector3 worldSize, float ratio = 1.0f);

        /// Keeps the latest component sized to the window as it resizes (it must be world sizable)
        This& SizeWithWindow(Vector2 ratio = Vector2::one);

        /// Add texture render component
        This& Texture(String id);

        /// Add sprite render component
        This& Sprite(String id) {
            return Texture(id);
        }

        /// Add circle render component
        This& Circle(float radius, Color color = Color::gray);

        /// Adds rect render component
        This& Rect(Vector2 size, Color color);

        /// Adds rect frame render component
        This& RectFrame(Vector2 size, Color color = Color::black, float strokeWidth = 2);

        /// Adds rect render component
        This& Square(float size, Color color) {
            return Rect({ size, size }, color);
        }

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
        This& RectCollider(Vector2 size = { 1, 1 });

        /// Add a circle collider
        This& CircleCollider(float radius);

        /// Add a slider control and thumb
        This& Slider(SliderConfig config);

        /// Rename the current node
        This& Named(String name) {
            Node().SetName(name);
            return *this;
        }

        /// Starts a build view chain using view builders
        /// Each view is given a view builder via the build func and can build the hierarchy using
        /// additional view builders
        This& RootView(Vector2 size, ViewBuilder::BuildViewFunc buildFunc);

        /// Sets uniform scale for the current node
        This& SetUniformScale(float value) {
            Node().transform.SetScale({ value, value, value });
            return *this;
        }

        /// Sets uniform scale for the current node
        This& SetUniformScale2D(float value) {
            Node().transform.SetScale({ value, value, 1 });
            return *this;
        }

        /// Sets uniform scale for the current node
        This& SetScale(Vector3 value) {
            Node().transform.SetScale(value);
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

        constexpr This& Pop(int count = 1) {
            while (count > 0) {
                // Can't pop the first node
                GUARDR(nodes.size() > 1, *this)
                nodes.erase(nodes.begin() + (nodes.size() - 1));
                count--;
            }
            return *this;
        }

        constexpr This& PopToName(String name) {
            while (nodes.size() > 1) {
                GUARDR(Node().Name() != name, *this)
                Pop();
            }
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

        template <class T, typename... Arguments>
        constexpr This& WithIfNeeded(Arguments... args) {
            bool wasNeeded{};
            auto& component = Node().AddComponentIfNeededWasNeeded<T>(wasNeeded);

            if (wasNeeded) {
                components.push_back(&component);
            }
            return *this;
        }

        template <class T, typename... Arguments>
        constexpr This& WithId(String id, Arguments... args) {
            auto& component = Node().AddComponent<T>(args...);
            component.SetId(id);
            components.push_back(&component);
            return *this;
        }

        /// Adds a component that performs an action when enabled
        This& WithOnEnable(String id, std::function<void(WorldComponent<>&)> func);

        template <class Type>
        constexpr This& RemoveType() {
            Node().RemoveType<Type>();
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

        /// Adds a behavior tree node builder func for a character controller state
        This& CharacterStateBehavior(String state, CharacterController::BuildBehaviorFunc func);

        /// Sets the character state in the character controller, if one exists
        This& SetCharacterState(String value);

        /// Adds cycle hue effect
        This& CycleHueEffect(
            String id = {}, SwitchState switchState = SwitchState::On, float cycleTime = 1
        );

        /// Sets the animate duration for subsequent animations
        This& SetAnimateDuration(float value);

        /// Sets the animate ease func for subsequent animations
        This& SetAnimateEase(EaseFunc value);

        /// Sets the animate delay for subsequent animations
        This& SetAnimateDelay(float value);

        /// Sets the frame rate for upcoming frame animations
        This& SetFrameRate(float value) {
            frameRate = value;
            return *this;
        }

        /// Sets the animation cycle type for upcoming frame animations
        This& SetCycleType(AnimationCycleType value) {
            cycleType = value;
            return *this;
        }

        /// Add a sprite animation based on the texture names
        This& AnimateSprite(VectorList<String> const& textureNames);

        /// Set the animation state in the animation controller, if one exists
        This& SetAnimationState(String value);

        /// Set the build renderer func for this animation state in the animation controller
        This& AnimationStateRenderer(
            String state, SimpleAnimationController::BuildRendererFunc buildRendererFunc
        );

        /// Add an animated texture renderer builder func for this animation state based on the
        /// keyframe models
        This& AnimationStateFrames(
            String state, VectorList<AnimatedSpriteRenderer::KeyframeModel> const& models
        );
        /// Add state transitions for the current animation controller
        This& AnimationStateTransitions(VectorList<StateTransitionModel> const& transitions);

        /// Send an input to the animation controller
        This& AnimationInput(String value);

        /// Set the components state to enable/disable specific components
        This& SetComponentsState(String value);

        /// Set the func for this components state in the components controller
        This& ComponentsState(String state, UnorderedSet<String> const& ids);

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
            node.updatables.AddDelay(std::max(0.0f, AnimateState().delay), std::move(animator));
            return *this;
        }

        template <class Type>
        This&
        AnimateCycle(Type startValue, Type endValue, MakeAnimatorFunc<Type, WorldNode&> maker) {
            auto& node = Node();
            auto animator = maker(startValue, endValue, node);
            node.updatables.AddDelay(std::max(0.0f, AnimateState().delay), std::move(animator));
            return *this;
        }

        // MARK: imGui

        using PaintImmediateNodeHandler = PJ::NodeHandler<PJ::RenderProcessor::Phase>;
        using PaintImmediateFunc = std::function<void()>;

        /// Attach an imGui window to this node, with a paint func
        This& ImGui(StringView title, PaintImmediateFunc paintFunc);

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

        /// Add rotate animation in 2D space with start and end values
        This& AnimateRotate(Angle startValue, Angle endValue);

        /// Add rotate animation in 2D space from start value to 1
        This& RotateIn(Angle startValue);

        /// Add rotate animation in 2D space from current to end value
        This& RotateTo(Angle endValue);

        /// Add continuous rotate animation in 2D space (angles per second)
        This& RotateContinue(Angle value);

        /// Rotate to an end value in 2D space and continue in an animation cycle
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

        /// Rotate in 2D space at the specified angles per second speed
        This& Turn(Angle speed);

        /// Destroy the current node in N seconds
        This& Destroy(float time);

        /// Destroy the current node after animation is finished
        This& DestroyAfterAnimate();
    };

    using QB = QuickBuilder;

} // namespace PJ
