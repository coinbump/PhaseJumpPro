#include "SomeWorldSystem.h"
#include "OrderedSet.h"
#include "PropertyIdBuilder.h"
#include "SomeKeyUIEvent.h"
#include <json.hpp>

using namespace std;
using namespace PJ;
using namespace nlohmann;

using This = SomeWorldSystem;

SomeWorldSystem::SomeWorldSystem(String name) :
    _core(*this) {
    _core.name = name;

    AddSignalHandler<KeyDownUIEvent>(
        { .id = SignalId::KeyDown,
          .func =
              [this](auto& system, auto& event) {
                  std::for_each(keyFuncs.begin(), keyFuncs.end(), [&](auto& keyFuncI) {
                      auto& key = keyFuncI.first;
                      auto& keyFunc = keyFuncI.second;

                      try {
                          json j;
                          std::stringstream ss;
                          ss << key;
                          ss >> j;

                          int keyCode = j["keyCode"];
                          GUARD(event.keyCode.value == keyCode);

                          auto eventModifiers =
                              Filter(event.keyModifiers.modifiers, [](auto& modifier) {
#ifdef _WIN32
                                  return modifier == KeyModifier::Shift ||
                                         modifier == KeyModifier::Alt ||
                                         modifier == KeyModifier::Shortcut ||
                                         modifier == KeyModifier::GUI;
#else
                        return modifier == KeyModifier::Shift || modifier == KeyModifier::Control || modifier == KeyModifier::Alt || modifier == KeyModifier::Shortcut;
#endif
                              });

                          auto modifiers = j["modifiers"];
                          OrderedSet<String> eventModifierSet(
                              eventModifiers.begin(), eventModifiers.end()
                          );
                          OrderedSet<String> modifierSet(modifiers.begin(), modifiers.end());

                          GUARD(eventModifierSet == modifierSet)
                          keyFunc(*this);
                      } catch (...) {}
                  });
              } }
    );
}

void SomeWorldSystem::AddKeyFunc(int keyCode, VectorList<String> modifiers, Func func) {
    PropertyIdBuilder builder;
    builder.Add("keyCode", keyCode);
    builder.AddCollection("modifiers", modifiers);
    auto id = builder.Result();

    keyFuncs.insert_or_assign(id, func);
}
