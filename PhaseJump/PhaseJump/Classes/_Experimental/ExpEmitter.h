
#pragma once

#include "AtlasTexture.h"
#include "EmitModel.h"
#include "Emitter.h"
#include "LimiterDriver.h"
#include "List.h"
#include "MatrixBoard.h"
#include "MatrixPiece.h"
#include "Pool.h"
#include "PublishedValue.h"
#include "QuadMeshBuilder.h"
#include "RateLimiter.h"
#include "Runner.h"
#include "Scene.h"
#include "SequenceCounter.h"
#include "SomeCamera.h"
#include "SomeMeshBuilder.h"
#include "SomeRenderEngine.h"
#include "SomeRenderer.h"
#include "SomeTexture.h"
#include "StateCore.h"
#include "StateMachine.h"
#include "Timer.h"
#include "UIControl2D.h"
#include "UnorderedSet.h"
#include "View2D.h"
#include "WorldComponent.h"
#include "WorldNode.h"

// Experimental
namespace PJ {
#if FALSE.
    SomeRenderer

        void
        DuckUI::BuildButtonFunc(ButtonConfig config) {
        auto color = Color("color.interactive") auto pressedColor =
            Color("color.pressed")

                vb.Button(.buildFrameFunc = [=](auto& vb) {
                    vb.Immediate({. renderViewFunc = [=](auto& view, auto& renderer)) {
                        auto& button = *(static_cast<ButtonControl*>(&view));
                        
                        renderer.SetColor(button.IsPressed() ? pressedColor : color);
                        renderer.FillRect(view.bounds);
                        renderer.SetFontSpec(config.fontSpec);
                        
                        renderer.SetColor(button.IsPressed() ? pressedLabelColor : labelColor);
                        renderer.Text(config.label, { x, y });
                    }
    })
                      .SetFixedSize({}, 20);
});
}

void DuckUI::BuildButtonFunc(ButtonConfig config) {
    auto color = Color("color.interactive") auto pressedColor =
        Color("color.pressed")

            vb.Button(.buildFrameFunc = [=](auto& vb) {
                vb.Immediate([=](auto& view, auto& renderer)) {
                    auto& button = *(static_cast<ButtonControl*>(&view));

                    renderer.SetColor(button.IsPressed() ? pressedColor : color);
                    renderer.FillRect(view.bounds);
                    renderer.SetFontSpec(config.fontSpec);

                    renderer.SetColor(button.IsPressed() ? pressedLabelColor : labelColor);
                    renderer.Text(config.label, { x, y });
                })
                      .SetFixedSize({}, 20);
            });
}

class AnimationTransition : public Updatable {
public:
    float duration = 0;

    bool canInterrupt{};
};

template <class StateType>
class AnimationMachine : public Updatable {
public:
    using Base = Updatable;
    using This = AnimationMachine;

    using Transition = AnimationTransition;

    using AnimationStateType = std::variant<StateType, Transition>;
    StateMachine<AnimationStateType> sm;

    void Input(String input) {
        auto state = sm.State();
        auto transition = std::get_if<Transition>(state);
        if (transition) {}

        auto nextState = sm.NextStateForInput(input);
        GUARD(nextState)

        auto stateType = std::get_if<StateType>(state);
        if (stateType) {}
    }
};
#endif

/// ??? Do we need this?
//    class MenuHandler : public WorldComponent<> {
//    public:
//        SP<Menu> menu = MAKE<Menu>;
//    };

namespace TextStyleId {
    auto constexpr Bold = "bold";
    auto constexpr Italic = "italic";
    auto constexpr Underline = "underline";
    auto constexpr Strikethrough = "strike";
    auto constexpr Caps = "caps";
} // namespace TextStyleId

/// Rich text is composed of multiple text parts.
/// Each part can be an attribute, or text, or something else
class SomeTextPart {
public:
    size_t index;
};

/// Allows states to be pushed on the stack and
class TextState {
public:
    Color color;
};

/// Image inside rich text
class ImageTextPart : public SomeTextPart {
public:
    Image image;
};

