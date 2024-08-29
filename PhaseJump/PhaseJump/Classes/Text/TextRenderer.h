#pragma once

#include "RenderTypes.h"
#include "SomeAligner.h"
#include "SomeRenderer.h"
#include "TextMeasurer.h"

namespace PJ {
    class Font;

    /**
     Renders text (no markup), requires a font and material to use
     */
    class TextRenderer : public SomeRenderer {
    public:
        using ModifyColorsFunc =
            std::function<void(TextRenderer&, VectorList<RenderColor>& colors)>;
        using ModifyVerticesFunc =
            std::function<void(TextRenderer&, VectorList<Vector3>& vertices)>;

        /// Because of whitespace, there isn't a direct 1-to-1 relationship between the text
        /// characters and the mesh vertices. This allows to establish a relationship
        struct RenderChar {
            size_t sourceIndex = 0;
            size_t vertexIndex = 0;
        };

    protected:
        Vector2 size;
        SP<Font> font;
        SP<RenderMaterial> material;
        String text;
        Color color = Color::white;

        LineClip lineClip = LineClip::Partial;

        Mesh mesh;
        VectorList<RenderColor> colors;
        VectorList<RenderChar> renderChars;

        ModifyColorsFunc modifyColorsFunc;
        ModifyVerticesFunc modifyVerticesFunc;

        // FUTURE: support locale-aware leading/trailing alignment
        AlignFunc lineAlignFunc = AlignFuncs::left;

        virtual void OnTextChange();
        virtual void OnColorChange();

    public:
        using Base = SomeRenderer;

        /// Optional. Sets the max # of characters to be rendered, for reveal-type animations
        // FUTURE: std::optional<size_t> revealCount;

        TextRenderer(SP<Font> font, String text, Vector2 size);

        Vector2 Size() const {
            return size;
        }

        VectorList<RenderChar> const& RenderChars() const {
            return renderChars;
        }

        void SetModifyColorsFunc(ModifyColorsFunc value) {
            modifyColorsFunc = value;
            OnColorChange();
        }

        void SetModifyVerticesFunc(ModifyVerticesFunc value) {
            modifyVerticesFunc = value;
            OnTextChange();
        }

        void SetLineAlignFunc(AlignFunc value) {
            lineAlignFunc = value;
            OnTextChange();
        }

        void SetFont(SP<Font> value) {
            GUARD(font != value)
            font = value;
            OnTextChange();
        }

        void SetLineClip(LineClip value) {
            GUARD(lineClip != value)
            lineClip = value;
            OnTextChange();
        }

        void SetSize(Vector2 value) {
            GUARD(size != value)
            size = value;
            OnTextChange();
        }

        String Text() const {
            return text;
        }

        void SetText(StringView value) {
            GUARD(text != value)
            text = value;
            OnTextChange();
        }

        void SetColor(Color color) override;

        // MARK: WorldComponent

        void Awake() override {
            Base::Awake();

            OnTextChange();
        }

        // MARK: SomeRenderer

        VectorList<RenderModel> MakeRenderModels() override;

        // MARK: WorldSizeable

        std::optional<Vector3> WorldSize() const override {
            auto size = Size();
            return Vector3(size.x, size.y, 0);
        }

        void SetWorldSize(Vector3 value) override {
            SetSize(value);
        }
    };
} // namespace PJ
