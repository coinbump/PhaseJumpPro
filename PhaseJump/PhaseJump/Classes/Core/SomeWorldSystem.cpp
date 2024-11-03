#include "SomeWorldSystem.h"
#include "OrderedSet.h"
#include "PropertyIdBuilder.h"
#include <json.hpp>

using namespace std;
using namespace PJ;
using namespace nlohmann;

SomeWorldSystem::SomeWorldSystem(String name) :
    name(name) {
    signalFuncs[SignalId::KeyDown] = [this](auto& system, auto& signal) {
        auto& event = *(static_cast<KeyDownUIEvent const*>(&signal));

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

                auto eventModifiers = Filter(event.keyModifiers.modifiers, [](auto& modifier) {
#ifdef _WIN32
                    return modifier == KeyModifier::Shift || modifier == KeyModifier::Alt ||
                           modifier == KeyModifier::Shortcut || modifier == KeyModifier::GUI;
#else
                    return modifier == KeyModifier::Shift || modifier == KeyModifier::Control || modifier == KeyModifier::Alt || modifier == KeyModifier::Shortcut;
#endif
                });

                auto modifiers = j["modifiers"];
                OrderedSet<String> eventModifierSet(eventModifiers.begin(), eventModifiers.end());
                OrderedSet<String> modifierSet(modifiers.begin(), modifiers.end());

                GUARD(eventModifierSet == modifierSet)
                keyFunc(*this);
            } catch (...) {}
        });
    };
}

void SomeWorldSystem::AddKeyFunc(int keyCode, VectorList<String> modifiers, Func func) {
    PropertyIdBuilder builder;
    builder.Add("keyCode", keyCode);
    builder.AddCollection("modifiers", modifiers);
    auto id = builder.Result();

    keyFuncs.insert_or_assign(id, func);
}
