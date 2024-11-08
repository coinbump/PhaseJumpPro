#include "QuickBuild.h"
#include "AnimatedSpriteRenderer.h"
#include "AnimateFuncs.h"
#include "Colliders2D.h"
#include "ColorRenderer.h"
#include "ComponentsController.h"
#include "CycleHueEffect.h"
#include "DesignSystem.h"
#include "DragHandler2D.h"
#include "DropFilesUIEvent.h"
#include "EllipseMeshBuilder.h"
#include "FramePlayable.h"
#include "Funcs.h"
#include "GridMeshBuilder.h"
#include "Matrix4x4.h"
#include "OrthoCamera.h"
#include "QuadFrameMeshBuilder.h"
#include "RenderFeature.h"
#include "RotateKSteering2D.h"
#include "SimpleAnimationController.h"
#include "SimpleRaycaster2D.h"
#include "SlicedTextureRenderer.h"
#include "SliderControl.h"
#include "SomeCollider2D.h"
#include "SomeHoverGestureHandler.h"
#include "SpriteRenderer.h"
#include "Theme.h"
#include "World.h"

using namespace std;
using namespace PJ;

using This = QuickBuild::This;

This& QuickBuild::WithOnEnable(String id, std::function<void(WorldComponent<>&)> func) {
    return With<WorldComponent<>>().ModifyLatestAny([=](auto& component) {
        // Disabled by default, so enable func runs
        component.Enable(false);

        component.id = id;
        component.onEnabledChangeFunc = [=](SomeWorldComponent& component) {
            GUARD(component.IsEnabled())
            func(*(static_cast<WorldComponent<>*>(&component)));
        };
    });
}

This& QuickBuild::Repeat(int count, std::function<void(This&)> func) {
    PJ::Repeat(count, [&]() { func(*this); });

    return *this;
}

This& QuickBuild::OrthoStandard(Color clearColor) {
    With<OrthoCamera>().With<SimpleRaycaster2D>().ModifyLatest<OrthoCamera>([=](auto& camera) {
        camera.clearColor = clearColor;
    });

    return *this;
}

This& QuickBuild::Texture(String id) {
    return With<SpriteRenderer>(Node().World()->FindTexture(id));
}

This& QuickBuild::Circle(float radius, Color color) {
    return With<ColorRenderer>(color, Vector2(radius * 2, radius * 2))
        .ModifyLatest<ColorRenderer>([](auto& renderer) {
            renderer.SetBuildMeshFunc([](RendererModel const& model) {
                return EllipseMeshBuilder(model.WorldSize()).BuildMesh();
            });
        });
}

This& QuickBuild::RectFrame(Vector2 size, Color color, float strokeWidth) {
    return With<ColorRenderer>(color, size).ModifyLatest<ColorRenderer>([=](auto& renderer) {
        renderer.SetBuildMeshFunc([=](RendererModel const& model) {
            return QuadFrameMeshBuilder(model.WorldSize(), { strokeWidth, strokeWidth })
                .BuildMesh();
        });
    });
}

This& QuickBuild::SquareFrame(float size, Color color, float strokeWidth) {
    return RectFrame({ size, size }, color, strokeWidth);
}

This& QuickBuild::Grid(Vector2 worldSize, Vec2I gridSize, Color color, float strokeWidth) {
    return With<ColorRenderer>(color, worldSize).ModifyLatest<ColorRenderer>([=](auto& renderer) {
        renderer.SetBuildMeshFunc([=](RendererModel const& model) {
            return GridMeshBuilder(worldSize, gridSize, strokeWidth).BuildMesh();
        });
    });
}

This& QuickBuild::Drag(OnDragUpdateFunc onDragUpdateFunc) {
    auto component = &Node().AddComponent<DragHandler2D>().SetOnDragUpdateFunc(onDragUpdateFunc);

    components.push_back(component);

    // Add basic drag collider if it's missing
    if (!Node().TypeComponent<SomeCollider2D>()) {
        auto worldSizeable = Node().TypeComponent<WorldSizeable>();
        if (worldSizeable) {
            auto worldSize = worldSizeable->WorldSize();
            return RectCollider((Vector2)worldSize);
        }
    }

    return *this;
}