/// String inside rich text
class StringTextPart : public SomeTextPart {
public:
    String value;
};

/// Style attribute inside rich text
class SomeAttributeTextPart : public SomeTextPart {
public:
    enum class Type { Start, End };

    Type type = Type::Start;
};

/// Font attribute inside rich text
class ColorAttributeTextPart : public SomeAttributeTextPart {
public:
    Color color;

    ColorAttributeTextPart(Color color) :
        color(color) {}
};

/// Style attribute inside rich text
class StyleAttributeTextPart : public SomeAttributeTextPart {
public:
    UnorderedSet<String> styles;
};

/// Font attribute inside rich text
class FontAttributeTextPart : public SomeAttributeTextPart {
public:
    String name;
    float size = 12;

    FontAttributeTextPart(String name, float size) :
        name(name),
        size(size) {}
};

/// Style attribute inside rich text
class StyleAttributeTextPart : public SomeAttributeTextPart {
public:
    UnorderedSet<String> styles;
};

struct TextStyle {
    UnorderedSet<String> styles;
    String fontName;
    float fontSize = 12;
};

class RichTextMeasurer {};

class RichTextRenderer : public SomeMaterialRenderer {};

class AnimateApp : public Scene {};

class ImageApp : public Scene {};

// EXTRA: boards, mine sweeper

//    class SomeTextLayout {};
//
//    struct TextChar {
//        String value;
//        size_t index = 0;
//    };
//
//    class StandardTextLayout : public SomeTextLayout {
//        VectorList<Vector3> BuildLayout(String string) {
//            VectorList<Vector3> result;
//            return result;
//        }
//    };
//
//    struct TextLayoutModel {
//        struct Glyph {
//            TextChar textChar;
//            Vector2 pos;
//        };
//
//        VectorList<Glyph> glyphs;
//    };
//
//    class TextRenderModel {
//        Mesh mesh;
//        VectorList<Color> colors;
//    };
//

/// Responsible for loading scenes
class SceneRepository {};

/// Responsible for loading tile maps
class TileMapRepository {};

class PaintBrush {};

class DelaunayBuilder {};

// https://docs.unity3d.com/Packages/com.unity.postprocessing@3.0/manual/Quick-start.html
// https://github.com/godotengine/godot/pull/80214
class RenderPostProcessor {};

class ShaderPostRenderOperation {};

class GrainPostRenderEffect {
    /// In order for this to work we need to always render into texture buffer
    /// Render texture buffer again into next texture buffer with effect shader
    /// Simple but needs some refactoring
};

class VignettePostRenderEffect {};

class ColorGradePostRenderEffect {};
class SomeCamera;

// #define WHATEVER
#ifdef WHATEVER
void foo() {
    // Valve need revamp, it has a list of interpolators
    // When valve state changes it affects interpolators
    // Hover gesture handler has switch func
    // Hover gesture handler is added, with switch func that captures valve and turns valve on
    // or off
    qb.OnHover([](Node& node) {
        return QuickValve(node)
            .ScaleTo(1.2f)

            // ?? Can the animations be reversible somehow? Just reverse the progress
            // What if in its destructor it can reverse the animations? But when would it get
            // destroyed?
            QuickBuild(node)
            .StartValve()
            .ScaleTo(1.2f)
            .EndValve()
            .ValveOn();
    });
}
#endif

// ? Isn't the camera a render system? If we have multiple cameras do we need multiple systems?
class CameraRenderSystem {
    /// Cameras, in render order ?? Can each camera have its own shader/post effects?
    List<SomeCamera> cameras;
};

class CustomShader {};

// This won't work. SwiftUI is Swift code, you'd need an entire Swift-language parser
class SwiftUIRepository {};

class Box2DNode {
public:
    WP<WorldNode> link;
};

class Box2DSystem {
    List<Box2DNode> nodes;

    void OnAdd(List<WorldNode> const& nodes) {}

    void OnRemove(List<WorldNode> const& nodes) {}

    void OnFixedUpdate() {}
};

