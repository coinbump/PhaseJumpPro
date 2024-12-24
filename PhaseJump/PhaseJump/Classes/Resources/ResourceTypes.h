#pragma once

namespace PJ {
    namespace ResourceType {
        /// Image texture
        auto constexpr Texture = "texture";

        /// Shader program (combined vertex + fragment shader)
        auto constexpr ShaderProgram = "shaderProgram";

        /// Texture atlas created by RTexPacker (by Raylib)
        auto constexpr RTexPackerAtlas = "rtexPacker.atlas";

        /// Texture atlas created by TexturePacker (by CodeAndWeb)
        auto constexpr TexturePackerAtlas = "texturePacker.atlas";

        /// Font created by BMFont (by AngelCode)
        auto constexpr BitmapFont = "bitmapFont";

        /// Resource map
        auto constexpr Rez = "rez";
    } // namespace ResourceType
} // namespace PJ
