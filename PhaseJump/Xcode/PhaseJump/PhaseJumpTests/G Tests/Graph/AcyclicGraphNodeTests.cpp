#include "gtest/gtest.h"
#include "Graph/AcyclicGraphNode.h"
#include "GraphNodeTool.h"

using namespace PJ;
using namespace std;

namespace AcyclicGraphNodeTests {
    class Node : public AcyclicGraphNode<StandardEdgeCore>
    {
    public:
        using Base = AcyclicGraphNode<StandardEdgeCore>;

        float time;

        Node() : time(0)
        {
        }
    };
}

using namespace AcyclicGraphNodeTests;

TEST(AcyclicGraphNode, Test_AddEdge_IsAdded)
{
    auto node = MAKE<Node>();
    auto childNode = MAKE<Node>();
    node->AddEdge(childNode);

    GraphNodeTool<> graphNodeTool;
    auto connectedNodes = graphNodeTool.CollectConnectedTo(node, true);
    EXPECT_EQ(connectedNodes.size(), 1);
    EXPECT_TRUE(Contains(connectedNodes, childNode));
    EXPECT_EQ(node->Edges().size(), 1);
    EXPECT_EQ(childNode->Edges().size(), 0);
    EXPECT_EQ(childNode->FromNodes().size(), 1);
}

