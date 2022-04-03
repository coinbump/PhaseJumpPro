import XCTest
@testable import PhaseJumpPro

final class GraphNodeTests: XCTestCase {
    struct EdgeModel: Hashable {
        let identifier = UUID().uuidString
    }

    private class Node: GraphNode<EdgeModel>
    {
        public var time = 0.0

        public override func evtUpdate(with timeSlice: TimeSlice)
        {
            self.time += timeSlice.delta
        }
    }

    func testAddEdge_IsAdded()
    {
        let node = Node()
        let childNode = Node()
        node.add(model: EdgeModel(), toNode: childNode)

        let connectedNodes = node.collectConnectedTo(isDeep: true)
        XCTAssertEqual(connectedNodes.count, 1)
        XCTAssertTrue(connectedNodes.contains(childNode))
        XCTAssertEqual(node.edges.count, 1)
        XCTAssertEqual(childNode.edges.count, 0)
        XCTAssertEqual(childNode.fromNodes.count, 1)
    }

    func testAddedges_IsAdded()
    {
        let node = Node()
        let childNode1 = Node()
        node.add(model: EdgeModel(), toNode: childNode1)

        let childNode2 = Node()
        node.add(model: EdgeModel(), toNode: childNode2)

        let connectedNodes = node.collectConnectedTo(isDeep: true)
        XCTAssertEqual(connectedNodes.count, 2)
        XCTAssertTrue(connectedNodes.contains(childNode1))
        XCTAssertTrue(connectedNodes.contains(childNode2))
        XCTAssertEqual(node.edges.count, 2)
        XCTAssertEqual(childNode1.edges.count, 0)
        XCTAssertEqual(childNode1.fromNodes.count, 1)
        XCTAssertEqual(childNode2.edges.count, 0)
        XCTAssertEqual(childNode2.fromNodes.count, 1)
    }

    func testClear_Removesedges()
    {
        let node = Node()
        let childNode1 = Node()
        node.add(model: EdgeModel(), toNode: childNode1)

        let childNode2 = Node()
        node.add(model: EdgeModel(), toNode: childNode2)

        XCTAssertEqual(childNode1.fromNodes.count, 1)
        XCTAssertEqual(childNode2.fromNodes.count, 1)

        node.clear()
        let connectedNodes = node.collectConnectedTo(isDeep: true)
        XCTAssertEqual(connectedNodes.count, 0)
        XCTAssertEqual(node.edges.count, 0)
        XCTAssertEqual(childNode1.fromNodes.count, 0)
        XCTAssertEqual(childNode2.fromNodes.count, 0)
    }

    func testUpdateRoot_UpdatesAll()
    {
        let node = Node()
        let childNode1 = Node()
        node.add(model: EdgeModel(), toNode: childNode1)

        let childNode2 = Node()
        node.add(model: EdgeModel(), toNode: childNode2)

        let deepNode = Node()
        childNode1.add(model: EdgeModel(), toNode: deepNode)

        let delta = 4.0
        node.evtUpdateRoot(with: TimeSlice(delta: delta))

        XCTAssertEqual(node.time, delta)
        XCTAssertEqual(childNode1.time, delta)
        XCTAssertEqual(childNode2.time, delta)
        XCTAssertEqual(deepNode.time, delta)
    }

    func testUpdateRootWithCircularReference_UpdatesAll()
    {
        let node = Node()
        let childNode1 = Node()
        node.add(model: EdgeModel(), toNode: childNode1)

        let childNode2 = Node()
        node.add(model: EdgeModel(), toNode: childNode2)

        let deepNode = Node()
        childNode1.add(model: EdgeModel(), toNode: deepNode)
        deepNode.add(model: EdgeModel(), toNode: node)

        let delta = 4.0
        node.evtUpdateRoot(with: TimeSlice(delta: delta))

        XCTAssertEqual(node.time, delta)
        XCTAssertEqual(childNode1.time, delta)
        XCTAssertEqual(childNode2.time, delta)
        XCTAssertEqual(deepNode.time, delta)
    }

    func testRemoveEdgeFromParent_RemovesBoth()
    {
        let node = Node()
        let childNode1 = Node()
        node.add(model: EdgeModel(), toNode: childNode1)

        XCTAssertEqual(node.edges.count, 1)
        XCTAssertEqual(childNode1.fromNodes.count, 1)

        node.remove(edge: node.edges[0])

        XCTAssertEqual(node.edges.count, 0)
        XCTAssertEqual(childNode1.fromNodes.count, 0)
    }

