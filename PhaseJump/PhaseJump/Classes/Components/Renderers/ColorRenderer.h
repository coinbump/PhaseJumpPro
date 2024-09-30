#pragma once

#include "Mesh.h"
#include "RenderTypes.h"
#include "RGBAColor.h"
#include "SomeRenderer.h"
#include "Vector2.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 9/6/24
 */
namespace PJ {
#if PSEUDOCODE
    // TODO: Why do we need this? Just make it all static?
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

    /// Renders a uniform color
    class ColorRenderer : public SomeRenderer {
    public:
#if PSEUDOCODE
        MIRROR_CLASS(ColorRendererClass);
#endif

        using Base = SomeRenderer;
        using This = ColorRenderer;

        ColorRenderer(Color color, Vector2 worldSize);
        ColorRenderer(SP<RenderMaterial> material, Color color, Vector2 worldSize);

        static SP<RenderMaterial> MakeMaterial(RenderOpacityType opacityType);

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "ColorRenderer";
        }
    };
} // namespace PJ