This& QuickBuild::DragSnapBack(OnDragUpdateFunc onDragUpdateFunc) {
    Drag(onDragUpdateFunc).ModifyLatest<DragHandler2D>([](auto& dragHandler) {
        dragHandler.InOnDropSnapBack();
    });

    return *this;
}

This& QuickBuild::OnDropFiles(OnDropFilesFunc onDropFilesFunc) {
    With<WorldComponent<>>("Drop files")
        .ModifyLatestAny([onDropFilesFunc](SomeWorldComponent& component) {
            component.signalFuncs[SignalId::DropFiles] =
                [onDropFilesFunc](auto& component, auto& signal) {
                    auto& dropFilesUIEvent = *(static_cast<DropFilesUIEvent const*>(&signal));
                    onDropFilesFunc({ component, dropFilesUIEvent });
                };
        });

    return *this;
}

This& QuickBuild::SquareCollider(float size) {
    return With<PolygonCollider2D>().ModifyLatest<PolygonCollider2D>([=](auto& collider) {
        collider.poly = Polygon::MakeSquare(size);
    });
}

This& QuickBuild::RectCollider(Vector2 size) {
    return With<PolygonCollider2D>().ModifyLatest<PolygonCollider2D>([=](auto& collider) {
        collider.poly = Polygon::MakeRect(size);
    });
}

This& QuickBuild::CircleCollider(float radius) {
    return With<CircleCollider2D>(radius);
}

void QuickBuild::AddSlider(
    World& world, WorldNode& parent, DesignSystem& designSystem, SP<SomeTexture> trackTexture,
    SP<SomeTexture> thumbTexture, String name, Vector2 worldSize, Axis2D axis,
    std::function<void(float)> valueFunc
) {
    GUARD(trackTexture && thumbTexture)

    SlicedTextureRenderer::SlicePoints slicePoints =
        designSystem.TagValue<SlicedTextureRenderer::SlicePoints>(
            UIElement::SliderTrack, UITag::SlicePoints
        );

    auto& sliderNode = parent.AddNode("Slider track");

    sliderNode.AddComponent<SlicedTextureRenderer>(trackTexture, vec2Zero, slicePoints);
    auto& sliderControl = sliderNode.AddComponent<SliderControl>(axis);
    components.push_back(&sliderControl);

    float endCapSize = designSystem.TagValue<float>(UIElement::SliderTrack, UITag::EndCapSize);
    sliderControl.SetEndCapSize(endCapSize).SetFrame(Rect({ 0, 0 }, worldSize));
    auto& thumbNode = sliderNode.AddNode("Slider thumb");

    thumbNode.AddComponent<PolygonCollider2D>().SetPoly(Polygon::MakeRect(thumbTexture->size));

    thumbNode.AddComponent<SpriteRenderer>(thumbTexture);

    if (valueFunc) {
        // Subscribe to value and store subscription
        // Careful: watch out for strong reference cycles when using smart pointers
        sliderNode.cancellables.insert(sliderControl.Value().Receive(valueFunc));
    }
}

This& QuickBuild::Slider(String name, Vector2 worldSize, std::function<void(float)> valueFunc) {
    if (worldSize.y > worldSize.x) {
        return SliderVertical(name, worldSize, valueFunc);
    }

    auto world = Node().World();
    GUARDR(world, *this)

    auto designSystem = world->designSystem;
    GUARDR_LOG(designSystem, *this, "Missing design system")

    auto trackTexture = designSystem->Texture(UIElement::SliderTrack);
    GUARDR_LOG(trackTexture, *this, "Missing slider track texture")

    auto thumbTexture = designSystem->Texture(UIElement::SliderThumb);
    GUARDR_LOG(thumbTexture, *this, "Missing slider thumb texture")

    AddSlider(
        *world, Node(), *designSystem, trackTexture, thumbTexture, name, worldSize, Axis2D::X,
        valueFunc
    );
    return *this;
}

