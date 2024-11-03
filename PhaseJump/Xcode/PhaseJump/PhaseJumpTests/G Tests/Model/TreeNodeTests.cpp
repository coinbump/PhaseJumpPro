#include "gtest/gtest.h"
#include "TreeNode.h"
#include "Funcs.h"

using namespace PJ;
using namespace std;

namespace TreeNodeTests {
    class TestNode {
    public:
        using This = TestNode;

        TreeNode<This, UP<This>> tree;

        TestNode() : tree(*this) {
        }

        This* Parent() const { return tree.Parent(); }
        void SetParent(This* value) { tree.SetParent(value); }
    };

    class TestNodeShared {
    public:
        using This = TestNodeShared;

        TreeNode<This, SP<This>> tree;

        TestNodeShared() : tree(*this) {
        }

        This* Parent() const { return tree.Parent(); }
        void SetParent(This* value) { tree.SetParent(value); }
    };
}

using namespace TreeNodeTests;

TEST(TreeNode, Init)
{
    TestNode sut;
    EXPECT_EQ(&sut, &sut.tree.Owner());
    EXPECT_EQ(nullptr, sut.tree.Parent());
    EXPECT_EQ(0, sut.tree.ChildCount());
}

TEST(TreeNode, SetParent)
{
    TestNode sut;
    EXPECT_EQ(nullptr, sut.tree.Parent());
    sut.tree.SetParent((TestNode*)10);
    EXPECT_EQ((TestNode*)10, sut.tree.Parent());
}

TEST(TreeNode, Add)
{
    TestNode sut;
    auto child = NEW<TestNode>();
    auto childPtr = child.get();
    sut.tree.Add(child);

    ASSERT_EQ(1, sut.tree.ChildCount());
    auto& first = sut.tree.Children()[0];
    EXPECT_EQ(childPtr, first.get());
    EXPECT_EQ(&sut.tree.Owner(), first->tree.Parent());
}

TEST(TreeNode, Insert)
{
    TestNode sut;
    auto child = NEW<TestNode>();
    auto childPtr = child.get();
    sut.tree.Insert(child, 0);

    ASSERT_EQ(1, sut.tree.ChildCount());
    auto& first = sut.tree.Children()[0];
    EXPECT_EQ(childPtr, first.get());
    EXPECT_EQ(&sut.tree.Owner(), first->tree.Parent());
}

TEST(TreeNode, InsertInvalid)
{
    TestNode sut;
    auto child = NEW<TestNode>();
    auto childPtr = child.get();
    sut.tree.Insert(child, 1000);

    ASSERT_EQ(1, sut.tree.ChildCount());
    auto& first = sut.tree.Children()[0];
    EXPECT_EQ(childPtr, first.get());
    EXPECT_EQ(&sut.tree.Owner(), first->tree.Parent());
}

TEST(TreeNode, InsertBefore)
{
    TestNode sut;
    auto child = NEW<TestNode>();
    sut.tree.Insert(child, 0);

    auto child2 = NEW<TestNode>();
    auto childPtr2 = child2.get();
    sut.tree.Insert(child2, 0);

    ASSERT_EQ(2, sut.tree.ChildCount());
    auto& first = sut.tree.Children()[0];
    EXPECT_EQ(childPtr2, first.get());
    EXPECT_EQ(&sut.tree.Owner(), first->tree.Parent());
}

TEST(TreeNode, Remove)
{
    TestNodeShared sut;
    auto child = MAKE<TestNodeShared>();
    sut.tree.Add(child);

    ASSERT_EQ(1, sut.tree.ChildCount());
    EXPECT_EQ(&sut, child->Parent());

    sut.tree.Remove(*child);
    ASSERT_EQ(0, sut.tree.ChildCount());
    EXPECT_EQ(nullptr, child->Parent());
}

TEST(TreeNode, RemoveAllChildren)
{
    TestNodeShared sut;
    auto child1 = MAKE<TestNodeShared>();
    sut.tree.Add(child1);
    auto child2 = MAKE<TestNodeShared>();
    sut.tree.Add(child2);

    ASSERT_EQ(2, sut.tree.ChildCount());
    EXPECT_EQ(&sut, child1->Parent());
    EXPECT_EQ(&sut, child2->Parent());

    sut.tree.RemoveAllChildren();
    ASSERT_EQ(0, sut.tree.ChildCount());
    EXPECT_EQ(nullptr, child1->Parent());
    EXPECT_EQ(nullptr, child2->Parent());
}
