#include "File.h"

using namespace std;
using namespace PJ;

void File::Open() {
    GUARD_LOG(path.string().size() > 0, "ERROR. Missing file path");

    Open(path);
}

void File::Open(FilePath path) {
    GUARD(!IsOpen())
    this->path = path;
    file.open(path);
}

void File::Close() {
    GUARD(IsOpen())
    file.close();
}

bool File::IsOpen() const {
    return file.is_open();
}
