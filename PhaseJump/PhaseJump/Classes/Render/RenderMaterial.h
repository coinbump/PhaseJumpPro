#pragma once

#include "Color.h"
#include "Macros.h"
#include "RenderFeature.h"
#include "RenderTypes.h"
#include "SomeTexture.h"
#include "Tags.h"
#include "UnorderedMap.h"
#include "Vector2.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/21/24
 */
namespace PJ {
    class SomeShaderProgram;

    /// Render properties for object
    class RenderMaterial {
    public:
        using FeatureStateMap = UnorderedMap<String, RenderFeatureState>;

    protected:
        SP<SomeShaderProgram> shaderProgram;

        /// Texture inputs to shader
        VectorList<SP<SomeTexture>> textures;

        /// Uniform (non-interpolated) color inputs to shader
        VectorList<Color> uniformColors;

        /// Uniform (non-interpolated) float inputs to shader
        VectorList<float> uniformFloats;

        /// Render features enabled or disabled for this material
        FeatureStateMap features;

        /// Cached propertyId that identifies the properties of this material, to determine if it
        /// can be combined with another render material. This also allows us to group propertyIds
        /// as keys in a map
        String propertyId;

        void OnChange() {
            propertyId = BuildPropertyId();
        }

    public:
        struct Config {
            SP<SomeTexture> texture;

            String shaderId;
            FeatureStateMap features = { { RenderFeature::Blend, RenderFeatureState::Disable } };
        };

        RenderMaterial(Config config);

        RenderMaterial() {}

        String PropertyId() const {
            return propertyId;
        }

        /// Sets the shader program for this material
        void SetShaderProgram(SP<SomeShaderProgram> program);

        /// @return Returns the shader program for this material
        SomeShaderProgram* ShaderProgram() const {
            return shaderProgram.get();
        }

        /// @return Returns the list of uniform colors (Warning: avoid uniforms, they can't be batch
        /// rendered)
        VectorList<Color> const& UniformColors() const {
            return uniformColors;
        }

        /// Adds a uniform color (Warning: avoid uniforms, they can't be batch rendered)
        void AddUniformColor(Color color);

        /// Sets the uniform color (Warning: avoid uniforms, they can't be batch rendered)
        void SetUniformColor(size_t index, Color color);

        /// @return Returns the list of uniform floats. (Warning: avoid uniforms, they can't be
        /// batch rendered)
        VectorList<float> const& UniformFloats() const {
            return uniformFloats;
        }

        UnorderedMap<String, RenderFeatureState> const& Features() const {
            return features;
        }

        VectorList<SP<SomeTexture>> const& Textures() const {
            return textures;
        }

        /// Sets the texture if one exists, or add it if missing (only used for single-texture
        /// materials)
        void SetTexture(SP<SomeTexture> texture);

        /// Adds the texture's render texture to the material
        void Add(SP<SomeTexture> texture);

        /// Enables or disables a feature for this material
        void EnableFeature(String feature, bool isEnabled);

        /// Updates all feature enabled states for this material
        void SetFeatureStates(FeatureStateMap const& value);

        /// @return Returns a unique string that determines if this material can be rendered with
        /// the same draw call as others
        String BuildPropertyId() const;
    };
} // namespace PJ