namespace Example {
    namespace Checkers {
        // BLOCKERS: drop zone
        class Scene : public PJ::Scene {
            void LoadInto(WorldNode& root) override {}
        };
    } // namespace Checkers
} // namespace Example

namespace ExampleTopDown2D {
    // BLOCKERS: CannonEmitter
    class Scene : public PJ::Scene {
        void LoadInto(WorldNode& root) override {}
    };
} // namespace ExampleTopDown2D

namespace ExampleLogo {
    // BLOCKERS: Intermediate renderer
    class Scene : public PJ::Scene {
        // Tap in scene, produce random physics object
        void LoadInto(WorldNode& root) override {}
    };
} // namespace ExampleLogo

namespace ExampleTowerDefense {
    // NO BLOCKERS
    class Scene : public PJ::Scene {
        // Tap in scene, produce random physics object
        void LoadInto(WorldNode& root) override {}
    };
} // namespace ExampleTowerDefense

namespace ExampleMatrixRunner {
    // BLOCKERS: MatrixView2D? <- do we actually need that?
    class Scene : public PJ::Scene {
        // Tap in scene, produce random physics object
        void LoadInto(WorldNode& root) override {}
    };
} // namespace ExampleMatrixRunner

namespace ExampleAudioPad {
    // NO BLOCKERS
    class Scene : public PJ::Scene {
        // Tap in scene, produce random physics object
        void LoadInto(WorldNode& root) override {}
    };
} // namespace ExampleAudioPad

namespace ExampleAnimate {
    // BLOCKERS: UI
    class Scene : public PJ::Scene {
        // Tap in scene, produce random physics object
        void LoadInto(WorldNode& root) override {}
    };
} // namespace ExampleAnimate

namespace ExampleDragAndDrop {
    // Blockers: drop logic + drag uni ttests
    class Scene : public PJ::Scene {
        // Tap in scene, produce random physics object
        void LoadInto(WorldNode& root) override {}
    };
} // namespace ExampleDragAndDrop

namespace ExampleGoals {
    // Blockers: drop zone
    class Scene : public PJ::Scene {
        // Tap in scene, produce random physics object
        void LoadInto(WorldNode& root) override {}
    };
} // namespace ExampleGoals

namespace ExampleVST {
    // BLOCKERS: UI, Audio/MIDI?
    class Scene : public PJ::Scene {
        // Tap in scene, produce random physics object
        void LoadInto(WorldNode& root) override {}
    };
} // namespace ExampleVST

namespace ExampleAnimators {
    // NO BLOCKERS
    class Scene : public PJ::Scene {
        // Tap in scene, produce random physics object
        void LoadInto(WorldNode& root) override {}
    };
} // namespace ExampleAnimators

namespace ExampleUI {
    // BLOCKERS: UI
    class Scene : public PJ::Scene {
        // Tap in scene, produce random physics object
        void LoadInto(WorldNode& root) override {}
    };
} // namespace ExampleUI

namespace ExampleWaveformRenderer {
    // BLOCKERS: painters model?
    class Scene : public PJ::Scene {
        // Tap in scene, produce random physics object
        void LoadInto(WorldNode& root) override {}
    };
} // namespace ExampleWaveformRenderer

namespace ExampleAnimatedBarGraphs {
    // NO BLOCKERS
    class Scene : public PJ::Scene {
        // Tap in scene, produce random physics object
        void LoadInto(WorldNode& root) override {}
    };
} // namespace ExampleAnimatedBarGraphs

namespace ExampleImageAnimation {
    // NO BLOCKERS
    class Scene : public PJ::Scene {
        // Tap in scene, produce random physics object
        void LoadInto(WorldNode& root) override {}
    };
} // namespace ExampleImageAnimation

namespace ExamplePaintCanvas {
    // BLOCKER: UI, Bitmap/Buffer conversions, bitmap operations
    class Scene : public PJ::Scene {
        // Tap in scene, produce random physics object
        void LoadInto(WorldNode& root) override {}
    };
} // namespace ExamplePaintCanvas

