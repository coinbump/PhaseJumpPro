#pragma once

#include "AlignFunc.h"
#include "AttributedString.h"
#include "Font.h"
#include "RenderTypes.h"
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
    class TextRenderer : public SomeMaterialRenderer {
    public:
        using Base = SomeMaterialRenderer;
        using This = TextRenderer;

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
        /// Font for text render
        SP<Font> font;

        /// Text to render
        AttributedString text;

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
        struct Config {
            SP<Font> font;
            AttributedString text;
            Vector2 worldSize;
        };

        /// Optional. Sets the max # of characters to be rendered, for reveal-type animations
        // FUTURE: std::optional<size_t> revealCount;

        TextRenderer(Config config);

        SP<Font> GetFont() const {
            return font;
        }

        TextRenderer& SizeToFit(Vector2 proposal);

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

        TextMetrics Metrics() {
            model.BuildIfNeeded();
            return metrics ? *metrics : TextMetrics{};
        }

        LineClip GetLineClip() const {
            return lineClip;
        }

        TextRenderer& SetLineClip(LineClip value) {
            GUARDR(lineClip != value, *this)
            lineClip = value;
            OnTextChange();
            return *this;
        }

        Vector2 Size() const {
            return WorldSize();
        }

        TextRenderer& SetSize(Vector2 value) {
            SetWorldSize(value);
            return *this;
        }

        /// @return Returns the attributed text value for this renderer
        AttributedString Text() const {
            return text;
        }

        /// @return Returns the plain text value for the attributed string
        String PlainText() const {
            return text.PlainText();
        }

        TextRenderer& SetPlainText(String value) {
            return SetText(value);
        }

        TextRenderer& SetText(AttributedString value) {
            // TODO: add equality to attributed string
            // TODO: GUARDR(text != value, *this)
            text = value;
            OnTextChange();

            return *this;
        }

        TextRenderer& SetTextColor(Color color) {
            SetColor(color);
            return *this;
        }

        // MARK: SomeRenderer

        Vector2 CalculateSize(Vector2 proposal) override;

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "TextRenderer";
        }

        // MARK: WorldComponent

        void Awake() override {
            Base::Awake();

            OnTextChange();
        }
    };
} // namespace PJ
