#include "Menu.h"
#include "List.h"

using namespace std;
using namespace PJ;

String KeyboardShortcut::ToString() {
    // FUTURE: support localization if needed
    // FUTURE: support ⌘, ⇧, ⌥, ⌃ (not supported by imGui)
    GUARDR(!IsEmpty(modifiers), "")

    String modifierString;
    List<String> modifierStrings;
    std::for_each(modifiers.begin(), modifiers.end(), [&](auto& modifier) {
        if (modifier == EventModifier::CommandControl) {
#ifdef _WIN32
            modifierStrings.push_back("Ctrl");
#else
            modifierStrings.push_back("Cmd");
#endif
        } else if (modifier == EventModifier::Control) {
            modifierStrings.push_back("Ctrl");
        } else if (modifier == EventModifier::OptionAlt) {
#ifdef _WIN32
            modifierStrings.push_back("Alt");
#else
            modifierStrings.push_back("Opt");
#endif
        } else if (modifier == EventModifier::Shift) {
            modifierStrings.push_back("Shift");
        }
    });
    modifierString = Joined(modifierStrings, "-");
    auto result = Joined({ modifierString, ToUpper(value) }, "+");
    return result;
}
