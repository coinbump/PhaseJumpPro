#pragma once

#include "Macros.h"
#include <algorithm>
#include <concepts>
#include <type_traits>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 10/14/24
 */
namespace PJ {
    /// Types used as the Node parameter of `TreeNode` must expose Parent/SetParent.
    template <class Node>
    concept Treeable = requires(Node& node, Node const& childNode, Node* parentNode) {
        { childNode.Parent() } -> std::same_as<Node*>;
        node.SetParent(parentNode);
    };

    /// Tree node with parent and children.
    /// Node must satisfy `Treeable`
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
        TreeNode(Node& owner)
            requires Treeable<Node>
            :
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

        /// Takes ownership of node and appends it as a child.
        void Add(NodePtr node) {
            Insert(std::move(node), children.size());
        }

        /// Takes ownership of node and inserts it at index
        void Insert(NodePtr node, size_t index) {
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

        void RemoveAllChildren() {
            for (auto& child : children) {
                child->SetParent(nullptr);
            }

            children.clear();
        }

        /// Transfers child from this tree to another tree
        void Reparent(Node& child, TreeNode& newParent) {
            auto it = std::find_if(children.begin(), children.end(), [&](NodePtr& c) {
                return c.get() == &child;
            });
            GUARD(it != children.end())

            NodePtr holding = std::move(*it);
            children.erase(it);
            child.SetParent(nullptr);
            newParent.Add(std::move(holding));
        }
    };
} // namespace PJ
