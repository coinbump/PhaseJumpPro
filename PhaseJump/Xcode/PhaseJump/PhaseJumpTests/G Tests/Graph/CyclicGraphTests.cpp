#include "gtest/gtest.h"
#include "CyclicGraph.h"
#include "StandardEdgeModel.h"

using namespace PJ;
using namespace std;

namespace CyclicGraphTests {
    class TestGraph : public CyclicGraph<StandardEdgeModel>
    {
    };

    class Node : public CyclicGraphNode<StandardEdgeModel>
    {
    public:
        using Base = CyclicGraphNode<StandardEdgeModel>;

        float time;

        Node() : time(0)
        {
        }

        void OnUpdate(TimeSlice time) override
        {
            Base::OnUpdate(time);
            this->time += time.delta;
        }
    };
}

using namespace CyclicGraphTests;

TEST(CyclicGraph, Test_RemoveRoot_RootIsNull)
{
    TestGraph graph;
    auto node = MAKE<Node>();
    auto childNode = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeModel(), childNode);

    graph.SetRoot(node);

    EXPECT_EQ(node, graph.Root());
    graph.Remove(node);
    EXPECT_EQ(nullptr, graph.Root());
}

TEST(CyclicGraph, Test_RemoveFromGraph_FromEdgesRemoved)
{
    TestGraph graph;
    auto node = MAKE<Node>();
    auto childNode = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeModel(), childNode);

    EXPECT_EQ(1, childNode->FromNodes().Count());

    graph.Remove(node);

    EXPECT_EQ(0, childNode->FromNodes().Count());
}

TEST(CyclicGraph, Test_RemoveFromGraph_ToEdgesRemoved)
{
    TestGraph graph;
    auto node = MAKE<Node>();
    auto childNode = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeModel(), childNode);

    EXPECT_EQ(1, node->Edges().Count());

    graph.Remove(childNode);

    EXPECT_EQ(0, node->Edges().Count());
}

TEST(CyclicGraph, Test_AddEdge_IsAdded)
{
    TestGraph graph;
    auto node = MAKE<Node>();
    auto childNode = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeModel(), childNode);

    auto connectedNodes = node->CollectConnectedTo(true);
    EXPECT_EQ(connectedNodes.Count(), 1);
    EXPECT_TRUE(connectedNodes.Contains(childNode));
    EXPECT_EQ(node->Edges().Count(), 1);
    EXPECT_EQ(childNode->Edges().Count(), 0);
    EXPECT_EQ(childNode->FromNodes().Count(), 1);
}

TEST(CyclicGraph, Test_AddEdges_IsAdded)
{
    TestGraph graph;
    auto node = MAKE<Node>();
    auto childNode1 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeModel(), childNode1);

    auto childNode2 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeModel(), childNode2);

    auto connectedNodes = node->CollectConnectedTo(true);
    EXPECT_EQ(connectedNodes.Count(), 2);
    EXPECT_TRUE(connectedNodes.Contains(childNode1));
    EXPECT_TRUE(connectedNodes.Contains(childNode2));
    EXPECT_EQ(node->Edges().Count(), 2);
    EXPECT_EQ(childNode1->Edges().Count(), 0);
    EXPECT_EQ(childNode1->FromNodes().Count(), 1);
    EXPECT_EQ(childNode2->Edges().Count(), 0);
    EXPECT_EQ(childNode2->FromNodes().Count(), 1);
}

TEST(CyclicGraph, Test_Clear_RemovesEdges)
{
    TestGraph graph;
    auto node = MAKE<Node>();
    auto childNode1 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeModel(), childNode1);

    auto childNode2 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeModel(), childNode2);

    EXPECT_EQ(childNode1->FromNodes().Count(), 1);
    EXPECT_EQ(childNode2->FromNodes().Count(), 1);

    node->Clear();
    auto connectedNodes = node->CollectConnectedTo(true);
    EXPECT_EQ(connectedNodes.Count(), 0);
    EXPECT_EQ(node->Edges().Count(), 0);
    EXPECT_EQ(childNode1->FromNodes().Count(), 0);
    EXPECT_EQ(childNode2->FromNodes().Count(), 0);
}

TEST(CyclicGraph, UpdateGraph_UpdatesAll)
{
    TestGraph graph;
    auto node = MAKE<Node>();
    auto childNode1 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeModel(), childNode1);

    auto childNode2 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeModel(), childNode2);

    auto deepNode = MAKE<Node>();
    graph.AddEdge(childNode1, StandardEdgeModel(), deepNode);

    auto delta = 4.0f;
    graph.OnUpdate(TimeSlice(delta));

    EXPECT_EQ(node->time, delta);
    EXPECT_EQ(childNode1->time, delta);
    EXPECT_EQ(childNode2->time, delta);
    EXPECT_EQ(deepNode->time, delta);
}

TEST(CyclicGraph, Test_RemoveEdgeFromParent_RemovesBoth)
{
    TestGraph graph;
    auto node = MAKE<Node>();
    auto childNode1 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeModel(), childNode1);

    EXPECT_EQ(node->Edges().Count(), 1);
    EXPECT_EQ(childNode1->FromNodes().Count(), 1);

    node->RemoveEdge(node->Edges()[0]);

    EXPECT_EQ(node->Edges().Count(), 0);
    EXPECT_EQ(childNode1->FromNodes().Count(), 0);
}

