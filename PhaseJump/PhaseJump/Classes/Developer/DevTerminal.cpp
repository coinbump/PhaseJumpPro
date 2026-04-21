#include "DevTerminal.h"
#include "Dev.h"

using namespace std;
using namespace PJ;

PJ::DevTerminal PJ::devTerminal;

DevTerminal::DevTerminal() {
    runCommandFunc = [](DevTerminal& terminal, String const& command) { Log(command); };
}

void DevTerminal::RunCommand(String command) {
    std::lock_guard<std::mutex> guard(mutex);

    if (runCommandFunc) {
        runCommandFunc(*this, command);
    }

    history.Add(command);
}
