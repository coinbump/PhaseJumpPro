import Foundation

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 4/2/22
 */

/// Use to build non cyclic graphs: trees, graphs, etc.
/// Limitation: cyclic graphs will create strong reference cycles
public class AcyclicGraphNode<EdgeModel> where EdgeModel: Hashable
{
    public typealias Node = AcyclicGraphNode<EdgeModel>

    public class Edge: Hashable
    {
        public weak var fromNode: Node?
        public var model: EdgeModel
        public var toNode: Node

        public init(fromNode: Node, model: EdgeModel, toNode: Node)
        {
            self.fromNode = fromNode
            self.model = model
            self.toNode = toNode
        }

        public func hash(into hasher: inout Hasher) {
            model.hash(into: &hasher)
        }

        public static func == (lhs: Node.Edge, rhs: Node.Edge) -> Bool {
            lhs.model == rhs.model
        }
    }

    public var identifier: String

    /// In some graphs, order matters (layers, selectors).
    public private(set) var edges = [Edge]()
    public private(set) var fromNodes = Set<WeakReference<Node>>()

    public var tags = Tags()

    public init(identifier: String = UUID().uuidString) {
        self.identifier = identifier
    }

    /// Clean up deallocated nodes
    public func cleanUp() {
        fromNodes = fromNodes.filter { $0.value != nil }
    }

    public func add(model: EdgeModel, toNode: Node)
    {
        let forwardEdge = Edge(fromNode: self, model: model, toNode: toNode)
        edges.append(forwardEdge)
        toNode.fromNodes.insert(WeakReference(value: self))
    }

    public func remove(edge: Edge)
    {
        edges = edges.filter { $0 != edge }
        edge.toNode.fromNodes.remove(WeakReference(value: self))
    }

    public func removeEdges(from fromNode: Node)
    {
        fromNode.removeEdges(to: self)
    }

    public func removeEdges(to toNode: Node)
    {
        let iterEdges = edges
        for edge in iterEdges
        {
            if edge.toNode == toNode
            {
                remove(edge: edge)
            }
        }
    }

    public func clear()
    {
        let iterEdges = edges
        for edge in iterEdges
        {
            remove(edge: edge)
        }
    }

    /// Collect all forward-directed nodes from this node
    public func collectGraph() -> Set<Node>
    {
        var nodes = Set<Node>()
        nodes.insert(self)

        var searchedNodes = Set<Node>()
        searchedNodes.insert(self)

        return collectEdgeNodes(result: nodes, searchedNodes: &searchedNodes, isDeep: true)
    }

    public func collectConnectedTo(isDeep: Bool) -> Set<Node>
    {
        let nodes = Set<Node>()
        var searchedNodes = Set<Node>()
        return collectEdgeNodes(result: nodes, searchedNodes: &searchedNodes, isDeep: isDeep)
    }

    func collectEdgeNodes(result nodes: Set<Node>, searchedNodes: inout Set<Node>, isDeep: Bool) -> Set<Node>
    {
        var nodes = nodes
        var searchedNodes = searchedNodes

        let iterEdges = edges
        for edge in iterEdges
        {
            nodes.insert(edge.toNode)

            // Prevent infinite loop
            if (isDeep && !searchedNodes.contains(edge.toNode)) {
                searchedNodes.insert(edge.toNode)
                nodes = nodes.union(edge.toNode.collectEdgeNodes(result: nodes, searchedNodes: &searchedNodes, isDeep: true))
            }
        }

        return nodes
    }

    /// Call for root node, to update entire graph
    public func onUpdateRoot(with timeSlice: TimeSlice)
    {
        let nodes = collectGraph()
        for node in nodes
        {
            node.onUpdate(with: timeSlice)
        }
    }

    /// Override if graph node needs to respond to time updates
    public func onUpdate(with timeSlice: TimeSlice)
    {
    }
}

// MARK: Hashable

extension AcyclicGraphNode: Equatable, Hashable {
    public func hash(into hasher: inout Hasher) {
        hasher.combine(identifier)
    }

    public static func == (lhs: Node, rhs: Node) -> Bool {
        lhs.identifier == rhs.identifier
    }
}
