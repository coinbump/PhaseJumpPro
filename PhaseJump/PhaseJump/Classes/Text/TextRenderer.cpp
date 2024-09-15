#include "TextRenderer.h"
#include "Font.h"
#include "QuadMeshBuilder.h"
#include "RenderFeature.h"
#include "RenderMaterial.h"
#include "RenderModelBuilder.h"
#include "SomeShaderProgram.h"
#include "TextMeasurer.h"
#include "TextureAtlas.h"

using namespace std;
using namespace PJ;

TextRenderer::TextRenderer(SP<Font> font, String text, Vector2 size) :
    Base(vec2Zero),
    font(font),
    text(text),
    size(size) {

    GUARD(font && font->atlas && font->atlas->Textures().size() > 0)

    model.material = MAKE<RenderMaterial>();
    auto program = SomeShaderProgram::registry["texture.vary"];
    model.material->Add(font->atlas->Textures()[0]);
    model.material->SetShaderProgram(program);
    model.material->EnableFeature(RenderFeature::Blend, true);

    auto defaultFunc = model.BuildVertexColorsFunc();

    model.SetBuildMeshFunc([this](auto& model) { return this->BuildMesh(); });

    model.SetBuildVertexColorsFunc([=, this](auto& model, auto& colors) {
        defaultFunc(model, colors);

        GUARD(this->modifyColorsFunc)
        this->modifyColorsFunc(*this, colors);
    });
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
    metrics = tm.Measure(text, size, lineClip);

    auto& lines = metrics->lines;

    float textHeight = 0;

    for (auto& line : lines) {
        textHeight += line.size.y;

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

            auto textureModel = atlasTexture->MakeRenderModel();
            GUARD_CONTINUE(textureModel)

            quadMesh.OffsetBy(
                Vector2((qm.worldSize.x / 2.0f) * vecRight, (qm.worldSize.y / 2.0f) * vecDown)
            );

            quadMesh.OffsetBy(Vector2(
                (size.x / 2.0f) * vecLeft + (viewPos.x + fontGlyph.offset.x) * vecRight,
                (size.y / 2.0f) * vecUp + (viewPos.y + fontGlyph.offset.y) * vecDown
            ));

            UVTransformFuncs::textureCoordinates(*textureModel, quadMesh.UVs());

            RenderChar renderChar;
            renderChar.sourceIndex = sourceIndex;
            renderChar.vertexIndex = mesh.Vertices().size();
            renderChars.push_back(renderChar);

            mesh += quadMesh;

            viewPos.x += cm.advanceX;
        }
    }

    if (textAlignFunc) {
        mesh.OffsetBy(Vector3(0, textAlignFunc(size.y, textHeight) * vecDown, 0));
    }

    if (modifyVerticesFunc) {
        modifyVerticesFunc(*this, mesh.ModifiableVertices());
        mesh.SetNeedsCalculate();
    }

    return mesh;
}

// TODO: this can't work because the size it's using to measure is the wrong size, it needs to be as
// large as possible (or based on # of lines?)
TextRenderer& TextRenderer::SizeToFit() {
    GUARDR(metrics, *this)

    size = metrics->size;

    return *this;
}
