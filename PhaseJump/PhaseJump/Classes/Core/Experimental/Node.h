#ifndef PJNODE_H_
#define PJNODE_H_

#include <vector>

namespace PJ {
    class Component {
    };

    typedef std::shared_ptr<Component> ComponentSharedPtr;

    // A node
    class Node {
    public:
        std::vector<ComponentSharedPtr> components;

        void AddComponent(Component* component) {
            auto shared = ComponentSharedPtr(component);
            components.push_back(shared);
        }
    };
}

#endif