    func testRemoveedgesTo()
    {
        let node = Node()

        let childNode1 = Node()
        node.add(model: EdgeModel(), toNode: childNode1)

        let childNode2 = Node()
        node.add(model: EdgeModel(), toNode: childNode2)

        XCTAssertEqual(node.edges.count, 2)
        XCTAssertEqual(childNode1.edges.count, 0)
        XCTAssertEqual(childNode1.fromNodes.count, 1)
        XCTAssertEqual(childNode2.fromNodes.count, 1)

        node.removeEdges(to: childNode1)
        XCTAssertEqual(node.edges.count, 1)
        XCTAssertEqual(node.edges[0].toNode, childNode2)
        XCTAssertEqual(childNode1.fromNodes.count, 0)
        XCTAssertEqual(childNode2.fromNodes.count, 1)
    }

    func testRemoveEdges()
    {
        let node = Node()
        let childNode1 = Node()
        node.add(model: EdgeModel(), toNode: childNode1)

        let deepNode = Node()
        deepNode.add(model: EdgeModel(), toNode: childNode1)

        XCTAssertEqual(node.edges.count, 1)
        XCTAssertEqual(childNode1.fromNodes.count, 2)
        XCTAssertEqual(deepNode.edges.count, 1)
        XCTAssertEqual(deepNode.fromNodes.count, 0)

        childNode1.removeEdges(from: node)

        XCTAssertEqual(node.edges.count, 0)
        XCTAssertEqual(childNode1.fromNodes.count, 1)
        XCTAssertEqual(deepNode.edges.count, 1)
        XCTAssertEqual(deepNode.fromNodes.count, 0)

        childNode1.removeEdges(from: deepNode)

        XCTAssertEqual(node.edges.count, 0)
        XCTAssertEqual(childNode1.edges.count, 0)
        XCTAssertEqual(childNode1.fromNodes.count, 0)
        XCTAssertEqual(deepNode.edges.count, 0)
    }

    func testcollectGraph()
    {
        let node = Node()
        let childNode1 = Node()
        node.add(model: EdgeModel(), toNode: childNode1)

        let childNode2 = Node()
        node.add(model: EdgeModel(), toNode: childNode2)

        let deepNode = Node()
        childNode1.add(model: EdgeModel(), toNode: deepNode)
        deepNode.add(model: EdgeModel(), toNode: node)    // Circular connection

        let graph = node.collectGraph()
        XCTAssertEqual(graph.count, 4)
        XCTAssertTrue(graph.contains(node))
        XCTAssertTrue(graph.contains(childNode1))
        XCTAssertTrue(graph.contains(childNode2))
        XCTAssertTrue(graph.contains(deepNode))
    }

    func testCollectConnectedToNotDeep()
    {
        let node = Node()
        let childNode1 = Node()
        node.add(model: EdgeModel(), toNode: childNode1)

        let childNode2 = Node()
        node.add(model: EdgeModel(), toNode: childNode2)

        let deepNode = Node()
        childNode1.add(model: EdgeModel(), toNode: deepNode)

        let graph = node.collectConnectedTo(isDeep: false)
        XCTAssertEqual(graph.count, 2)
        XCTAssertTrue(graph.contains(childNode1))
        XCTAssertTrue(graph.contains(childNode2))
    }

    func testCollectConnectedToDeep()
    {
        let node = Node()
        let childNode1 = Node()
        node.add(model: EdgeModel(), toNode: childNode1)

        let childNode2 = Node()
        node.add(model: EdgeModel(), toNode: childNode2)

        let deepNode = Node()
        childNode1.add(model: EdgeModel(), toNode: deepNode)

        let graph = node.collectConnectedTo(isDeep: true)
        XCTAssertEqual(graph.count, 3)
        XCTAssertTrue(graph.contains(childNode1))
        XCTAssertTrue(graph.contains(childNode2))
        XCTAssertTrue(graph.contains(deepNode))
    }

    func testCollectConnectedToCircular()
    {
        let node = Node()
        let childNode1 = Node()
        node.add(model: EdgeModel(), toNode: childNode1)

        let childNode2 = Node()
        node.add(model: EdgeModel(), toNode: childNode2)

        let deepNode = Node()
        childNode1.add(model: EdgeModel(), toNode: deepNode)
        deepNode.add(model: EdgeModel(), toNode: node)    // Circular connection

        let graph = node.collectConnectedTo(isDeep: true)
        XCTAssertEqual(graph.count, 4)
        XCTAssertTrue(graph.contains(node))
        XCTAssertTrue(graph.contains(childNode1))
        XCTAssertTrue(graph.contains(childNode2))
        XCTAssertTrue(graph.contains(deepNode))
    }
}
