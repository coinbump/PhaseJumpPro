#include "RenderMaterial.h"
#include "Dev.h"
#include "PropertyIdBuilder.h"
#include "ResourceTypes.h"
#include "SomeShaderProgram.h"

using namespace std;
using namespace PJ;

RenderMaterial::RenderMaterial(Config config) {
    if (config.texture) {
        SetTexture(config.texture);
    }

    if (!IsEmpty(config.shaderId)) {
        auto shaderProgram = SomeShaderProgram::registry.find(config.shaderId);
        GUARD_LOG(
            shaderProgram != SomeShaderProgram::registry.end(),
            String("ERROR. Missing shader: ") + config.shaderId
        )
        SetShaderProgram(shaderProgram->second);
    }

    SetFeatureStates(config.features);
}

String RenderMaterial::BuildPropertyId() const {
    PropertyIdBuilder builder;
    if (shaderProgram) {
        builder.Add(ResourceType::ShaderProgram, shaderProgram->id);
    }
    builder.AddCollection("uniformColors", uniformColors);
    builder.AddCollection("uniformFloats", uniformFloats);

    List<uint32_t> textureIds;
    std::transform(
        textures.cbegin(), textures.cend(), std::back_inserter(textureIds),
        [](SP<SomeTexture> texture) { return texture->RenderId(); }
    );
    builder.AddCollection("textureIds", textureIds);

    builder.AddMap("features", features);

    // cout << "BuildPropertyId: " << builder.Result() << std::endl;

    return builder.Result();
}

void RenderMaterial::SetShaderProgram(SP<SomeShaderProgram> program) {
    GUARD(program && shaderProgram != program);
    shaderProgram = program;

    OnChange();
}

void RenderMaterial::AddUniformColor(Color color) {
    PJ::Add(uniformColors, color);

    OnChange();
}

void RenderMaterial::SetUniformColor(size_t index, Color color) {
    GUARD(index < uniformColors.size());
    uniformColors[index] = color;

    OnChange();
}

void RenderMaterial::SetTexture(SP<SomeTexture> texture) {
    GUARD(texture)

    if (textures.size() == 1 && textures[0] == texture) {
        return;
    }

    textures = { texture };
    OnChange();
}

void RenderMaterial::Add(SP<SomeTexture> texture) {
    GUARD(texture)
    PJ::Add(textures, texture);

    OnChange();
}

void RenderMaterial::EnableFeature(String feature, bool isEnabled) {
    features[feature] = isEnabled ? RenderFeatureState::Enable : RenderFeatureState::Disable;

    OnChange();
}

void RenderMaterial::SetFeatureStates(FeatureStateMap const& value) {
    features = value;
    OnChange();
}
