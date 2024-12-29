#pragma once

namespace PJ {
#if PSEUDOCODE
    // PROBLEM: for Tower defense, need collisions, raycasting, and steering

    // Why do we need this? Just make it all static?
    class ColorRendererClass : public SomeRendererClass {
        class ImGuiInspectorBuilder {
        public:
            template <class T>
            void Build(T const& value) {
                Build(T::mirrorClass().MakeClassMirrors(value));
            }

            void Build(ClassMirrors const& mirrors) {
                // Iterate through mirrors
                // Create imGui UI based on mirror type
                // Use binding to update values
                //
            }
        };

        /// Produces an area object for a workspace, based on id
        class WorkspaceFactory {};

        /// A set of defined areas
        class Workspace {
            // TODO: how to define location/spacing of areas?
            // TODO: how to link id with actual area <- factory?
        };

        /// Adds interactive split objects to split a world into areas
        /// Each area contains a single world sizable object (view, renderer, etc.)
        /// Area coordinates are in the view coordinate system (top-left is 0, 0)
        class WorldSplitter {
        public:
            /// Splitters child node stores the interactive splitter nodes
            auto constexpr SplittersNodeId = "areas";

            /// Areas child node stores the areas nodes
            auto constexpr AreasNodeId = "splitters";

            UP<WorldArea> root;

            /// Add interactive splitters for pane objects
            /// FUTURE: use design system
            void BuildSplitters();
        };

        /// Pane inside the world. Used to split up the world into different sections such as
        /// toolbars views, scenes, etc.
        class WorkspaceArea : public Treeable<Workspace> {
            {
            public:
                using This = Workspace;

                TreeNode<This, UP<This>> tree;
            };

            struct ClassMirrors {};

            class SomeClassMirror {
                String id;
                String type;
                String name;

                std::any binding;
            }

            template <class Type>
            class ClassMirror : public SomeClassMirror {

                Binding<Type> binding;
            };

            class SeparatorClassMirror;
            class ButtonClassMirror;

            ClassMirrors& MakeMirrors(ColorRenderer& target) {
                ClassMirrors result;

                // ?? can it auto-add the static property mirrors first? For simpler objects?
                result.AddValue<Color>(
                    "color", "Color", [](auto& t) { t.Color(); }, [](auto& t) { return t.Color(); }
                );
                result.AddSeparator();
                result.AddButton("Apply Layout", [](auto& t) { t.ApplyLayout(); });
                return result + Base::MakeMirrors(target);
            }
        };
#endif

        /// An app tool is presented in a toolbar, usually with an icon
        /// One app tool can be selected at a time
        class SomeAppTool {
        public:
            /// Unique identifier
            String id;

            /// User facing name for display
            String name;

            /// Tip text for hover gesture tool tip
            String tipText;

            /// Icon id
            String iconId;
        };

        /// An app space that accepts tool events
    public
        SomeCanvas{ public: virtual ~SomeCanvas(){} };

    public
        ImageCanvas : public SomeCanvas {
        public:
            /// Source of truth for the image
            SP<Bitmap> bitmap;

            /// Texture to render the image
            SP<SomeTexture> texture;
        };

        /// Tool that operates on a canvas
        class SomeCanvasTool {
        public:
            struct GestureState {
            public:
                Vector2 position;
            };

            void OnGestureStart(SomeCanvas& canvas, GestureState state) {}

            void OnGestureUpdate(SomeCanvas& canvas, GestureState state) {}

            void OnGestureEnd(SomeCanvas& canvas, GestureState state) {}
        };

        template <class CanvasType, class Core = Void>
        public CanvasTool : public SomeCanvasTool {
        public:
            Core core{};

            using CanvasGestureFunc = std::function < void(CanvasType&, GestureState);

            CanvasGestureFunc onGestureStartFunc;
            CanvasGestureFunc onGestureUpdateFunc;
            CanvasGestureFunc onGestureEndFunc;

            void OnGestureStart(SomeCanvas & canvas, GestureState state) override {
                auto canvasPtr = dynamic_cast<CanvasType*>(&canvas);
                GUARD(canvasPtr && onGestureStartFunc);
                onGestureStartFunc(*canvasPtr, state);
            }

            void OnGestureUpdate(SomeCanvas & canvas, GestureState state) override {
                auto canvasPtr = dynamic_cast<CanvasType*>(&canvas);
                GUARD(canvasPtr && onGestureUpdateFunc);
                onGestureUpdateFunc(*canvasPtr, state);
            }

            void OnGestureEnd(SomeCanvas & canvas, GestureState state) override {
                auto canvasPtr = dynamic_cast<CanvasType*>(&canvas);
                GUARD(canvasPtr && onGestureEndFunc);
                onGestureEndFunc(*canvasPtr, state);
            }
        };

        /// Contains a collection of tools for a toolbar
        class AppToolbar {
        public:
            VectorList<UP<SomeAppTool>> tools;
        };
    } // namespace PJ