TEST(CyclicGraph, Test_RemoveEdgesTo)
{
    TestGraph graph;
    auto node = MAKE<Node>();

    auto childNode1 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeModel(), childNode1);

    auto childNode2 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeModel(), childNode2);

    EXPECT_EQ(node->Edges().Count(), 2);
    EXPECT_EQ(childNode1->Edges().Count(), 0);
    EXPECT_EQ(childNode1->FromNodes().Count(), 1);
    EXPECT_EQ(childNode2->FromNodes().Count(), 1);

    node->RemoveEdgesTo(childNode1);
    EXPECT_EQ(node->Edges().Count(), 1);
    EXPECT_EQ(node->Edges()[0]->toNode->Value(), childNode2);
    EXPECT_EQ(childNode1->FromNodes().Count(), 0);
    EXPECT_EQ(childNode2->FromNodes().Count(), 1);
}

TEST(CyclicGraph, Test_RemoveEdgesFrom)
{
    TestGraph graph;
    auto node = MAKE<Node>();
    auto childNode1 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeModel(), childNode1);

    auto deepNode = MAKE<Node>();
    graph.AddEdge(deepNode, StandardEdgeModel(), childNode1);

    EXPECT_EQ(node->Edges().Count(), 1);
    EXPECT_EQ(childNode1->FromNodes().Count(), 2);
    EXPECT_EQ(deepNode->Edges().Count(), 1);
    EXPECT_EQ(deepNode->FromNodes().Count(), 0);

    childNode1->RemoveEdgesFrom(node);

    EXPECT_EQ(node->Edges().Count(), 0);
    EXPECT_EQ(childNode1->FromNodes().Count(), 1);
    EXPECT_EQ(deepNode->Edges().Count(), 1);
    EXPECT_EQ(deepNode->FromNodes().Count(), 0);

    childNode1->RemoveEdgesFrom(deepNode);

    EXPECT_EQ(node->Edges().Count(), 0);
    EXPECT_EQ(childNode1->Edges().Count(), 0);
    EXPECT_EQ(childNode1->FromNodes().Count(), 0);
    EXPECT_EQ(deepNode->Edges().Count(), 0);
}

TEST(CyclicGraph, Test_CollectGraph)
{
    TestGraph graph;
    auto node = MAKE<Node>();
    auto childNode1 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeModel(), childNode1);

    auto childNode2 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeModel(), childNode2);

    auto deepNode = MAKE<Node>();
    graph.AddEdge(childNode1, StandardEdgeModel(), deepNode);
    graph.AddEdge(deepNode, StandardEdgeModel(), node);  // Circular connection

    auto collectedGraph = node->CollectGraph();
    EXPECT_EQ(collectedGraph.Count(), 4);
    EXPECT_TRUE(collectedGraph.Contains(node));
    EXPECT_TRUE(collectedGraph.Contains(childNode1));
    EXPECT_TRUE(collectedGraph.Contains(childNode2));
    EXPECT_TRUE(collectedGraph.Contains(deepNode));
}

TEST(CyclicGraph, Test_CollectConnectedToNotDeep)
{
    TestGraph graph;
    auto node = MAKE<Node>();
    auto childNode1 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeModel(), childNode1);

    auto childNode2 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeModel(), childNode2);

    auto deepNode = MAKE<Node>();
    childNode1->AddEdge(StandardEdgeModel(), deepNode);

    auto collectedGraph = node->CollectConnectedTo(false);
    EXPECT_EQ(collectedGraph.Count(), 2);
    EXPECT_TRUE(collectedGraph.Contains(childNode1));
    EXPECT_TRUE(collectedGraph.Contains(childNode2));
}

TEST(CyclicGraph, Test_CollectConnectedToDeep)
{
    TestGraph graph;
    auto node = MAKE<Node>();
    auto childNode1 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeModel(), childNode1);

    auto childNode2 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeModel(), childNode2);

    auto deepNode = MAKE<Node>();
    graph.AddEdge(childNode1, StandardEdgeModel(), deepNode);

    auto collectedGraph = node->CollectConnectedTo(true);
    EXPECT_EQ(collectedGraph.Count(), 3);
    EXPECT_TRUE(collectedGraph.Contains(childNode1));
    EXPECT_TRUE(collectedGraph.Contains(childNode2));
    EXPECT_TRUE(collectedGraph.Contains(deepNode));
}

TEST(CyclicGraph, Test_CollectConnectedToCircular)
{
    TestGraph graph;
    auto node = MAKE<Node>();
    auto childNode1 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeModel(), childNode1);

    auto childNode2 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeModel(), childNode2);

    auto deepNode = MAKE<Node>();
    graph.AddEdge(childNode1, StandardEdgeModel(), deepNode);
    graph.AddEdge(deepNode, StandardEdgeModel(), node);  // Circular connection

    auto collectedGraph = node->CollectConnectedTo(true);
    EXPECT_EQ(collectedGraph.Count(), 4);
    EXPECT_TRUE(collectedGraph.Contains(node));
    EXPECT_TRUE(collectedGraph.Contains(childNode1));
    EXPECT_TRUE(collectedGraph.Contains(childNode2));
    EXPECT_TRUE(collectedGraph.Contains(deepNode));
}

