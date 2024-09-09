#pragma once

#include "Color.h"
#include "Macros.h"
#include "RenderTypes.h"
#include "SomeTexture.h"
#include "Tags.h"
#include "UnorderedMap.h"
#include "Vector2.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 7/21/23
 */
// TODO: needs unit tests
namespace PJ {
    class SomeShaderProgram;

    /// Render properties for object
    class RenderMaterial {
    protected:
        SP<SomeShaderProgram> shaderProgram;

        /// Texture inputs to shader
        VectorList<SP<SomeTexture>> textures;

        /// Uniform (non-interpolated) color inputs to shader
        VectorList<Color> uniformColors;

        /// Uniform (non-interpolated) float inputs to shader
        VectorList<float> uniformFloats;

        /// Render features enabled or disabled for this material
        UnorderedMap<String, RenderFeatureState> features;

        /// Cached propertyId that identifies the properties of this material, to determine if it
        /// can be combined with another render material. This also allows us to group propertyIds
        /// as keys in a map
        String propertyId;

        void OnChange() {
            propertyId = BuildPropertyId();
        }

    public:
        String PropertyId() const {
            return propertyId;
        }

        void SetShaderProgram(SP<SomeShaderProgram> program) {
            GUARD(program && shaderProgram != program);
            shaderProgram = program;

            OnChange();
        }

        SP<SomeShaderProgram> ShaderProgram() const {
            return shaderProgram;
        }

        /// Warning: Avoid using uniforms. They can't be batch-rendered
        VectorList<Color> const& UniformColors() const {
            return uniformColors;
        }

        void AddUniformColor(Color color) {
            PJ::Add(uniformColors, color);

            OnChange();
        }

        void SetUniformColor(size_t index, Color color) {
            GUARD(index < uniformColors.size());
            uniformColors[index] = color;

            OnChange();
        }

        /// Warning: Avoid using uniforms. They can't be batch-rendered
        VectorList<float> const& UniformFloats() const {
            return uniformFloats;
        }

        UnorderedMap<String, RenderFeatureState> const& Features() const {
            return features;
        }

        VectorList<SP<SomeTexture>> const& Textures() const {
            return textures;
        }

        /// Add the texture's render texture to the material
        void Add(SP<SomeTexture> texture) {
            GUARD(texture)

            auto renderTexture = texture->RenderTexture();
            GUARD(renderTexture)

            // Material tracks the actual texture being used for renders,
            // not the child texture object (used in texture atlas)
            // Renderers should store the child texture separately from the material
            PJ::Add(textures, renderTexture);

            OnChange();
        }

        /// Set the texture if one exists, or add it if missing (only used for single-texture
        /// materials)
        void Set(SP<SomeTexture> texture) {
            GUARD(texture)

            if (IsEmpty(textures)) {
                Add(texture);
            } else {
                auto renderTexture = texture->RenderTexture();
                GUARD(renderTexture)

                textures[0] = renderTexture;
                OnChange();
            }
        }

        void EnableFeature(String feature, bool isEnabled) {
            features[feature] =
                isEnabled ? RenderFeatureState::Enable : RenderFeatureState::Disable;

            OnChange();
        }

        /// A unique string that determines if this material can be rendered with the same draw call
        /// as others
        String BuildPropertyId() const;

        bool operator==(RenderMaterial const& rhs) const;
    };
} // namespace PJ