namespace ExampleObjectCanvas {
    // BLOCKER: UI
    class Scene : public PJ::Scene {
        // Tap in scene, produce random physics object
        void LoadInto(WorldNode& root) override {}
    };
} // namespace ExampleObjectCanvas

namespace ExampleTimelineCanvas {
    // BLOCKER: UI
    class Scene : public PJ::Scene {
        // Tap in scene, produce random physics object
        void LoadInto(WorldNode& root) override {}
    };
} // namespace ExampleTimelineCanvas

namespace ExampleBreakout {
    // BLOCKER: Box2D + Collisions
    class Scene : public PJ::Scene {
        // Tap in scene, produce random physics object
        void LoadInto(WorldNode& root) override {}
    };
} // namespace ExampleBreakout

namespace ExampleTopDownShooter {
    // BLOCKER: Cannon Emitter
    class Scene : public PJ::Scene {
        // Tap in scene, produce random physics object
        void LoadInto(WorldNode& root) override {}
    };
} // namespace ExampleTopDownShooter

namespace ExampleSide2D {
    // BLOCKER: Box2D
    class Scene : public PJ::Scene {
        // Tap in scene, produce random physics object
        void LoadInto(WorldNode& root) override {}
    };
} // namespace ExampleSide2D

namespace ExamplePostFX {
    class Scene : public PJ::Scene {
        // Tap in scene, produce random physics object
        void LoadInto(WorldNode& root) override {}
    };
} // namespace ExamplePostFX

namespace ExampleBox2D {
    class Scene : public PJ::Scene {
        // Tap in scene, produce random physics object
        void LoadInto(WorldNode& root) override {}
    };
} // namespace ExampleBox2D

namespace ExampleSpeech {
    class Scene : public PJ::Scene {
        // Tap in scene, produce random physics object
        void LoadInto(WorldNode& root) override {}
    };
} // namespace ExampleSpeech

namespace ExampleSideShooter {
    class Scene : public PJ::Scene {
        // Tap in scene, produce random physics object
        void LoadInto(WorldNode& root) override {}
    };
} // namespace ExampleSideShooter

namespace ExampleMeshLibrary {
    class Scene : public PJ::Scene {
        // Tap in scene, produce random physics object
        void LoadInto(WorldNode& root) override {}
    };
} // namespace ExampleMeshLibrary

namespace ExampleShaderLibrary {
    class Scene : public PJ::Scene {
        // Tap in scene, produce random physics object
        void LoadInto(WorldNode& root) override {}
    };
} // namespace ExampleShaderLibrary

namespace ExampleText {
    class Scene : public PJ::Scene {
        // Tap in scene, produce random physics object
        void LoadInto(WorldNode& root) override {}
    };
} // namespace ExampleText

namespace ExampleBezierCurveEditor {
    class Scene : public PJ::Scene {
        // Tap in scene, produce random physics object
        void LoadInto(WorldNode& root) override {}
    };
} // namespace ExampleBezierCurveEditor

namespace ExampleGuitarTabs {
    class Scene : public PJ::Scene {
        // Tap in scene, produce random physics object
        void LoadInto(WorldNode& root) override {}
    };
} // namespace ExampleGuitarTabs

//    template <class Type>
//    class FastBFSTreeNode {
//    public:
//        std::optional<Type> value;
//        size_t depth = 0;
//        size_t childCount = 0;
//        size_t reservedChildCount = 0;
//        size_t nextSiblingIndex = 0;
//
//        List<FastTreeNode<Type>> children;
//    };
//
//    /// A fast tree, divided into slices for easy BFS iteration
//    /// Order of the children is not guaranteed, useful in situations where we need
//    /// tree relationships, but we don't care about the order of evaluation
//    template <class Type>
//    class FastUnstableBFSTree {
//        using Node = FastTreeNode<Type>;
//
//        List<VectorList<Node>> slices{ VectorList<Node>(), 1 };
//
//        size_t nextOpenIndex = 0;
//
//        void Add(Type value) {
//            slices[0].push_back(value);
//        }
//
//        void AddTo(Node parent, Type value) {
//            slices[parent->depth + 1].insert();
//            nextOpenIndex++;
//        }
//    };