TEST(CyclicGraph, Test_CollectDepthFirstGraphTree)
{
    TestGraph graph;
    auto node = MAKE<Node>();
    auto childNode1 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeModel(), childNode1);

    auto childNode2 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeModel(), childNode2);

    auto deepNode = MAKE<Node>();
    graph.AddEdge(childNode1, StandardEdgeModel(), deepNode);

    auto collectedGraph = node->CollectDepthFirstGraph();
    EXPECT_EQ(collectedGraph.Count(), 4);
    EXPECT_EQ(node, collectedGraph[0]);
    EXPECT_EQ(childNode1, collectedGraph[1]);
    EXPECT_EQ(deepNode, collectedGraph[2]);
    EXPECT_EQ(childNode2, collectedGraph[3]);
}

TEST(CyclicGraph, Test_CollectDepthFirstGraphTree2)
{
    TestGraph graph;
    auto node = MAKE<Node>();
    auto childNode1 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeModel(), childNode1);

    auto childNode2 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeModel(), childNode2);

    auto deepNode = MAKE<Node>();
    graph.AddEdge(childNode2, StandardEdgeModel(), deepNode);

    auto collectedGraph = node->CollectDepthFirstGraph();
    EXPECT_EQ(collectedGraph.Count(), 4);
    EXPECT_EQ(node, collectedGraph[0]);
    EXPECT_EQ(childNode1, collectedGraph[1]);
    EXPECT_EQ(childNode2, collectedGraph[2]);
    EXPECT_EQ(deepNode, collectedGraph[3]);
}

TEST(CyclicGraph, Test_CollectDepthFirstGraphCircular)
{
    TestGraph graph;
    auto node = MAKE<Node>();
    auto childNode1 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeModel(), childNode1);

    auto childNode2 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeModel(), childNode2);

    auto deepNode = MAKE<Node>();
    graph.AddEdge(childNode1, StandardEdgeModel(), deepNode);
    graph.AddEdge(deepNode, StandardEdgeModel(), node);  // Circular connection

    auto collectedGraph = node->CollectDepthFirstGraph();
    EXPECT_EQ(collectedGraph.Count(), 4);
    EXPECT_EQ(node, collectedGraph[0]);
    EXPECT_EQ(childNode1, collectedGraph[1]);
    EXPECT_EQ(deepNode, collectedGraph[2]);
    EXPECT_EQ(childNode2, collectedGraph[3]);
}

TEST(CyclicGraph, Test_CollectBreadthFirstGraphTree)
{
    TestGraph graph;
    auto node = MAKE<Node>();
    auto childNode1 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeModel(), childNode1);

    auto childNode2 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeModel(), childNode2);

    auto deepNode = MAKE<Node>();
    graph.AddEdge(childNode1, StandardEdgeModel(), deepNode);

    auto collectedGraph = node->CollectBreadthFirstGraph();
    EXPECT_EQ(4, collectedGraph.Count());
    EXPECT_EQ(node, collectedGraph[0]);
    EXPECT_EQ(childNode1, collectedGraph[1]);
    EXPECT_EQ(childNode2, collectedGraph[2]);
    EXPECT_EQ(deepNode, collectedGraph[3]);
}

TEST(CyclicGraph, Test_CollectBreadthFirstGraphTree2)
{
    TestGraph graph;
    auto node = MAKE<Node>();
    auto childNode1 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeModel(), childNode1);

    auto childNode2 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeModel(), childNode2);

    auto deepNode = MAKE<Node>();
    graph.AddEdge(childNode2, StandardEdgeModel(), deepNode);

    auto collectedGraph = node->CollectBreadthFirstGraph();
    EXPECT_EQ(4, collectedGraph.Count());
    EXPECT_EQ(node, collectedGraph[0]);
    EXPECT_EQ(childNode1, collectedGraph[1]);
    EXPECT_EQ(childNode2, collectedGraph[2]);
    EXPECT_EQ(deepNode, collectedGraph[3]);
}

TEST(CyclicGraph, Test_CollectBreadthFirstGraphCircular)
{
    TestGraph graph;
    auto node = MAKE<Node>();
    auto childNode1 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeModel(), childNode1);

    auto childNode2 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeModel(), childNode2);

    auto deepNode = MAKE<Node>();
    graph.AddEdge(childNode1, StandardEdgeModel(), deepNode);
    graph.AddEdge(deepNode, StandardEdgeModel(), node);  // Circular connection

    auto collectedGraph = node->CollectBreadthFirstGraph();
    EXPECT_EQ(4, collectedGraph.Count());
    EXPECT_EQ(node, collectedGraph[0]);
    EXPECT_EQ(childNode1, collectedGraph[1]);
    EXPECT_EQ(childNode2, collectedGraph[2]);
    EXPECT_EQ(deepNode, collectedGraph[3]);
}
