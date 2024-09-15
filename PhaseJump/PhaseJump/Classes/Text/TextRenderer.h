#pragma once

#include "RenderTypes.h"
#include "SomeAligner.h"
#include "SomeRenderer.h"
#include "TextMeasurer.h"

/*
 RATING: 4 stars
 Tested and works. Missing word wrap
 CODE REVIEW: 9/10/24
 */
namespace PJ {
    class Font;

    /**
     Renders text (no markup), requires a font and material to use
     */
    class TextRenderer : public SomeRenderer {
    public:
        using Base = SomeRenderer;

        using ModifyColorsFunc =
            std::function<void(TextRenderer const&, VectorList<RenderColor>& colors)>;
        using ModifyVerticesFunc =
            std::function<void(TextRenderer const&, VectorList<Vector3>& vertices)>;

        /// Because of whitespace, there isn't a direct 1-to-1 relationship between the text
        /// characters and the mesh vertices. This allows us to establish a relationship
        struct RenderChar {
            size_t sourceIndex = 0;
            size_t vertexIndex = 0;
        };

    protected:
        /// Text area size. Text will be wrapped/clipped inside this
        Vector2 size;

        /// Font for text render
        SP<Font> font;

        /// Text to render
        String text;

        /// Line clip behavior when text measurer reaches the end of the text area
        LineClip lineClip = LineClip::Partial;

        /// Measured information for rendering characters
        VectorList<RenderChar> renderChars;

        /// Custom funcs allow users of this renderer to modify its behavior
        ModifyColorsFunc modifyColorsFunc;
        ModifyVerticesFunc modifyVerticesFunc;

        /// Measured text metrics
        std::optional<TextMetrics> metrics;

        // FUTURE: support locale-aware leading/trailing alignment
        AlignFunc lineAlignFunc = AlignFuncs::center;
        AlignFunc textAlignFunc = AlignFuncs::center;

        /// Called when the text content changes
        virtual void OnTextChange();

    public:
        /// Optional. Sets the max # of characters to be rendered, for reveal-type animations
        // FUTURE: std::optional<size_t> revealCount;

        TextRenderer(SP<Font> font, String text, Vector2 size);

        Vector2 Size() const {
            return size;
        }

        TextRenderer& SizeToFit();

        /// Builds the mesh and metrics if needed
        Mesh BuildMesh();

        VectorList<RenderChar> const& RenderChars() const {
            return renderChars;
        }

        TextRenderer& SetModifyColorsFunc(ModifyColorsFunc value) {
            modifyColorsFunc = value;
            model.SetVertexColorsNeedsBuild();
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
