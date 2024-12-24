#pragma once

#include "ClassRegistry.h"
#include "SomeLoadResourcesOperation.h"
#include "TypeClass.h"
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

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "Scene";
        }
    };

    /// Used to register a type of scene that can be instantiated as needed
    class SceneClass : public TypeClass<Scene> {
    public:
        SceneClass(String id, String name, FactoryFunc factoryFunc) :
            TypeClass<Scene>(id, factoryFunc) {
            core.name = name;
        }
    };

    using SceneClassRegistry = ClassRegistry<SceneClass>;
} // namespace PJ
