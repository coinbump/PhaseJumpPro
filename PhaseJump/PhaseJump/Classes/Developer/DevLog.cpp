#include "DevLog.h"

using namespace std;
using namespace PJ;

PJ::DevLog PJ::devLog;

DevLog::DevLog() {
    history.limit = 100;
}

void DevLog::Add(String value) {
    std::lock_guard<std::mutex> guard(mutex);
    history.Add(value);

    if (onAddFunc) {
        onAddFunc(value);
    }
}