This& QuickBuild::SliderVertical(
    String name, Vector2 worldSize, std::function<void(float)> valueFunc
) {
    auto world = Node().World();
    GUARDR(world, *this)

    auto designSystem = world->designSystem;
    GUARDR_LOG(designSystem, *this, "Missing design system")

    // FUTURE: support UV rotation so we don't have to duplicate textures
    auto trackTexture = designSystem->Texture(UIElement::SliderVerticalTrack);
    GUARDR_LOG(trackTexture, *this, "Missing slider track texture")

    auto thumbTexture = designSystem->Texture(UIElement::SliderVerticalThumb);
    GUARDR_LOG(thumbTexture, *this, "Missing slider thumb texture")

    AddSlider(
        *world, Node(), *designSystem, trackTexture, thumbTexture, name, worldSize, Axis2D::Y,
        valueFunc
    );
    return *this;
}

This& QuickBuild::AndPrefab(String id) {
    auto world = Node().World();
    GUARDR(world, *this)

    auto prefabNode = MAKE<WorldNode>();

    Node().Add(prefabNode);
    world->LoadPrefab(id, *prefabNode);
    Push(*prefabNode);

    return *this;
}

// MARK: - Animate

This& QuickBuild::SetAnimateDuration(float value) {
    AnimateState().duration = value;
    return *this;
}

This& QuickBuild::SetAnimateEase(EaseFunc value) {
    AnimateState().easeFunc = value;
    return *this;
}

This& QuickBuild::SetAnimateDelay(float value) {
    AnimateState().delay = value;
    return *this;
}

This& QuickBuild::SetValveDurations(float turnOnDuration, float turnOffDuration) {
    AnimateState().valveTurnOnDuration = turnOnDuration;
    AnimateState().valveTurnOffDuration = turnOffDuration;

    return *this;
}

This& QuickBuild::HoverScaleTo(float endValue) {
    float startValue = Node().transform.Scale().x;

    return With<ValveHoverGestureHandler>(
               AnimateState().valveTurnOnDuration, AnimateState().valveTurnOffDuration
    )
        .ModifyLatest<ValveHoverGestureHandler>([=](auto& handler) {
            // Match the interpolation curve in both directions
            auto binding = AnimateFuncs::MakeUniformScaleBinding(
                *handler.owner, startValue, endValue, EaseFuncs::outSquared
            );
            auto reverseBinding = AnimateFuncs::MakeUniformScaleBinding(
                *handler.owner, endValue, startValue, EaseFuncs::outSquared
            );

            handler.valve->SetOnValveUpdateFunc([=](auto& valve) {
                switch (valve.State()) {
                case Valve::StateType::TurnOff:
                    reverseBinding(1.0f - valve.Value());
                    break;
                default:
                    binding(valve.Value());
                    break;
                }
            });
        });
}

This& QuickBuild::HoverScaleToPingPong(float endValue) {
    auto& animateState = AnimateState();

    HoverScaleTo(endValue).ModifyLatest<ValveHoverGestureHandler>([&](auto& handler) {
        ValveHoverGestureHandler* handlerPtr = &handler;
        Valve::OnValveUpdateFunc overrideFunc = [=](auto& valve) {
            if (handlerPtr->IsHovering()) {
                switch (valve.State()) {
                case Valve::StateType::Off:
                    valve.TurnOn(animateState.valveTurnOnDuration);
                    break;
                case Valve::StateType::On:
                    valve.TurnOff(animateState.valveTurnOffDuration);
                    break;
                default:
                    break;
                }
            }
        };

        handlerPtr->valve->SetOnValveUpdateFunc(
            OverrideFunc(handlerPtr->valve->GetOnValveUpdateFunc(), overrideFunc)
        );
    });

    return *this;
}

This& QuickBuild::AnimateMove(Vector3 startValue, Vector3 endValue) {
    return AnimateStartEnd<Vector3>(
        startValue, endValue,
        AnimateFuncs::PositionMaker(AnimateState().duration, AnimateState().easeFunc)
    );
}

