#pragma once

namespace PJ {
    namespace ResourceType {
        /// Image texture
        auto constexpr Texture = "texture";

        /// Image loaded as an audio stream
        auto constexpr AudioStream = "audio.stream";

        /// Shader program (combined vertex + fragment shader)
        auto constexpr ShaderProgram = "shader.program";

        /// Texture atlas created by RTexPacker (by Raylib)
        auto constexpr RTexPackerAtlas = "rtexPacker.atlas";

        /// Texture atlas created by TexturePacker (by CodeAndWeb)
        auto constexpr TexturePackerAtlas = "texturePacker.atlas";

        /// Image loaded as a 32-bit RGBA Bitmap
        auto constexpr Bitmap = "bitmap";

        /// Font created by BMFont (by AngelCode)
        auto constexpr BitmapFont = "font.bitmap";

        /// Resource map
        auto constexpr Rez = "rez";
    } // namespace ResourceType
} // namespace PJ
