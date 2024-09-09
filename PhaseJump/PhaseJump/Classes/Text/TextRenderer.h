#pragma once

#include "RenderTypes.h"
#include "SomeAligner.h"
#include "SomeRenderer.h"
#include "TextMeasurer.h"

// CODE REVIEW: ?/23
namespace PJ {
    class Font;

    /**
     Renders text (no markup), requires a font and material to use
     */
    class TextRenderer : public SomeRenderer {
    public:
        using Base = SomeRenderer;

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
        String text;
        Color color = Color::white;
        bool isBuildNeeded = true;

        LineClip lineClip = LineClip::Partial;

        Mesh mesh;
        VectorList<RenderColor> colors;
        VectorList<RenderChar> renderChars;

        ModifyColorsFunc modifyColorsFunc;
        ModifyVerticesFunc modifyVerticesFunc;

        std::optional<TextMetrics> metrics;

        // FUTURE: support locale-aware leading/trailing alignment
        AlignFunc lineAlignFunc = AlignFuncs::center;
        AlignFunc textAlignFunc = AlignFuncs::center;

        virtual void OnTextChange();
        virtual void OnColorChange();

    public:
        /// Optional. Sets the max # of characters to be rendered, for reveal-type animations
        // FUTURE: std::optional<size_t> revealCount;

        TextRenderer(SP<Font> font, String text, Vector2 size);

        Vector2 Size() const {
            return size;
        }

        TextRenderer& SizeToFit();

        /// Builds the mesh and metrics if needed
        void BuildIfNeeded();

        VectorList<RenderChar> const& RenderChars() const {
            return renderChars;
        }

        TextRenderer& SetModifyColorsFunc(ModifyColorsFunc value) {
            modifyColorsFunc = value;
            OnColorChange();
            return *this;
        }

        TextRenderer& SetModifyVerticesFunc(ModifyVerticesFunc value) {
            modifyVerticesFunc = value;
            OnTextChange();
            return *this;
        }

        TextRenderer& SetLineAlignFunc(AlignFunc value) {
            lineAlignFunc = value;
            OnTextChange();
            return *this;
        }

        TextRenderer& SetTextAlignFunc(AlignFunc value) {
            textAlignFunc = value;
            OnTextChange();
            return *this;
        }

        TextRenderer& SetFont(SP<Font> value) {
            GUARDR(font != value, *this)
            font = value;
            OnTextChange();
            return *this;
        }

        TextRenderer& SetLineClip(LineClip value) {
            GUARDR(lineClip != value, *this)
            lineClip = value;
            OnTextChange();
            return *this;
        }

        TextRenderer& SetSize(Vector2 value) {
            GUARDR(size != value, *this)
            size = value;
            OnTextChange();
            return *this;
        }

        String Text() const {
            return text;
        }

        TextRenderer& SetText(StringView value) {
            GUARDR(text != value, *this)
            text = value;
            OnTextChange();

            return *this;
        }

        TextRenderer& SetTextColor(Color color) {
            SetColor(color);
            return *this;
        }

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "TextRenderer";
        }

        // MARK: WorldComponent

        void Awake() override {
            Base::Awake();

            OnTextChange();
        }

        // MARK: SomeRenderer

        VectorList<RenderModel> MakeRenderModels() override;
        void SetColor(Color color) override;

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