/// Add a position animation with a start value
This& QuickBuild::MoveIn(Vector3 startValue) {
    return AnimateMove(startValue, Node().transform.LocalPosition());
}

/// Add a position animation with an end value
This& QuickBuild::MoveTo(Vector3 endValue) {
    return AnimateMove(Node().transform.LocalPosition(), endValue);
}

This& QuickBuild::AnimateScale(float startValue, float endValue) {
    return AnimateStartEnd<float>(
        startValue, endValue,
        AnimateFuncs::UniformScaleMaker(AnimateState().duration, AnimateState().easeFunc)
    );
}

This& QuickBuild::ScaleIn(float startValue) {
    return AnimateScale(startValue, 1);
}

This& QuickBuild::ScaleTo(float endValue) {
    return AnimateScale(Node().transform.Scale().x, endValue);
}

This& QuickBuild::ScaleToPingPong(float endValue) {
    // TODO: Need reverse ease func
    MakeAnimatorFunc<float, WorldNode&> maker = AnimateFuncs::UniformScaleMaker(
        AnimateState().duration, AnimateState().easeFunc, AnimationCycleType::PingPong
    );

    return AnimateCycle(Node().transform.Scale().x, endValue, maker);
}

This& QuickBuild::AnimateRotate(Angle startValue, Angle endValue) {
    return AnimateStartEnd<float>(
        startValue.Degrees(), endValue.Degrees(),
        AnimateFuncs::RotateMaker(AnimateState().duration, AnimateState().easeFunc)
    );
}

This& QuickBuild::RotateTo(Angle endValue) {
    return AnimateRotate(Angle::DegreesAngle(-Node().transform.Rotation().z), endValue);
}

This& QuickBuild::RotateToPingPong(Angle endValue) {
    // TODO: Need reverse ease func
    MakeAnimatorFunc<float, WorldNode&> maker = AnimateFuncs::RotateMaker(
        AnimateState().duration, AnimateState().easeFunc, AnimationCycleType::PingPong
    );

    return AnimateCycle(-Node().transform.Rotation().z, endValue.Degrees(), maker);
}

This& QuickBuild::RotateIn(Angle startValue) {
    return AnimateRotate(startValue, Angle::DegreesAngle(0));
}

This& QuickBuild::RotateContinue(Angle value) {
    auto& node = Node();

    node.updatables.AddContinue([=, &node](auto time) {
        Angle angle = Angle::DegreesAngle(-node.transform.Rotation().z);
        auto deltaDegrees = value.Degrees() * time.delta;
        node.transform.SetRotation(Angle::DegreesAngle(angle.Degrees() + deltaDegrees));
    });

    return *this;
}

This& QuickBuild::CycleHueEffect(String id, SwitchState switchState, float cycleTime) {
    return WithId<PJ::CycleHueEffect>(id, switchState, cycleTime);
}

This& QuickBuild::AnimateOpacity(float startValue, float endValue) {
    return AnimateStartEnd<float>(
        startValue, endValue,
        AnimateFuncs::OpacityMaker(AnimateState().duration, AnimateState().easeFunc)
    );
}

This& QuickBuild::OpacityIn(float startValue) {
    return AnimateOpacity(startValue, 1);
}

This& QuickBuild::OpacityOut() {
    return AnimateOpacity(Node().Opacity(), 0);
}

This& QuickBuild::OpacityTo(float endValue) {
    return AnimateOpacity(Node().Opacity(), endValue);
}

This& QuickBuild::TurnRight(Angle speed) {
    return With<RotateKSteering2D>(speed).ModifyLatest<RotateKSteering2D>([=](auto& component) {
        component.TurnRight(speed);
    });
}

This& QuickBuild::Destroy(float time) {
    Node().Destroy(time);
    return *this;
}

This& QuickBuild::DestroyAfterAnimate() {
    return Destroy(AnimateState().duration);
}

