#pragma once

namespace PJ {
    /// A prefab is a node factory. We can use prefabs to quickly build scenes
    class Prefab {
    public:
        using MakeFunc = std::function<SP<WorldNode>(World&)>;

        String id;
        MakeFunc makeFunc;

        Prefab() {}

        Prefab(MakeFunc makeFunc) :
            makeFunc(makeFunc) {}

        virtual ~Prefab() {}

        virtual SP<WorldNode> Make(World& world) {
            GUARDR(makeFunc, nullptr)
            return makeFunc(world);
        }
    };
} // namespace PJ
