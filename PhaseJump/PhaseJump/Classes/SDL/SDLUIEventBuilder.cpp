#include "SDLUIEventBuilder.h"
#include "FilePath.h"
#include "SomeUIEvent.h"

using namespace std;
using namespace PJ;

VectorList<std::shared_ptr<SomeUIEvent>> SDLUIEventBuilder::BuildUIEvents(SDL_Event sdlEvent) {
    VectorList<std::shared_ptr<SomeUIEvent>> result;

    switch (sdlEvent.type) {
        case SDL_DROPFILE: {
            auto droppedFile = sdlEvent.drop.file;
            auto event = make_shared<DropFileUIEvent>(FilePath(droppedFile));
            result.Add(static_pointer_cast<SomeUIEvent>(event));
            SDL_free(droppedFile);
            break;
        }
    }

    return result;
}
