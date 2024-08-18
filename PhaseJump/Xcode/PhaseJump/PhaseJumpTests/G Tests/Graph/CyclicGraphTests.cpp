#include "gtest/gtest.h"
#include "CyclicGraph.h"
#include "StandardEdgeCore.h"
#include "GraphNodeTool.h"

using namespace PJ;
using namespace std;

namespace CyclicGraphTests {
    class TestGraph : public CyclicGraph<CyclicGraphNode<StandardEdgeCore>>
    {
    };

    class Node : public CyclicGraphNode<StandardEdgeCore>
    {
    public:
        using Base = CyclicGraphNode<StandardEdgeCore>;

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
    graph.AddEdge(node, StandardEdgeCore(), childNode);

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
    graph.AddEdge(node, StandardEdgeCore(), childNode);

    EXPECT_EQ(1, childNode->FromNodes().size());

    graph.Remove(node);

    EXPECT_EQ(0, childNode->FromNodes().size());
}

TEST(CyclicGraph, Test_RemoveFromGraph_ToEdgesRemoved)
{
    TestGraph graph;
    auto node = MAKE<Node>();
    auto childNode = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeCore(), childNode);

    EXPECT_EQ(1, node->Edges().size());

    graph.Remove(childNode);

    EXPECT_EQ(0, node->Edges().size());
}

TEST(CyclicGraph, Test_AddEdge_IsAdded)
{
    TestGraph graph;
    auto node = MAKE<Node>();
    auto childNode = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeCore(), childNode);

    GraphNodeTool<> graphNodeTool;
    auto connectedNodes = graphNodeTool.CollectConnectedTo(node, true);
    EXPECT_EQ(connectedNodes.size(), 1);
    EXPECT_TRUE(Contains(connectedNodes, childNode));
    EXPECT_EQ(node->Edges().size(), 1);
    EXPECT_EQ(childNode->Edges().size(), 0);
    EXPECT_EQ(childNode->FromNodes().size(), 1);
}

TEST(CyclicGraph, Test_AddEdges_IsAdded)
{
    TestGraph graph;
    auto node = MAKE<Node>();
    auto childNode1 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeCore(), childNode1);

    auto childNode2 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeCore(), childNode2);

    GraphNodeTool<> graphNodeTool;
    auto connectedNodes = graphNodeTool.CollectConnectedTo(node, true);
    EXPECT_EQ(connectedNodes.size(), 2);
    EXPECT_TRUE(Contains(connectedNodes, childNode1));
    EXPECT_TRUE(Contains(connectedNodes, childNode2));
    EXPECT_EQ(node->Edges().size(), 2);
    EXPECT_EQ(childNode1->Edges().size(), 0);
    EXPECT_EQ(childNode1->FromNodes().size(), 1);
    EXPECT_EQ(childNode2->Edges().size(), 0);
    EXPECT_EQ(childNode2->FromNodes().size(), 1);
}

TEST(CyclicGraph, Test_Clear_RemovesEdges)
{
    TestGraph graph;
    auto node = MAKE<Node>();
    auto childNode1 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeCore(), childNode1);

    auto childNode2 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeCore(), childNode2);

    EXPECT_EQ(childNode1->FromNodes().size(), 1);
    EXPECT_EQ(childNode2->FromNodes().size(), 1);

    node->RemoveAllEdges();
    GraphNodeTool<> graphNodeTool;
    auto connectedNodes = graphNodeTool.CollectConnectedTo(node, true);
    EXPECT_EQ(connectedNodes.size(), 0);
    EXPECT_EQ(node->Edges().size(), 0);
    EXPECT_EQ(childNode1->FromNodes().size(), 0);
    EXPECT_EQ(childNode2->FromNodes().size(), 0);
}

TEST(CyclicGraph, UpdateGraph_UpdatesAll)
{
    TestGraph graph;
    auto node = MAKE<Node>();
    auto childNode1 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeCore(), childNode1);

    auto childNode2 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeCore(), childNode2);

    auto deepNode = MAKE<Node>();
    graph.AddEdge(childNode1, StandardEdgeCore(), deepNode);

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
    graph.AddEdge(node, StandardEdgeCore(), childNode1);

    EXPECT_EQ(node->Edges().size(), 1);
    EXPECT_EQ(childNode1->FromNodes().size(), 1);

    node->RemoveEdge(node->Edges()[0]);

    EXPECT_EQ(node->Edges().size(), 0);
    EXPECT_EQ(childNode1->FromNodes().size(), 0);
}