// Example: attach a stencil to a view
class ComposableRenderModifier {};

class ProAnimationStateMachine {};

class ProTimeline {};

/// SDF-signed fonts for accessibility. Problem: need fonts with rounded corners?
class SDFTextRenderer {};

class TextureBuffer {};

class BehaviorGraphNode {};

class Evaluator {};

class ConditionBehavior {};

class RandomSelectorBehavior {};

class SelectorBehavior {};

class SequenceSelector {};

class StateBehavior {};

class TimedBehavior {};

class TimedStateBehavior {};

template <class T>
struct AnimatedValue {
    using This = AnimatedValue<T>;
    using ModifierFunc = std::function<T(AnimatedValue const&, T const&)>;

    T start{};
    T end{};

    ModifierFunc startModifierFunc;
    ModifierFunc endModifierFunc;

    AnimatedValue() {};

    AnimatedValue(T start, T end) :
        start(start),
        end(end) {}

    void SetUniform(T value) {
        start = value;
        end = value;
    }

    //        static This Uniform(T value) {
    //            return This(start, end);
    //        }
};

template <class T>
struct MinMaxValue {
    T min{};
    T max{};

    MinMaxValue(T min, T max) :
        min(min),
        max(max) {}
};

template <class T>
struct VaryingValue {
    T value{};
    T vary{};

    VaryingValue(T value, T vary) :
        value(value),
        vary(vary) {}
};

//    template <class T>
//    struct RandomMinMaxValue {
//        T min{};
//        T max{};
//        T vary{};
//
//        MinMaxValue(T min, T max, T vary) : min(start), max(end), vary(vary) {
//        }
//    };

class ParticlesRenderer2D : public SomeMaterialRenderer {
protected:
    bool didFire{};

public:
    SP<SomeTexture> texture;

    struct Particle : public Poolable {
        AnimatedValue<Color> color;
        AnimatedValue<Vector2> velocity;
        AnimatedValue<Vector2> worldSize;

        float duration{};
        float state{};

        Vector2 position;
        Angle angularVelocity;
        Angle rotation;

        Particle() {
            resetFunc = [](auto& _p) {
                Particle& p = *(static_cast<Particle*>(&_p));

                p.duration = 0;
                p.state = 0;
            };
        }
    };

    bool isOneShot = false;

    /// How much of the emitter's velocity is added to particles
    // FUTURE: float emitterVelocityScale = 1.0f;

    bool useWorldSpace{};

    int maxAlive = 0;

    // can we add paths to this?
    //        ParticlesRenderer2D() {
    //            onUpdateFunc = [](auto& owner, auto time) {
    //                for (size_t i = 0; i < particles.EndIndex(); i++) {
    //                    auto* p = particles[i];
    //                    GUARD(p && p->IsAlive())
    //                    p->OnUpdate(time);
    //                }
    //            };
    //        }

    size_t Count() const;

    void Fire() {
        if (isOneShot) {
            GUARD(!didFire)
        }
        didFire = true;

        // Pick color, velocity, angle, rotation, and angular velocity
        // Add the particle to the pool
        // Every cycle update the particle
    }

    VaryingValue<Color> emitColor;
    VaryingValue<Color> endColor;
    VaryingValue<float> emitVelocity;
    VaryingValue<float> endVelocity;
    VaryingValue<Vector2> emitSize;
    VaryingValue<Vector2> endSize;

    VaryingValue<Vector2> emitPosition;
    VaryingValue<Vector2> emitGravity;
    MinMaxValue<float> emitLifeTime;

    // use VaryingValue here? is there an advantage to either one?
    MinMaxValue<Angle> emitAngle;
    MinMaxValue<Angle> emitRotation;
    MinMaxValue<Angle> emitAngularVelocity;

