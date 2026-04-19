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
    sut.tree.Add(std::move(child));

    ASSERT_EQ(1, sut.tree.ChildCount());
    auto& first = sut.tree.Children()[0];
    EXPECT_EQ(childPtr, first.get());
    EXPECT_EQ(&sut.tree.Owner(), first->tree.Parent());
}

TEST(TreeNode, AddRvalue)
{
    // rvalue factory result must bind directly to Add without a named variable
    TestNode sut;
    sut.tree.Add(NEW<TestNode>());

    ASSERT_EQ(1, sut.tree.ChildCount());
    EXPECT_EQ(&sut.tree.Owner(), sut.tree.Children()[0]->tree.Parent());
}

TEST(TreeNode, Insert)
{
    TestNode sut;
    auto child = NEW<TestNode>();
    auto childPtr = child.get();
    sut.tree.Insert(std::move(child), 0);

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
    sut.tree.Insert(std::move(child), 1000);

    ASSERT_EQ(1, sut.tree.ChildCount());
    auto& first = sut.tree.Children()[0];
    EXPECT_EQ(childPtr, first.get());
    EXPECT_EQ(&sut.tree.Owner(), first->tree.Parent());
}

TEST(TreeNode, InsertBefore)
{
    TestNode sut;
    auto child = NEW<TestNode>();
    sut.tree.Insert(std::move(child), 0);

    auto child2 = NEW<TestNode>();
    auto childPtr2 = child2.get();
    sut.tree.Insert(std::move(child2), 0);

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

TEST(TreeNode, Reparent)
{
    TestNodeShared oldParent;
    TestNodeShared newParent;

    auto child = MAKE<TestNodeShared>();
    auto childPtr = child.get();
    oldParent.tree.Add(child);

    ASSERT_EQ(1, oldParent.tree.ChildCount());
    ASSERT_EQ(0, newParent.tree.ChildCount());
    EXPECT_EQ(&oldParent, childPtr->Parent());

    oldParent.tree.Reparent(*childPtr, newParent.tree);

    EXPECT_EQ(0, oldParent.tree.ChildCount());
    ASSERT_EQ(1, newParent.tree.ChildCount());
    EXPECT_EQ(childPtr, newParent.tree.Children()[0].get());
    EXPECT_EQ(&newParent, childPtr->Parent());
}

TEST(TreeNode, ReparentUP)
{
    // Reparent must preserve UP ownership — child must still be alive after the move
    TestNode oldParent;
    TestNode newParent;

    auto child = NEW<TestNode>();
    auto childPtr = child.get();
    oldParent.tree.Add(std::move(child));

    oldParent.tree.Reparent(*childPtr, newParent.tree);

    EXPECT_EQ(0, oldParent.tree.ChildCount());
    ASSERT_EQ(1, newParent.tree.ChildCount());
    EXPECT_EQ(childPtr, newParent.tree.Children()[0].get());
    EXPECT_EQ(&newParent, childPtr->Parent());
}

TEST(TreeNode, ReparentNonChildDoesNothing)
{
    TestNodeShared parent;
    TestNodeShared otherParent;

    auto sibling = MAKE<TestNodeShared>();
    otherParent.tree.Add(sibling);

    // `sibling` is not a child of `parent` — Reparent must be a no-op
    parent.tree.Reparent(*sibling, otherParent.tree);

    EXPECT_EQ(0, parent.tree.ChildCount());
    EXPECT_EQ(1, otherParent.tree.ChildCount());
    EXPECT_EQ(&otherParent, sibling->Parent());
}

TEST(TreeNode, InsertAlreadyParentedIsNoOp)
{
    TestNodeShared firstParent;
    TestNodeShared secondParent;

    auto child = MAKE<TestNodeShared>();
    firstParent.tree.Add(child);

    // child already has a parent — insert into a different tree should do nothing
    secondParent.tree.Add(child);

    EXPECT_EQ(1, firstParent.tree.ChildCount());
    EXPECT_EQ(0, secondParent.tree.ChildCount());
    EXPECT_EQ(&firstParent, child->Parent());
}