TEST(CyclicGraph, Test_RemoveEdgesTo)
{
    TestGraph graph;
    auto node = MAKE<Node>();

    auto childNode1 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeCore(), childNode1);

    auto childNode2 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeCore(), childNode2);

    EXPECT_EQ(node->Edges().size(), 2);
    EXPECT_EQ(childNode1->Edges().size(), 0);
    EXPECT_EQ(childNode1->FromNodes().size(), 1);
    EXPECT_EQ(childNode2->FromNodes().size(), 1);

    node->RemoveEdgesTo(*childNode1);
    EXPECT_EQ(node->Edges().size(), 1);
    EXPECT_EQ(node->Edges()[0]->toNode->Value(), childNode2);
    EXPECT_EQ(childNode1->FromNodes().size(), 0);
    EXPECT_EQ(childNode2->FromNodes().size(), 1);
}

TEST(CyclicGraph, Test_RemoveEdgesFrom)
{
    TestGraph graph;
    auto node = MAKE<Node>();
    auto childNode1 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeCore(), childNode1);

    auto deepNode = MAKE<Node>();
    graph.AddEdge(deepNode, StandardEdgeCore(), childNode1);

    EXPECT_EQ(node->Edges().size(), 1);
    EXPECT_EQ(childNode1->FromNodes().size(), 2);
    EXPECT_EQ(deepNode->Edges().size(), 1);
    EXPECT_EQ(deepNode->FromNodes().size(), 0);

    childNode1->RemoveEdgesFrom(*node);

    EXPECT_EQ(node->Edges().size(), 0);
    EXPECT_EQ(childNode1->FromNodes().size(), 1);
    EXPECT_EQ(deepNode->Edges().size(), 1);
    EXPECT_EQ(deepNode->FromNodes().size(), 0);

    childNode1->RemoveEdgesFrom(*deepNode);

    EXPECT_EQ(node->Edges().size(), 0);
    EXPECT_EQ(childNode1->Edges().size(), 0);
    EXPECT_EQ(childNode1->FromNodes().size(), 0);
    EXPECT_EQ(deepNode->Edges().size(), 0);
}

TEST(CyclicGraph, Test_CollectGraph)
{
    TestGraph graph;
    auto node = MAKE<Node>();
    auto childNode1 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeCore(), childNode1);

    auto childNode2 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeCore(), childNode2);

    auto deepNode = MAKE<Node>();
    graph.AddEdge(childNode1, StandardEdgeCore(), deepNode);
    graph.AddEdge(deepNode, StandardEdgeCore(), node);  // Circular connection

    GraphNodeTool graphNodeTool;
    auto collectedGraph = graphNodeTool.CollectGraph(node);
    EXPECT_EQ(collectedGraph.size(), 4);
    EXPECT_TRUE(Contains(collectedGraph, node));
    EXPECT_TRUE(Contains(collectedGraph, childNode1));
    EXPECT_TRUE(Contains(collectedGraph, childNode2));
    EXPECT_TRUE(Contains(collectedGraph, deepNode));
}

TEST(CyclicGraph, Test_CollectConnectedToNotDeep)
{
    TestGraph graph;
    auto node = MAKE<Node>();
    auto childNode1 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeCore(), childNode1);

    auto childNode2 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeCore(), childNode2);

    auto deepNode = MAKE<Node>();
    childNode1->AddEdge(deepNode);

    GraphNodeTool<> graphNodeTool;
    auto collectedGraph = graphNodeTool.CollectConnectedTo(node, false);
    EXPECT_EQ(collectedGraph.size(), 2);
    EXPECT_TRUE(Contains(collectedGraph, childNode1));
    EXPECT_TRUE(Contains(collectedGraph, childNode2));
}

TEST(CyclicGraph, Test_CollectConnectedToDeep)
{
    TestGraph graph;
    auto node = MAKE<Node>();
    auto childNode1 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeCore(), childNode1);

    auto childNode2 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeCore(), childNode2);

    auto deepNode = MAKE<Node>();
    graph.AddEdge(childNode1, StandardEdgeCore(), deepNode);

    GraphNodeTool<> graphNodeTool;
    auto collectedGraph = graphNodeTool.CollectConnectedTo(node, true);
    EXPECT_EQ(collectedGraph.size(), 3);
    EXPECT_TRUE(Contains(collectedGraph, childNode1));
    EXPECT_TRUE(Contains(collectedGraph, childNode2));
    EXPECT_TRUE(Contains(collectedGraph, deepNode));
}