This& QuickBuild::PushAnimateSettings() {
    animateStates.push_back(AnimateState());
    return *this;
}

This& QuickBuild::PopAnimateSettings() {
    if (animateStates.size() > 1) {
        animateStates.erase(animateStates.begin() + (animateStates.size() - 1));
    }
    return *this;
}

This& QuickBuild::AnimateSprite(VectorList<String> const& textureNames) {
    using KeyframeModel = AnimatedSpriteRenderer::KeyframeModel;

    VectorList<KeyframeModel> keyframeModels =
        Map<KeyframeModel>(textureNames, [](auto& textureName) {
            return KeyframeModel{ .textureId = textureName };
        });
    RemoveType<AnimatedSpriteRenderer>().With<AnimatedSpriteRenderer>(
        keyframeModels, frameRate, cycleType
    );

    return *this;
}

This& QuickBuild::SetAnimationState(String value) {
    auto simpleAnimationController = Node().TypeComponent<SimpleAnimationController>();
    GUARDR_LOG(simpleAnimationController, *this, "ERROR. Missing animation controller");

    simpleAnimationController->SetState(value);

    return *this;
}

This& QuickBuild::CharacterStateBehavior(
    String state, CharacterController::BuildBehaviorFunc func
) {
    return WithIfNeeded<CharacterController>().ModifyLatest<CharacterController>(
        [&](auto& controller) { controller.AddBuildBehaviorFunc(state, func); }
    );
}

This& QuickBuild::SetCharacterState(String value) {
    auto characterController = Node().TypeComponent<CharacterController>();
    GUARDR_LOG(characterController, *this, "ERROR. Missing character controller");

    characterController->states.SetState(value);

    return *this;
}

This& QuickBuild::AnimationStateRenderer(
    String state, SimpleAnimationController::BuildRendererFunc buildRendererFunc
) {
    auto simpleAnimationController = Node().TypeComponent<SimpleAnimationController>();
    GUARDR_LOG(simpleAnimationController, *this, "ERROR. Missing animation controller");

    simpleAnimationController->Add(state, buildRendererFunc);

    return *this;
}

This& QuickBuild::AnimationStateFrames(
    String state, VectorList<AnimatedSpriteRenderer::KeyframeModel> const& models
) {
    auto frameRate = this->frameRate;
    auto cycleType = this->cycleType;
    SimpleAnimationController::BuildRendererFunc buildRendererFunc = [=](auto& controller) {
        auto result = MAKE<AnimatedSpriteRenderer>(models, frameRate, cycleType);
        return result;
    };
    return AnimationStateRenderer(state, buildRendererFunc);
}

This& QuickBuild::SetComponentsState(String value) {
    auto componentsController = Node().TypeComponent<ComponentsController>();
    GUARDR_LOG(componentsController, *this, "ERROR. Missing components controller");

    componentsController->states.SetState(value);
    return *this;
}

This& QuickBuild::ComponentsState(String state, UnorderedSet<String> const& ids) {
    return WithIfNeeded<ComponentsController>().ModifyLatest<ComponentsController>(
        [&](auto& controller) { controller.AddEnableComponentsById(state, ids); }
    );
}

This& QuickBuild::AnimationStateTransitions(VectorList<StateTransitionModel> const& transitions) {
    auto simpleAnimationController = Node().TypeComponent<SimpleAnimationController>();
    GUARDR_LOG(simpleAnimationController, *this, "ERROR. Missing animation controller");

    auto& states = simpleAnimationController->states;

    for (auto& transition : transitions) {
        if (transition.state) {
            states.AddTransition(*transition.state, transition.input, transition.toState);
        } else {
            states.anyStateTransitions[transition.input] = transition.toState;
        }
    }

    return *this;
}

This& QuickBuild::AnimationInput(String value) {
    auto simpleAnimationController = Node().TypeComponent<SimpleAnimationController>();
    GUARDR_LOG(simpleAnimationController, *this, "ERROR. Missing animation controller");

    simpleAnimationController->states.Input(value);

    return *this;
}
