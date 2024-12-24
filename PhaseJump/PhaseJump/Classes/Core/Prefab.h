#pragma once

#include <functional>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/16/24
 */
namespace PJ {
    class WorldNode;

    /// A prefab is a node factory. We can use prefabs to quickly build scenes
    class Prefab {
    public:
        using LoadFunc = std::function<void(WorldNode&)>;

        String id;
        LoadFunc loadFunc;

        Prefab() {}

        Prefab(LoadFunc loadFunc) :
            loadFunc(loadFunc) {}

        virtual ~Prefab() {}

        virtual void LoadInto(WorldNode& node) {
            GUARD(loadFunc)
            loadFunc(node);
        }
    };
} // namespace PJ