TEST(CyclicGraph, Test_CollectConnectedToCircular)
{
    TestGraph graph;
    auto node = MAKE<Node>();
    auto childNode1 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeCore(), childNode1);

    auto childNode2 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeCore(), childNode2);

    auto deepNode = MAKE<Node>();
    graph.AddEdge(childNode1, StandardEdgeCore(), deepNode);
    graph.AddEdge(deepNode, StandardEdgeCore(), node);  // Circular connection

    GraphNodeTool<> graphNodeTool;
    auto collectedGraph = graphNodeTool.CollectConnectedTo(node, true);
    EXPECT_EQ(collectedGraph.size(), 4);
    EXPECT_TRUE(Contains(collectedGraph, node));
    EXPECT_TRUE(Contains(collectedGraph, childNode1));
    EXPECT_TRUE(Contains(collectedGraph, childNode2));
    EXPECT_TRUE(Contains(collectedGraph, deepNode));
}

TEST(CyclicGraph, Test_CollectDepthFirstGraphTree)
{
    TestGraph graph;
    auto node = MAKE<Node>();
    auto childNode1 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeCore(), childNode1);

    auto childNode2 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeCore(), childNode2);

    auto deepNode = MAKE<Node>();
    graph.AddEdge(childNode1, StandardEdgeCore(), deepNode);

    GraphNodeTool graphNodeTool;
    auto collectedGraph = graphNodeTool.CollectDepthFirstGraph(node);
    EXPECT_EQ(collectedGraph.size(), 4);
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
    graph.AddEdge(node, StandardEdgeCore(), childNode1);

    auto childNode2 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeCore(), childNode2);

    auto deepNode = MAKE<Node>();
    graph.AddEdge(childNode2, StandardEdgeCore(), deepNode);

    GraphNodeTool graphNodeTool;
    auto collectedGraph = graphNodeTool.CollectDepthFirstGraph(node);
    EXPECT_EQ(collectedGraph.size(), 4);
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
    graph.AddEdge(node, StandardEdgeCore(), childNode1);

    auto childNode2 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeCore(), childNode2);

    auto deepNode = MAKE<Node>();
    graph.AddEdge(childNode1, StandardEdgeCore(), deepNode);
    graph.AddEdge(deepNode, StandardEdgeCore(), node);  // Circular connection

    GraphNodeTool graphNodeTool;
    auto collectedGraph = graphNodeTool.CollectDepthFirstGraph(node);
    EXPECT_EQ(collectedGraph.size(), 4);
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
    graph.AddEdge(node, StandardEdgeCore(), childNode1);

    auto childNode2 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeCore(), childNode2);

    auto deepNode = MAKE<Node>();
    graph.AddEdge(childNode1, StandardEdgeCore(), deepNode);

    GraphNodeTool graphNodeTool;
    auto collectedGraph = graphNodeTool.CollectBreadthFirstGraph(node);
    EXPECT_EQ(4, collectedGraph.size());
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
    graph.AddEdge(node, StandardEdgeCore(), childNode1);

    auto childNode2 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeCore(), childNode2);

    auto deepNode = MAKE<Node>();
    graph.AddEdge(childNode2, StandardEdgeCore(), deepNode);

    GraphNodeTool graphNodeTool;
    auto collectedGraph = graphNodeTool.CollectBreadthFirstGraph(node);
    EXPECT_EQ(4, collectedGraph.size());
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
    graph.AddEdge(node, StandardEdgeCore(), childNode1);

    auto childNode2 = MAKE<Node>();
    graph.AddEdge(node, StandardEdgeCore(), childNode2);

    auto deepNode = MAKE<Node>();
    graph.AddEdge(childNode1, StandardEdgeCore(), deepNode);
    graph.AddEdge(deepNode, StandardEdgeCore(), node);  // Circular connection

    GraphNodeTool graphNodeTool;
    auto collectedGraph = graphNodeTool.CollectBreadthFirstGraph(node);
    EXPECT_EQ(4, collectedGraph.size());
    EXPECT_EQ(node, collectedGraph[0]);
    EXPECT_EQ(childNode1, collectedGraph[1]);
    EXPECT_EQ(childNode2, collectedGraph[2]);
    EXPECT_EQ(deepNode, collectedGraph[3]);
}