TEST(AcyclicGraphNode, Test_AddEdges_IsAdded)
{
    auto node = MAKE<Node>();
    auto childNode1 = MAKE<Node>();
    node->AddEdge(childNode1);

    auto childNode2 = MAKE<Node>();
    node->AddEdge(childNode2);

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

TEST(AcyclicGraphNode, Test_Clear_RemovesEdges)
{
    auto node = MAKE<Node>();
    auto childNode1 = MAKE<Node>();
    node->AddEdge(childNode1);

    auto childNode2 = MAKE<Node>();
    node->AddEdge(childNode2);

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

// FUTURE: re-enable this
//TEST(AcyclicGraphNode, Test_UpdateRoot_UpdatesAll)
//{
//    auto node = MAKE<Node>();
//    auto childNode1 = MAKE<Node>();
//    node->AddEdge(childNode1);
//
//    auto childNode2 = MAKE<Node>();
//    node->AddEdge(childNode2);
//
//    auto deepNode = MAKE<Node>();
//    childNode1->AddEdge(deepNode);
//
//    auto delta = 4.0f;
//    node->OnUpdate(TimeSlice(delta));
//
//    EXPECT_EQ(node->time, delta);
//    EXPECT_EQ(childNode1->time, delta);
//    EXPECT_EQ(childNode2->time, delta);
//    EXPECT_EQ(deepNode->time, delta);
//}

//TEST(AcyclicGraphNode, Test_UpdateRootWithCircularReference_DoesNotUpdateAll)
//{
//    auto node = MAKE<Node>();
//    auto childNode1 = MAKE<Node>();
//    node->AddEdge(childNode1);
//
//    auto childNode2 = MAKE<Node>();
//    node->AddEdge(childNode2);
//
//    auto deepNode = MAKE<Node>();
//    childNode1->AddEdge(deepNode);
//    deepNode->AddEdge(node);
//
//    auto delta = 4.0f;
//    node->OnUpdate(TimeSlice(delta));
//
//    EXPECT_EQ(node->time, delta);
//    EXPECT_EQ(childNode1->time, 0);
//    EXPECT_EQ(childNode2->time, 0);
//    EXPECT_EQ(deepNode->time, 0);
//}

TEST(AcyclicGraphNode, Test_RemoveEdgeFromParent_RemovesBoth)
{
    auto node = MAKE<Node>();
    auto childNode1 = MAKE<Node>();
    node->AddEdge(childNode1);

    EXPECT_EQ(node->Edges().size(), 1);
    EXPECT_EQ(childNode1->FromNodes().size(), 1);

    node->RemoveEdge(*node->Edges()[0]);

    EXPECT_EQ(node->Edges().size(), 0);
    EXPECT_EQ(childNode1->FromNodes().size(), 0);
}

TEST(AcyclicGraphNode, Test_RemoveEdgesTo)
{
    auto node = MAKE<Node>();

    auto childNode1 = MAKE<Node>();
    node->AddEdge(childNode1);

    auto childNode2 = MAKE<Node>();
    node->AddEdge(childNode2);

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

TEST(AcyclicGraphNode, Test_RemoveEdgesFrom)
{
    auto node = MAKE<Node>();
    auto childNode1 = MAKE<Node>();
    node->AddEdge(childNode1);

    auto deepNode = MAKE<Node>();
    deepNode->AddEdge(childNode1);

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

TEST(AcyclicGraphNode, Test_CollectGraph)
{
    auto node = MAKE<Node>();
    auto childNode1 = MAKE<Node>();
    node->AddEdge(childNode1);

    auto childNode2 = MAKE<Node>();
    node->AddEdge(childNode2);

    auto deepNode = MAKE<Node>();
    childNode1->AddEdge(deepNode);
    deepNode->AddEdge(node);    // Circular connection

    GraphNodeTool<> graphNodeTool;
    auto graph =graphNodeTool.CollectGraph(node);
    
    EXPECT_EQ(graph.size(), 4);
    EXPECT_TRUE(Contains(graph, node));
    EXPECT_TRUE(Contains(graph, childNode1));
    EXPECT_TRUE(Contains(graph, childNode2));
    EXPECT_TRUE(Contains(graph, deepNode));
}

TEST(AcyclicGraphNode, Test_CollectConnectedToNotDeep)
{
    auto node = MAKE<Node>();
    auto childNode1 = MAKE<Node>();
    node->AddEdge(childNode1);

    auto childNode2 = MAKE<Node>();
    node->AddEdge(childNode2);

    auto deepNode = MAKE<Node>();
    childNode1->AddEdge(deepNode);

    GraphNodeTool<> graphNodeTool;
    auto graph = graphNodeTool.CollectConnectedTo(node, false);
    EXPECT_EQ(graph.size(), 2);
    EXPECT_TRUE(Contains(graph, childNode1));
    EXPECT_TRUE(Contains(graph, childNode2));
}

TEST(AcyclicGraphNode, Test_CollectConnectedToDeep)
{
    auto node = MAKE<Node>();
    auto childNode1 = MAKE<Node>();
    node->AddEdge(childNode1);

    auto childNode2 = MAKE<Node>();
    node->AddEdge(childNode2);

    auto deepNode = MAKE<Node>();
    childNode1->AddEdge(deepNode);

    GraphNodeTool<> graphNodeTool;
    auto graph = graphNodeTool.CollectConnectedTo(node, true);
    EXPECT_EQ(graph.size(), 3);
    EXPECT_TRUE(Contains(graph, childNode1));
    EXPECT_TRUE(Contains(graph, childNode2));
    EXPECT_TRUE(Contains(graph, deepNode));
}

TEST(AcyclicGraphNode, Test_CollectConnectedToCircular)
{
    auto node = MAKE<Node>();
    auto childNode1 = MAKE<Node>();
    node->AddEdge(childNode1);

    auto childNode2 = MAKE<Node>();
    node->AddEdge(childNode2);

    auto deepNode = MAKE<Node>();
    childNode1->AddEdge(deepNode);
    deepNode->AddEdge(node);    // Circular connection

    GraphNodeTool<> graphNodeTool;
    auto graph = graphNodeTool.CollectConnectedTo(node, true);
    EXPECT_EQ(graph.size(), 4);
    EXPECT_TRUE(Contains(graph, node));
    EXPECT_TRUE(Contains(graph, childNode1));
    EXPECT_TRUE(Contains(graph, childNode2));
    EXPECT_TRUE(Contains(graph, deepNode));
}

TEST(AcyclicGraphNode, Test_CollectDepthFirstGraphTree)
{
    auto node = MAKE<Node>();
    auto childNode1 = MAKE<Node>();
    node->AddEdge(childNode1);

    auto childNode2 = MAKE<Node>();
    node->AddEdge(childNode2);

    auto deepNode = MAKE<Node>();
    childNode1->AddEdge(deepNode);

    GraphNodeTool graphNodeTool;
    auto graph = graphNodeTool.CollectDepthFirstGraph(node);
    EXPECT_EQ(graph.size(), 4);
    EXPECT_EQ(node, graph[0]);
    EXPECT_EQ(childNode1, graph[1]);
    EXPECT_EQ(deepNode, graph[2]);
    EXPECT_EQ(childNode2, graph[3]);
}

TEST(AcyclicGraphNode, Test_CollectDepthFirstGraphTree2)
{
    auto node = MAKE<Node>();
    auto childNode1 = MAKE<Node>();
    node->AddEdge(childNode1);

    auto childNode2 = MAKE<Node>();
    node->AddEdge(childNode2);

    auto deepNode = MAKE<Node>();
    childNode2->AddEdge(deepNode);

    GraphNodeTool graphNodeTool;
    auto graph = graphNodeTool.CollectDepthFirstGraph(node);
    EXPECT_EQ(graph.size(), 4);
    EXPECT_EQ(node, graph[0]);
    EXPECT_EQ(childNode1, graph[1]);
    EXPECT_EQ(childNode2, graph[2]);
    EXPECT_EQ(deepNode, graph[3]);
}

TEST(AcyclicGraphNode, Test_CollectDepthFirstGraphCircular)
{
    auto node = MAKE<Node>();
    auto childNode1 = MAKE<Node>();
    node->AddEdge(childNode1);

    auto childNode2 = MAKE<Node>();
    node->AddEdge(childNode2);

    auto deepNode = MAKE<Node>();
    childNode1->AddEdge(deepNode);
    deepNode->AddEdge(node);  // Circular connection

    GraphNodeTool graphNodeTool;
    auto graph = graphNodeTool.CollectDepthFirstGraph(node);
    EXPECT_EQ(graph.size(), 4);
    EXPECT_EQ(node, graph[0]);
    EXPECT_EQ(childNode1, graph[1]);
    EXPECT_EQ(deepNode, graph[2]);
    EXPECT_EQ(childNode2, graph[3]);
}

TEST(AcyclicGraphNode, Test_CollectBreadthFirstGraphTree)
{
    auto node = MAKE<Node>();
    auto childNode1 = MAKE<Node>();
    node->AddEdge(childNode1);

    auto childNode2 = MAKE<Node>();
    node->AddEdge(childNode2);

    auto deepNode = MAKE<Node>();
    childNode1->AddEdge(deepNode);

    GraphNodeTool graphNodeTool;
    auto graph = graphNodeTool.CollectBreadthFirstGraph(node);
    EXPECT_EQ(4, graph.size());
    EXPECT_EQ(node, graph[0]);
    EXPECT_EQ(childNode1, graph[1]);
    EXPECT_EQ(childNode2, graph[2]);
    EXPECT_EQ(deepNode, graph[3]);
}

TEST(AcyclicGraphNode, Test_CollectBreadthFirstGraphTree2)
{
    auto node = MAKE<Node>();
    auto childNode1 = MAKE<Node>();
    node->AddEdge(childNode1);

    auto childNode2 = MAKE<Node>();
    node->AddEdge(childNode2);

    auto deepNode = MAKE<Node>();
    childNode2->AddEdge(deepNode);

    GraphNodeTool graphNodeTool;
    auto graph = graphNodeTool.CollectBreadthFirstGraph(node);
    EXPECT_EQ(4, graph.size());
    EXPECT_EQ(node, graph[0]);
    EXPECT_EQ(childNode1, graph[1]);
    EXPECT_EQ(childNode2, graph[2]);
    EXPECT_EQ(deepNode, graph[3]);
}

TEST(AcyclicGraphNode, Test_CollectBreadthFirstGraphCircular)
{
    auto node = MAKE<Node>();
    auto childNode1 = MAKE<Node>();
    node->AddEdge(childNode1);

    auto childNode2 = MAKE<Node>();
    node->AddEdge(childNode2);

    auto deepNode = MAKE<Node>();
    childNode1->AddEdge(deepNode);
    deepNode->AddEdge(node);  // Circular connection

    GraphNodeTool graphNodeTool;
    auto graph = graphNodeTool.CollectBreadthFirstGraph(node);
    EXPECT_EQ(4, graph.size());
    EXPECT_EQ(node, graph[0]);
    EXPECT_EQ(childNode1, graph[1]);
    EXPECT_EQ(childNode2, graph[2]);
    EXPECT_EQ(deepNode, graph[3]);
}

TEST(AcyclicGraphNode, Test_Root)
{
    auto node = MAKE<Node>();
    auto childNode1 = MAKE<Node>();
    node->AddEdge(childNode1);

    auto childNode2 = MAKE<Node>();
    node->AddEdge(childNode2);

    auto deepNode = MAKE<Node>();
    childNode1->AddEdge(deepNode);

    EXPECT_EQ(node.get(), node->Root());
    EXPECT_EQ(node.get(), childNode1->Root());
    EXPECT_EQ(node.get(), childNode2->Root());
    EXPECT_EQ(node.get(), deepNode->Root());
}

TEST(AcyclicGraphNode, Test_RootCircular)
{
    auto node = MAKE<Node>();
    auto childNode1 = MAKE<Node>();
    node->AddEdge(childNode1);

    auto childNode2 = MAKE<Node>();
    node->AddEdge(childNode2);

    auto deepNode = MAKE<Node>();
    childNode1->AddEdge(deepNode);
    deepNode->AddEdge(node);

    EXPECT_EQ(nullptr, node->Root());
    EXPECT_EQ(nullptr, childNode2->Root());
    EXPECT_EQ(nullptr, childNode1->Root());
    EXPECT_EQ(nullptr, deepNode->Root());
}

TEST(AcyclicGraphNode, Test_IsRoot)
{
    auto node = MAKE<Node>();
    auto childNode1 = MAKE<Node>();
    node->AddEdge(childNode1);

    EXPECT_TRUE(node->IsRoot());
    EXPECT_FALSE(childNode1->IsRoot());
}

TEST(AcyclicGraphNode, Test_Parent)
{
    auto node = MAKE<Node>();
    auto childNode1 = MAKE<Node>();
    node->AddEdge(childNode1);

    EXPECT_EQ(nullptr, node->Parent());
    EXPECT_EQ(node.get(), childNode1->Parent());
}
