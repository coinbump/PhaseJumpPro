#include "TextRenderer.h"
#include "Font.h"
#include "QuadMeshBuilder.h"
#include "RenderFeature.h"
#include "RenderMaterial.h"
#include "RenderModelBuilder.h"
#include "SomeShaderProgram.h"
#include "TextMeasurer.h"
#include "TextureAtlas.h"
#include "UIPlanner.h"

using namespace std;
using namespace PJ;

TextRenderer::TextRenderer(Config config) :
    Base(config.worldSize),
    font(config.font),
    text(config.text) {

    GUARD(font && font->atlas && font->atlas->Textures().size() > 0)

    model.material =
        MAKE<RenderMaterial>(RenderMaterial::Config{ .shaderId = ShaderId::TextureVary });
    model.material->Add(font->atlas->texture);
    model.material->EnableFeature(RenderFeature::Blend, true);

    auto defaultFunc = model.BuildVertexColorsFunc();

    model.SetBuildMeshFunc([this](auto& model) { return BuildMesh(); });

    model.SetBuildVertexColorsFunc([=, this](auto& model, auto& colors) {
        defaultFunc(model, colors);

        if (metrics) {
            int colorIndex = 0;

            for (auto& line : metrics->lines) {
                for (auto& c : line.charMetrics) {
                    auto& attributes = c.attributes;

                    auto colorAttributes = Filter(attributes, [](auto& part) {
                        return part.type == TextPartType::AttributeColor;
                    });

                    if (!IsEmpty(colorAttributes)) {
                        Color color = colorAttributes[0].color;
                        colors[colorIndex] = color;
                        colors[colorIndex + 1] = color;
                        colors[colorIndex + 2] = color;
                        colors[colorIndex + 3] = color;
                    }

                    colorIndex += 4;
                }
            }
        }

        GUARD(this->modifyColorsFunc)
        this->modifyColorsFunc(*this, colors);
    });

    PlanUIFunc planUIFunc = [this](auto& component, String context, UIPlanner& planner) {
        planner.InputText({ .label = "Text",
                            .binding = { [this]() { return text.PlainText(); },
                                         [this](auto& value) { SetText(value); } } });
    };
    Override(planUIFuncs[UIContextId::Inspector], planUIFunc);
}

void TextRenderer::OnTextChange() {
    model.SetMeshNeedsBuild();
}

Mesh TextRenderer::BuildMesh() {
    GUARDR(font, {})

    auto atlas = font->atlas;
    GUARDR(atlas, {})

    auto& atlasTextures = atlas->Textures();
    GUARDR(!IsEmpty(atlasTextures), {})

    Mesh mesh;
    renderChars.clear();

    TextMeasurer tm(*font);
    Vector2 worldSize2 = WorldSize();
    auto metrics = tm.Measure(text, worldSize2, lineClip);
    this->metrics = metrics;

    auto& lines = metrics.lines;

    auto size = metrics.CalculateSize();
    float textHeight = size.y;

    for (auto& line : lines) {
        float lineX = 0;
        if (lineAlignFunc) {
            lineX = lineAlignFunc(size.x, line.size.x);
        }

        /// Character positions are in view coordinates
        Vector2 viewPos(lineX, line.y);

        auto sourceIndex = line.sourceIndex;

        auto const& charMetrics = line.charMetrics;

        // FUTURE: support Unicode
        for (int i = 0; i < charMetrics.size(); i++, sourceIndex++) {
            auto& cm = charMetrics[i];
            auto u32 = ToU32String(cm.text);
            uint32_t u32Char = u32[0];

            QuadMeshBuilder qm;
            auto const& fontGlyphI = font->glyphs.find(u32Char);
            GUARD_CONTINUE(fontGlyphI != font->glyphs.end());

            auto& fontGlyph = fontGlyphI->second;
            auto& atlasTexture = fontGlyph.texture;
            GUARD_CONTINUE(atlasTexture)

            auto textureSize = atlasTexture->Size();
            qm.worldSize = Vector3(textureSize.x, textureSize.y, 0);

            auto quadMesh = qm.BuildMesh();

            quadMesh.Offset(
                Vector2((qm.worldSize.x / 2.0f) * vecRight, (qm.worldSize.y / 2.0f) * vecDown)
            );

            quadMesh.Offset(Vector2(
                (size.x / 2.0f) * vecLeft + (viewPos.x + fontGlyph.offset.x) * vecRight,
                (size.y / 2.0f) * vecUp + (viewPos.y + fontGlyph.offset.y) * vecDown
            ));

            UVTransformFuncs::textureCoordinates(*atlasTexture, quadMesh.UVs());

            RenderChar renderChar;
            renderChar.sourceIndex = sourceIndex;
            renderChar.vertexIndex = mesh.Vertices().size();
            renderChars.push_back(renderChar);

            mesh += quadMesh;

            viewPos.x += cm.advanceX;
        }
    }

    if (textAlignFunc) {
        mesh.Offset(Vector3(0, textAlignFunc(size.y, textHeight) * vecDown, 0));
    }

    if (modifyVerticesFunc) {
        modifyVerticesFunc(*this, mesh.Vertices());
        mesh.SetNeedsCalculate();
    }

    return mesh;
}

TextRenderer& TextRenderer::SizeToFit(Vector2 proposal) {
    SetWorldSize(CalculateSize(proposal));
    return *this;
}

Vector2 TextRenderer::CalculateSize(Vector2 proposal) {
    TextMeasurer tm(*font);
    auto metrics = tm.Measure(Text(), proposal, GetLineClip());
    auto calculatedSize = metrics.CalculateSize(metrics.lines);
    return calculatedSize;
}