    // EmitterModel, driver, but SpawnFunc spawns new object in pool, object must have lifetime
    // (Can't be infinite)
};

class SpeechRunner {};

class a2DFXSystem {};

class ResourcePack {};

class TileMap {};

/// Abstracts when and how to move an object (move_and_slide vs just move)
// !! should just be a func, no need to have a class
class SomeMover {};

/// Steers the camera
class SomeCameraSteering : WorldComponent<> {
public:
    /// Focus point of the camera
    WP<WorldNode> target;
};

/// Steers the camera in 2D space
class SomeCameraSteering2D : WorldComponent<> {
public:
    /// Focus point of the camera
    WP<WorldNode> target;
};

// this is incorrect, fix it
using ViewPosition = ScreenPosition;

class DocumentBundle;

class FocusCoordinator {
    // how do we attach metadata to nodes, like left-neighbor? Can only UI controls be
    // focused? SEE: https://docs.godotengine.org/en/stable/tutorials/ui/gui_navigation.html
};

class PaintCanvas : public View2D {};

class ImageCanvas : public View2D {
    // Reference bitmapFilters list
};

class ObjectCanvas : public View2D {};

class PortGraphCanvas : public ObjectCanvas {};

class PortGraphCanvasModel {};

class TimelineCanvas : public ObjectCanvas {};

class CanvasTool {};

class SomePaintTool : public CanvasTool {};

class BrushPaintTool : public SomePaintTool {};

class ErasePaintTool : public SomePaintTool {};

class CanvasToolClass;

class ToolCanvasModel {
    VectorList<CanvasToolClass> toolClasses;
    SP<CanvasTool> activeTool;
};

class TimelineCanvasModel {};

/// An app that allows properties to be animated along the timeline
class TimelineApp : public Scene {};

class Feed : public View2D {};

class Cursor {
public:
    Image image;
    Vector2 hotSpot;

    static void Hide();
    static void UnHide();
};

namespace CursorId {
    /// The native OS cursor
    auto constexpr Native = "native";

    // add from here: https://developer.apple.com/documentation/appkit/nscursor
}; // namespace CursorId

class CursorArea {
public:
    String cursorId;
    // UISystem checks for collision, changes cursor <- the OS cursor or the node cursor?
};

class WindowManager {};

// PlatformWindow and Window are not the same thing
class Window : public View2D {
    // WindowModel
};

namespace ThemeKey {
    auto constexpr ButtonBackgroundColor = "button.background.color";
}

/// Collection of shared UI properties and modifiers
/// Example: button.background.color = "0x442314"
class UIThemeV2 {
public:
    Tags tags;

    using ModifyViewFunc = std::function<void(View2D&)>;

    /// Allows for advanced behavior, where each individual view can be modified
    /// based on the theme
    List<ModifyViewFunc> viewModifiers;
};

/// Whenever a child or deep child is added, applies the theme view modifiers ?? how would it
/// know about deep children though?
class ThemedView : public View2D {};

// FUTURE: class ProjectionCamera : public SomeCamera {};
// FUTURE: class MetalRenderEngine : public SomeRenderEngine {};

} // namespace PJ

#if FALSE
struct QuickView {
    This& SetOrigin(Vector2 value);

    This& SetSize(Vector2 value);
    This& AndHStack();
    This& AndVStack();
    This& AndZStack();
    This& AndGrid();
    This& Pad(leading, trailing, top, bottom) < -Adds parent pad view
};

// Experimental
namespace PJ {
#if FALSE

    class ViewSizeModel {
    public:
        Vector2 size;
    };

#endif

    //    class View2D : public WorldComponent<>, public WorldSizeable {
    //    protected:
    //        // UP<ViewLayout> layout;
    //
    //    public:
    //        //    void SetLayout(UP<ViewLayout>& value) {
    //        //        layout = std::move(value);
    //        //
    //        //        // apply layout
    //        //    }
    //
    //
    //    };

    // FUTURE: support UI constraints resolver if needed
    // FUTURE: class ConstraintsResolverViewLayout: public SomeViewLayout
#endif
