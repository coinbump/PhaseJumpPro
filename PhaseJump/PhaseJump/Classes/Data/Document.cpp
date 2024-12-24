#include "Document.h"

using namespace std;
using namespace PJ;

void Document::Load() {
    GUARD(loadFunc)

    auto result = loadFunc(*this);
    GUARD(result.IsSuccess())

    isLoaded = true;

    GUARD(onLoadFunc)
    onLoadFunc(*this);
}

void Document::Save() {
    GUARD(saveFunc)

    auto result = saveFunc(*this);
    GUARD(result.IsSuccess())

    GUARD(onSaveFunc)
    onSaveFunc(*this);
}

void Document::Close() {
    GUARD(onCloseFunc)
    onCloseFunc(*this);
}
