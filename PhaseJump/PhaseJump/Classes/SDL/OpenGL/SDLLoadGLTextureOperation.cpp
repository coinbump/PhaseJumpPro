#include "SDLLoadGLTextureOperation.h"
#include "Bitmap.h"
#include "GLTexture.h"
#include "SDLFileManager.h"
#include "SDLLoadBitmapOperation.h"
#include "SDLSurface.h"
#include "StringUtils.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_surface.h>

using namespace std;
using namespace PJ;

LoadResourcesOperation::Result SDLLoadGLTextureOperation::_LoadResources() {
    auto path = info.filePath;

    SDLLoadBitmapOperation loadBitmap{ info, repoModel };
    loadBitmap.Run();
    auto rgbaBitmap = loadBitmap.TypeSuccessFirst<RGBABitmap>();
    GUARDR(rgbaBitmap, Failure())

    // FUTURE: we should premultiply our textures, this is slow (need image processor)
    // SDL does not load textures premultiplied, so apply it
    // FUTURE: check shaders to make sure they take into account textures are premultiplied
    for (auto& pixel : rgbaBitmap->Pixels()) {
        Color color(pixel);
        GUARD_CONTINUE(color.a < 1.0f)

        color.r *= color.a;
        color.g *= color.a;
        color.b *= color.a;
        pixel = (RGBAColor)color;
    }

    // OpenGl expects textures to be upside-down
    // TODO: even though we aren't flipping the bitmap, it still works, so something is incorrect

    SDLFileManager fm;
    String id = fm.FileName(path, FileManager::FileNameFormat::NoExtension);

    auto texture = MAKE<GLTexture>(GLTexture::Config{ .id = id, .bitmap = rgbaBitmap.get() });

    ResourceModel loadedResource{ .resource = texture, .info = info };
    Success result;
    result.Add(loadedResource);

    return result;
}
