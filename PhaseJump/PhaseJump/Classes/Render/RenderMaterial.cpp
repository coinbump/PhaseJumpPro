#include "RenderMaterial.h"
#include "Dev.h"
#include "PropertyIdBuilder.h"
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

bool RenderMaterial::operator==(RenderMaterial const& rhs) const {
    GUARDR(textures.size() == rhs.textures.size(), false)
    GUARDR(
        shaderProgram == rhs.shaderProgram && uniformColors == rhs.uniformColors &&
            uniformFloats == rhs.uniformFloats,
        false
    )

    /// We can't compare the texture objects directly because an atlas texture uses sub-textures,
    /// but have a shared renderId
    for (int i = 0; i < textures.size(); i++) {
        auto& left = textures[i];
        auto& right = rhs.textures[i];

        GUARDR(left->RenderId() == right->RenderId(), false)
    }

    return false;
}

String RenderMaterial::BuildPropertyId() const {
    PropertyIdBuilder builder;
    if (shaderProgram) {
        builder.Add("shaderProgram", shaderProgram->id);
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
