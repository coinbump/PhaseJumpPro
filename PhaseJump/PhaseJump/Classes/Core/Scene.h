#pragma once

#include "SomeLoadResourcesOperation.h"
#include "WorldComponent.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 8/17/24
 */
namespace PJ {
    class World;

    /**
     Scenes load themselves into their parent node on start
     */
    class Scene : public WorldComponent<> {
    protected:
        bool isLoaded = false;

    public:
        // FUTURE: develop a file format, or parse Unity/Godot scenes
        using Base = WorldComponent<>;
        using LoadFunc = std::function<void(WorldNode& root)>;

        LoadFunc loadFunc;

        virtual ~Scene() {}

        bool IsLoaded() const {
            return isLoaded;
        }

        virtual void LoadInto(WorldNode& root) {
            GUARD(loadFunc)
            loadFunc(root);
        }

        // MARK: WorldComponent

        void Start() override;
    };
} // namespace PJ
