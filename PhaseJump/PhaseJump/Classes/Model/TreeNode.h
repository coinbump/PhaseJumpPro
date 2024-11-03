#pragma once

#include "Macros.h"
#include <type_traits>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 10/14/24
 */
namespace PJ {
    /// TreeNodes require their owner to be Treeable
    template <class Node>
    class Treeable {
    public:
        virtual ~Treeable() {}

        virtual Node* Parent() const = 0;
        virtual void SetParent(Node* value) = 0;
    };

    /// Tree node with parent and children
    template <class Node, class NodePtr>
    class TreeNode {
    public:
        using This = TreeNode;
        using NodeList = VectorList<NodePtr>;

    protected:
        /// Owner
        Node& owner;

        /// Node parent
        Node* parent{};

        /// Node children
        NodeList children;

    public:
        TreeNode(Node& owner) :
            owner(owner) {}

        Node const& Owner() const {
            return owner;
        }

        Node* Parent() const {
            return parent;
        }

        void SetParent(Node* value) {
            parent = value;
        }

        NodeList const& Children() const {
            return children;
        }

        size_t ChildCount() const {
            return children.size();
        }

        void Add(NodePtr& node) {
            Insert(node, children.size());
        }

        /// Shared pointer specialization of Insert
        SPECIALIZE_IS_SAME(NodePtr, SP<Node>) void Insert(SP<Node>& node, size_t index) {
            GUARD(node)
            GUARD(nullptr == node->Parent())

            index = std::clamp(index, (size_t)0, children.size());

            node->SetParent(&owner);
            children.insert(children.begin() + index, node);
        }

        /// Unique pointer specialization of Insert
        SPECIALIZE_IS_SAME(NodePtr, UP<Node>) void Insert(UP<Node>& node, size_t index) {
            GUARD(node)
            GUARD(nullptr == node->Parent())

            index = std::clamp(index, (size_t)0, children.size());

            node->SetParent(&owner);
            children.insert(children.begin() + index, std::move(node));
        }

        void Remove(Node& node) {
            GUARD(&owner == node.Parent())

            node.SetParent(nullptr);

            RemoveFirstIf(children, [&](NodePtr& child) { return child.get() == &node; });
        }

        void RemoveAllChildren(bool updateParent = true) {
            if (updateParent) {
                for (auto& child : children) {
                    child->SetParent(nullptr);
                }
            }

            children.clear();
        }
    };
} // namespace PJ
