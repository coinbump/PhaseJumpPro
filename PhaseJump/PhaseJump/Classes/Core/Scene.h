#ifndef PJSCENE_H
#define PJSCENE_H

#include "SomeLoadResourcesOperation.h"

namespace PJ {
    /**
     Scenes can be loaded into the world.
     FUTURE: develop a file format, or parse Unity scenes?
     */
    class Scene {
    public:
        /// First key: resource type, second key: id
        LoadedResourceMap resources;

        virtual ~Scene() {}
    };
}

#endif
