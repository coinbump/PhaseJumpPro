#include "gtest/gtest.h"
#include "Graph/AcyclicGraphNode.h"

using namespace PJ;
using namespace std;

namespace AcyclicGraphNodeTests {
    class Node : public AcyclicGraphNode<StandardEdgeModel>
    {
    public:
        using Base = AcyclicGraphNode<StandardEdgeModel>;

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

using namespace AcyclicGraphNodeTests;

TEST(AcyclicGraphNode, Test_AddEdge_IsAdded)
{
    auto node = make_shared<Node>();
    auto childNode = make_shared<Node>();
    node->AddEdge(StandardEdgeModel(), childNode);

    auto connectedNodes = node->CollectConnectedTo(true);
    EXPECT_EQ(connectedNodes.Count(), 1);
    EXPECT_TRUE(connectedNodes.Contains(childNode));
    EXPECT_EQ(node->Edges().Count(), 1);
    EXPECT_EQ(childNode->Edges().Count(), 0);
    EXPECT_EQ(childNode->FromNodes().Count(), 1);
}

TEST(AcyclicGraphNode, Test_AddEdges_IsAdded)
{
    auto node = make_shared<Node>();
    auto childNode1 = make_shared<Node>();
    node->AddEdge(StandardEdgeModel(), childNode1);

    auto childNode2 = make_shared<Node>();
    node->AddEdge(StandardEdgeModel(), childNode2);

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

TEST(AcyclicGraphNode, Test_Clear_RemovesEdges)
{
    auto node = make_shared<Node>();
    auto childNode1 = make_shared<Node>();
    node->AddEdge(StandardEdgeModel(), childNode1);

    auto childNode2 = make_shared<Node>();
    node->AddEdge(StandardEdgeModel(), childNode2);

    EXPECT_EQ(childNode1->FromNodes().Count(), 1);
    EXPECT_EQ(childNode2->FromNodes().Count(), 1);

    node->Clear();
    auto connectedNodes = node->CollectConnectedTo(true);
    EXPECT_EQ(connectedNodes.Count(), 0);
    EXPECT_EQ(node->Edges().Count(), 0);
    EXPECT_EQ(childNode1->FromNodes().Count(), 0);
    EXPECT_EQ(childNode2->FromNodes().Count(), 0);
}

TEST(AcyclicGraphNode, Test_UpdateRoot_UpdatesAll)
{
    auto node = make_shared<Node>();
    auto childNode1 = make_shared<Node>();
    node->AddEdge(StandardEdgeModel(), childNode1);

    auto childNode2 = make_shared<Node>();
    node->AddEdge(StandardEdgeModel(), childNode2);

    auto deepNode = make_shared<Node>();
    childNode1->AddEdge(StandardEdgeModel(), deepNode);

    auto delta = 4.0f;
    node->OnUpdate(TimeSlice(delta));

    EXPECT_EQ(node->time, delta);
    EXPECT_EQ(childNode1->time, delta);
    EXPECT_EQ(childNode2->time, delta);
    EXPECT_EQ(deepNode->time, delta);
}

TEST(AcyclicGraphNode, Test_UpdateRootWithCircularReference_DoesNotUpdateAll)
{
    auto node = make_shared<Node>();
    auto childNode1 = make_shared<Node>();
    node->AddEdge(StandardEdgeModel(), childNode1);

    auto childNode2 = make_shared<Node>();
    node->AddEdge(StandardEdgeModel(), childNode2);

    auto deepNode = make_shared<Node>();
    childNode1->AddEdge(StandardEdgeModel(), deepNode);
    deepNode->AddEdge(StandardEdgeModel(), node);

    auto delta = 4.0f;
    node->OnUpdate(TimeSlice(delta));

    EXPECT_EQ(node->time, delta);
    EXPECT_EQ(childNode1->time, 0);
    EXPECT_EQ(childNode2->time, 0);
    EXPECT_EQ(deepNode->time, 0);
}

TEST(AcyclicGraphNode, Test_RemoveEdgeFromParent_RemovesBoth)
{
    auto node = make_shared<Node>();
    auto childNode1 = make_shared<Node>();
    node->AddEdge(StandardEdgeModel(), childNode1);

    EXPECT_EQ(node->Edges().Count(), 1);
    EXPECT_EQ(childNode1->FromNodes().Count(), 1);

    node->RemoveEdge(node->Edges()[0]);

    EXPECT_EQ(node->Edges().Count(), 0);
    EXPECT_EQ(childNode1->FromNodes().Count(), 0);
}

TEST(AcyclicGraphNode, Test_RemoveEdgesTo)
{
    auto node = make_shared<Node>();

    auto childNode1 = make_shared<Node>();
    node->AddEdge(StandardEdgeModel(), childNode1);

    auto childNode2 = make_shared<Node>();
    node->AddEdge(StandardEdgeModel(), childNode2);

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

TEST(AcyclicGraphNode, Test_RemoveEdgesFrom)
{
    auto node = make_shared<Node>();
    auto childNode1 = make_shared<Node>();
    node->AddEdge(StandardEdgeModel(), childNode1);

    auto deepNode = make_shared<Node>();
    deepNode->AddEdge(StandardEdgeModel(), childNode1);

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

TEST(AcyclicGraphNode, Test_CollectGraph)
{
    auto node = make_shared<Node>();
    auto childNode1 = make_shared<Node>();
    node->AddEdge(StandardEdgeModel(), childNode1);

    auto childNode2 = make_shared<Node>();
    node->AddEdge(StandardEdgeModel(), childNode2);

    auto deepNode = make_shared<Node>();
    childNode1->AddEdge(StandardEdgeModel(), deepNode);
    deepNode->AddEdge(StandardEdgeModel(), node);    // Circular connection

    auto graph = node->CollectGraph();
    EXPECT_EQ(graph.Count(), 4);
    EXPECT_TRUE(graph.Contains(node));
    EXPECT_TRUE(graph.Contains(childNode1));
    EXPECT_TRUE(graph.Contains(childNode2));
    EXPECT_TRUE(graph.Contains(deepNode));
}

TEST(AcyclicGraphNode, Test_CollectConnectedToNotDeep)
{
    auto node = make_shared<Node>();
    auto childNode1 = make_shared<Node>();
    node->AddEdge(StandardEdgeModel(), childNode1);

    auto childNode2 = make_shared<Node>();
    node->AddEdge(StandardEdgeModel(), childNode2);

    auto deepNode = make_shared<Node>();
    childNode1->AddEdge(StandardEdgeModel(), deepNode);

    auto graph = node->CollectConnectedTo(false);
    EXPECT_EQ(graph.Count(), 2);
    EXPECT_TRUE(graph.Contains(childNode1));
    EXPECT_TRUE(graph.Contains(childNode2));
}

TEST(AcyclicGraphNode, Test_CollectConnectedToDeep)
{
    auto node = make_shared<Node>();
    auto childNode1 = make_shared<Node>();
    node->AddEdge(StandardEdgeModel(), childNode1);

    auto childNode2 = make_shared<Node>();
    node->AddEdge(StandardEdgeModel(), childNode2);

    auto deepNode = make_shared<Node>();
    childNode1->AddEdge(StandardEdgeModel(), deepNode);

    auto graph = node->CollectConnectedTo(true);
    EXPECT_EQ(graph.Count(), 3);
    EXPECT_TRUE(graph.Contains(childNode1));
    EXPECT_TRUE(graph.Contains(childNode2));
    EXPECT_TRUE(graph.Contains(deepNode));
}

TEST(AcyclicGraphNode, Test_CollectConnectedToCircular)
{
    auto node = make_shared<Node>();
    auto childNode1 = make_shared<Node>();
    node->AddEdge(StandardEdgeModel(), childNode1);

    auto childNode2 = make_shared<Node>();
    node->AddEdge(StandardEdgeModel(), childNode2);

    auto deepNode = make_shared<Node>();
    childNode1->AddEdge(StandardEdgeModel(), deepNode);
    deepNode->AddEdge(StandardEdgeModel(), node);    // Circular connection

    auto graph = node->CollectConnectedTo(true);
    EXPECT_EQ(graph.Count(), 4);
    EXPECT_TRUE(graph.Contains(node));
    EXPECT_TRUE(graph.Contains(childNode1));
    EXPECT_TRUE(graph.Contains(childNode2));
    EXPECT_TRUE(graph.Contains(deepNode));
}

TEST(AcyclicGraphNode, Test_CollectDepthFirstGraphTree)
{
    auto node = make_shared<Node>();
    auto childNode1 = make_shared<Node>();
    node->AddEdge(StandardEdgeModel(), childNode1);

    auto childNode2 = make_shared<Node>();
    node->AddEdge(StandardEdgeModel(), childNode2);

    auto deepNode = make_shared<Node>();
    childNode1->AddEdge(StandardEdgeModel(), deepNode);

    auto graph = node->CollectDepthFirstGraph();
    EXPECT_EQ(graph.Count(), 4);
    EXPECT_EQ(node, graph[0]);
    EXPECT_EQ(childNode1, graph[1]);
    EXPECT_EQ(deepNode, graph[2]);
    EXPECT_EQ(childNode2, graph[3]);
}

TEST(AcyclicGraphNode, Test_CollectDepthFirstGraphTree2)
{
    auto node = make_shared<Node>();
    auto childNode1 = make_shared<Node>();
    node->AddEdge(StandardEdgeModel(), childNode1);

    auto childNode2 = make_shared<Node>();
    node->AddEdge(StandardEdgeModel(), childNode2);

    auto deepNode = make_shared<Node>();
    childNode2->AddEdge(StandardEdgeModel(), deepNode);

    auto graph = node->CollectDepthFirstGraph();
    EXPECT_EQ(graph.Count(), 4);
    EXPECT_EQ(node, graph[0]);
    EXPECT_EQ(childNode1, graph[1]);
    EXPECT_EQ(childNode2, graph[2]);
    EXPECT_EQ(deepNode, graph[3]);
}

TEST(AcyclicGraphNode, Test_CollectDepthFirstGraphCircular)
{
    auto node = make_shared<Node>();
    auto childNode1 = make_shared<Node>();
    node->AddEdge(StandardEdgeModel(), childNode1);

    auto childNode2 = make_shared<Node>();
    node->AddEdge(StandardEdgeModel(), childNode2);

    auto deepNode = make_shared<Node>();
    childNode1->AddEdge(StandardEdgeModel(), deepNode);
    deepNode->AddEdge(StandardEdgeModel(), node);  // Circular connection

    auto graph = node->CollectDepthFirstGraph();
    EXPECT_EQ(graph.Count(), 4);
    EXPECT_EQ(node, graph[0]);
    EXPECT_EQ(childNode1, graph[1]);
    EXPECT_EQ(deepNode, graph[2]);
    EXPECT_EQ(childNode2, graph[3]);
}

TEST(AcyclicGraphNode, Test_CollectBreadthFirstGraphTree)
{
    auto node = make_shared<Node>();
    auto childNode1 = make_shared<Node>();
    node->AddEdge(StandardEdgeModel(), childNode1);

    auto childNode2 = make_shared<Node>();
    node->AddEdge(StandardEdgeModel(), childNode2);

    auto deepNode = make_shared<Node>();
    childNode1->AddEdge(StandardEdgeModel(), deepNode);

    auto graph = node->CollectBreadthFirstGraph();
    EXPECT_EQ(4, graph.Count());
    EXPECT_EQ(node, graph[0]);
    EXPECT_EQ(childNode1, graph[1]);
    EXPECT_EQ(childNode2, graph[2]);
    EXPECT_EQ(deepNode, graph[3]);
}

TEST(AcyclicGraphNode, Test_CollectBreadthFirstGraphTree2)
{
    auto node = make_shared<Node>();
    auto childNode1 = make_shared<Node>();
    node->AddEdge(StandardEdgeModel(), childNode1);

    auto childNode2 = make_shared<Node>();
    node->AddEdge(StandardEdgeModel(), childNode2);

    auto deepNode = make_shared<Node>();
    childNode2->AddEdge(StandardEdgeModel(), deepNode);

    auto graph = node->CollectBreadthFirstGraph();
    EXPECT_EQ(4, graph.Count());
    EXPECT_EQ(node, graph[0]);
    EXPECT_EQ(childNode1, graph[1]);
    EXPECT_EQ(childNode2, graph[2]);
    EXPECT_EQ(deepNode, graph[3]);
}

TEST(AcyclicGraphNode, Test_CollectBreadthFirstGraphCircular)
{
    auto node = make_shared<Node>();
    auto childNode1 = make_shared<Node>();
    node->AddEdge(StandardEdgeModel(), childNode1);

    auto childNode2 = make_shared<Node>();
    node->AddEdge(StandardEdgeModel(), childNode2);

    auto deepNode = make_shared<Node>();
    childNode1->AddEdge(StandardEdgeModel(), deepNode);
    deepNode->AddEdge(StandardEdgeModel(), node);  // Circular connection

    auto graph = node->CollectBreadthFirstGraph();
    EXPECT_EQ(4, graph.size());
    EXPECT_EQ(node, graph[0]);
    EXPECT_EQ(childNode1, graph[1]);
    EXPECT_EQ(childNode2, graph[2]);
    EXPECT_EQ(deepNode, graph[3]);
}

TEST(AcyclicGraphNode, Test_RootNode)
{
    auto node = make_shared<Node>();
    auto childNode1 = make_shared<Node>();
    node->AddEdge(StandardEdgeModel(), childNode1);

    auto childNode2 = make_shared<Node>();
    node->AddEdge(StandardEdgeModel(), childNode2);

    auto deepNode = make_shared<Node>();
    childNode1->AddEdge(StandardEdgeModel(), deepNode);

    EXPECT_EQ(node, node->RootNode());
    EXPECT_EQ(node, childNode1->RootNode());
    EXPECT_EQ(node, childNode2->RootNode());
    EXPECT_EQ(node, deepNode->RootNode());
}

TEST(AcyclicGraphNode, Test_RootNodeCircular)
{
    auto node = make_shared<Node>();
    auto childNode1 = make_shared<Node>();
    node->AddEdge(StandardEdgeModel(), childNode1);

    auto childNode2 = make_shared<Node>();
    node->AddEdge(StandardEdgeModel(), childNode2);

    auto deepNode = make_shared<Node>();
    childNode1->AddEdge(StandardEdgeModel(), deepNode);
    deepNode->AddEdge(StandardEdgeModel(), node);

    EXPECT_EQ(nullptr, node->RootNode());
    EXPECT_EQ(nullptr, childNode2->RootNode());
    EXPECT_EQ(nullptr, childNode1->RootNode());
    EXPECT_EQ(nullptr, deepNode->RootNode());
}

TEST(AcyclicGraphNode, Test_IsRootNode)
{
    auto node = make_shared<Node>();
    auto childNode1 = make_shared<Node>();
    node->AddEdge(StandardEdgeModel(), childNode1);

    EXPECT_TRUE(node->IsRootNode());
    EXPECT_FALSE(childNode1->IsRootNode());
}

TEST(AcyclicGraphNode, Test_Parent)
{
    auto node = make_shared<Node>();
    auto childNode1 = make_shared<Node>();
    node->AddEdge(StandardEdgeModel(), childNode1);

    EXPECT_EQ(nullptr, node->Parent());
    EXPECT_EQ(node, childNode1->Parent());
}
