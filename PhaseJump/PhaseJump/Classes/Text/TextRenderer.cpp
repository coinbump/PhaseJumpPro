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
    font(font),
    text(text),
    size(size) {

    GUARD(font && font->atlas && font->atlas->Textures().size() > 0)

    material = MAKE<RenderMaterial>();
    auto program = SomeShaderProgram::registry["texture.vary"];
    material->Add(font->atlas->Textures()[0]);
    material->SetShaderProgram(program);
    material->EnableFeature(RenderFeature::Blend, true);
}

void TextRenderer::SetColor(Color color) {
    this->color = color;
}

void TextRenderer::OnColorChange() {
    GUARD(!IsEmpty(mesh.vertices))

    VectorList<RenderColor> colors(mesh.vertices.size(), color);

    if (modifyColorsFunc) {
        modifyColorsFunc(*this, colors);
    }

    this->colors = colors;
}

void TextRenderer::OnTextChange() {
    GUARD(font)

    auto atlas = font->atlas;
    GUARD(atlas)

    auto& atlasTextures = atlas->Textures();
    GUARD(!IsEmpty(atlasTextures))

    Mesh mesh;
    renderChars.clear();

    TextMeasurer tm(*font);
    auto lines = tm.MeasureLines(text, size, lineClip);

    for (auto& line : lines) {
        float lineX = 0;
        if (lineAlignFunc) {
            lineX = lineAlignFunc(size.x, line.size.x);
        }

        /// Character positions are in view coordinates
        Vector2 viewPos(lineX, line.y);

        auto sourceIndex = line.sourceIndex;

        // FUTURE: support Unicode
        for (int i = 0; i < line.text.size(); i++, sourceIndex++) {
            auto u32 = ToU32String(line.text);
            auto charString = U32CharToString(u32[i]);

            QuadMeshBuilder qm;
            auto const& fontGlyphI = font->glyphs.find(charString);
            GUARD_CONTINUE(fontGlyphI != font->glyphs.end());

            auto& fontGlyph = fontGlyphI->second;
            auto& atlasTexture = fontGlyph.texture;
            GUARD_CONTINUE(atlasTexture)

            auto textureSize = atlasTexture->Size();
            qm.worldSize = Vector3(textureSize.x, textureSize.y, 0);

            auto quadMesh = qm.BuildMesh();

            auto textureModel = atlasTexture->MakeRenderModel();
            GUARD_CONTINUE(textureModel)

            quadMesh.OffsetBy(Vector2(
                (qm.worldSize.x / 2.0f) * Vector2::right.x,
                (qm.worldSize.y / 2.0f) * Vector2::down.y
            ));

            quadMesh.OffsetBy(Vector2(
                (size.x / 2.0f) * Vector2::left.x +
                    (viewPos.x + fontGlyph.offset.x) * Vector2::right.x,
                (size.y / 2.0f) * Vector2::up.y + (viewPos.y + fontGlyph.offset.y) * Vector2::down.y
            ));

            UVTransformFuncs::textureCoordinates(*textureModel, quadMesh.UVs());

            RenderChar renderChar;
            renderChar.sourceIndex = sourceIndex;
            renderChar.vertexIndex = mesh.vertices.size();
            renderChars.push_back(renderChar);

            mesh += quadMesh;

            viewPos.x += fontGlyph.advanceX;
        }
    }

    if (modifyVerticesFunc) {
        modifyVerticesFunc(*this, mesh.vertices);
    }

    this->mesh = mesh;
    OnColorChange();
}

VectorList<RenderModel> TextRenderer::MakeRenderModels(RenderContextModel const& model) {
    VectorList<RenderModel> result;

    // TODO: optmize this. Only build when needed

    GUARDR(font, result)

    auto atlas = font->atlas;
    GUARDR(atlas, result)

    auto& atlasTextures = atlas->Textures();
    GUARDR(!IsEmpty(atlasTextures), result)

    RenderModelBuilder builder;
    VectorList<SomeTexture*> textures{ atlasTextures[0].get() };
    auto renderModel = builder.Build(
        mesh, *material, textures, ModelMatrix(), owner->transform.WorldPosition().z,
        UVTransformFunc()
    );

    renderModel.colors = this->colors;

    result.push_back(renderModel);
    return result;
}
