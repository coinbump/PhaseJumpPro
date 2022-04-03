//
//  ContentViewModel.swift
//  pj-swift-example-dialogue
//
//  Created by Jeremy Vineyard on 4/2/22.
//

import Foundation
import PhaseJumpPro

final class ContentViewModel: ObservableObject {
    typealias Node = GraphNode<GraphStandardEdgeModel>

    struct Choice: Equatable, Hashable {
        let identifier: String
        let text: String

        public init(identifier: String = UUID().uuidString, text: String = "") {
            self.identifier = identifier
            self.text = text
        }

        func hash(into hasher: inout Hasher) {
            hasher.combine(identifier)
        }
    }

    @Published private(set) var text = ""
    @Published private(set) var choices = [Choice]()
    @Published private(set) var canGoBack = false

    var currentNode: Node? {
        didSet {
            updateForCurrentNode()
        }
    }
    var previousNodes = [Node]() {
        didSet {
            canGoBack = previousNodes.count > 0
        }
    }

    private var graph: Node?

    public init() {
        var graphNodes = [Node]()

        if let url = Bundle.main.url(forResource: "graph", withExtension: "json") {
            do {
                let data = try Data(contentsOf: url)
                let nodes = try JSONDecoder().decode(GraphNodesDecodable.self, from: data)
                let decodableNodes = nodes.nodes

                graphNodes = decodableNodes.map { decodableNode in
                    let node = Node(identifier: decodableNode.identifier)
                    node.tags["text"] = decodableNode.text
                    return node
                }

                for decodableNode in decodableNodes {
                    guard let fromNode = (graphNodes.first { $0.identifier == decodableNode.identifier }) else { continue }
                    guard let edges = decodableNode.edges else { continue }

                    for edge in edges {
                        guard let toNode = (graphNodes.first { $0.identifier == edge.identifier }) else { continue }

                        var tags = Tags()
                        tags["title"] = edge.title
                        fromNode.add(model: .init(identifier: edge.identifier, tags: tags), toNode: toNode)
                    }
                }
            }
            catch let error {
                print("Decode Error: \(error)")
            }
        }

        self.graph = graphNodes.first
        self.currentNode = graphNodes.first
        updateForCurrentNode()
    }

    func goBack() {
        guard let previousNode = previousNodes.last else { return }

        self.currentNode = previousNode

        previousNodes.remove(at: previousNodes.count - 1)
    }

    func select(choice: Choice) {
        guard let currentNode = currentNode,
              let edge = (currentNode.edges.first { $0.model.identifier == choice.identifier }) else { return }

        let node = edge.toNode
        previousNodes.append(currentNode)
        self.currentNode = node
    }

    // MARK: Private

    private func updateForCurrentNode() {
        guard let node = currentNode else { return }
        text = node.tags["text"] as? String ?? "Missing Dialogue 😔"

        choices = node.edges.map { edge in
            .init(identifier: edge.model.identifier, text: edge.model.tags["title"] as? String ?? "")
        }
    }
}
