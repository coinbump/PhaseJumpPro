import XCTest
@testable import PhaseJumpPro

final class BinaryTreeNodeTests: XCTestCase {
    struct EdgeModel: Hashable {
        let identifier = UUID().uuidString
    }

    private class Node: BinaryTreeNode<EdgeModel>
    {
        init()
        {
            super.init(edgeModelFactory: { EdgeModel() })
        }
    }

    func testLeftChild_isAdded()
    {
        let node = Node()
        let childNode = Node()
        node.leftChild = childNode

        let connectedNodes = node.collectConnectedTo(isDeep: true)
        XCTAssertEqual(connectedNodes.count, 1)
        XCTAssertTrue(connectedNodes.contains(childNode))
        XCTAssertEqual(node.edges.count, 1)
        XCTAssertEqual(childNode.edges.count, 0)
        XCTAssertEqual(childNode.fromNodes.count, 1)
    }

    func testRightChild_isAdded()
    {
        let node = Node()
        let childNode = Node()
        node.rightChild = childNode

        let connectedNodes = node.collectConnectedTo(isDeep: true)
        XCTAssertEqual(connectedNodes.count, 1)
        XCTAssertTrue(connectedNodes.contains(childNode))
        XCTAssertEqual(node.edges.count, 1)
        XCTAssertEqual(childNode.edges.count, 0)
        XCTAssertEqual(childNode.fromNodes.count, 1)
    }

    func testLeftChild_isRemoved()
    {
        let node = Node()
        let childNode = Node()
        node.leftChild = childNode
        node.leftChild = nil

        let connectedNodes = node.collectConnectedTo(isDeep: true)
        XCTAssertEqual(connectedNodes.count, 0)
        XCTAssertEqual(node.edges.count, 0)
        XCTAssertEqual(childNode.edges.count, 0)
        XCTAssertEqual(childNode.fromNodes.count, 0)
    }

    func testRightChild_isRemoved()
    {
        let node = Node()
        let childNode = Node()
        node.rightChild = childNode
        node.rightChild = nil

        let connectedNodes = node.collectConnectedTo(isDeep: true)
        XCTAssertEqual(connectedNodes.count, 0)
        XCTAssertEqual(node.edges.count, 0)
        XCTAssertEqual(childNode.edges.count, 0)
        XCTAssertEqual(childNode.fromNodes.count, 0)
    }

    func testLeftChild_isReplaced()
    {
        let node = Node()
        let childNode = Node()
        node.leftChild = childNode

        let newNode = Node()
        node.leftChild = newNode

        let connectedNodes = node.collectConnectedTo(isDeep: true)
        XCTAssertEqual(connectedNodes.count, 1)
        XCTAssertEqual(node.edges.count, 1)
        XCTAssertEqual(childNode.edges.count, 0)
        XCTAssertEqual(childNode.fromNodes.count, 0)
        XCTAssertEqual(newNode.fromNodes.count, 1)
    }

    func testRightChild_isReplaced()
    {
        let node = Node()
        let childNode = Node()
        node.rightChild = childNode
        
        let newNode = Node()
        node.rightChild = newNode

        let connectedNodes = node.collectConnectedTo(isDeep: true)
        XCTAssertEqual(connectedNodes.count, 1)
        XCTAssertEqual(node.edges.count, 1)
        XCTAssertEqual(childNode.edges.count, 0)
        XCTAssertEqual(childNode.fromNodes.count, 0)
        XCTAssertEqual(newNode.fromNodes.count, 1)
    }
}
